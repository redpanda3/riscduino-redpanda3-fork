## YCR Core

### Common Link

EAS: https://github.com/syntacore/scr1/blob/master/docs/scr1_eas.pdf

### IPIC

core/pipeline/ycr_icpc.sv

This is a type of PLINT, its function is to manage the external interrupts and the interrupt in service. It is corresponding to EAS chapter 7.

```
module ycr_ipic
(
    // Common
    input   logic                                   rst_n,                  // IPIC reset
    input   logic                                   clk,                    // IPIC clock

    // External Interrupt lines
    input   logic [YCR_IRQ_LINES_NUM-1:0]          soc2ipic_irq_lines_i,   // External IRQ lines

    // CSR <-> IPIC interface
    input   logic                                   csr2ipic_r_req_i,       // IPIC read request
    input   logic                                   csr2ipic_w_req_i,       // IPIC write request
    input   logic [2:0]                             csr2ipic_addr_i,        // IPIC address
    input   logic [`YCR_XLEN-1:0]                  csr2ipic_wdata_i,       // IPIC write data
    output  logic [`YCR_XLEN-1:0]                  ipic2csr_rdata_o,       // IPIC read data
    output  logic                                   ipic2csr_irq_m_req_o    // IRQ request from IPIC
);
```

External interrupt lines corresponds to the external interrupt, for example, keyboard, mouse, etc. Another type of the interrupt is internal interrupt, or ISV, interrupt in service, those are managed by software.

```
typedef struct packed { // cp.6
    logic                                   vd;
    logic                                   idx;
} type_ycr_search_one_2_s;

typedef struct packed { // cp.6
    logic                                   vd;
    logic   [YCR_IRQ_VECT_WIDTH-1:0]       idx;
} type_ycr_search_one_16_s;

typedef struct packed {
    logic                                   ip;
    logic                                   ie;
    logic                                   im;
    logic                                   inv;
    logic                                   is;
    logic   [YCR_IRQ_LINES_WIDTH-1:0]      line;
} type_ycr_icsr_m_s;

typedef struct packed {
    logic                                   ip;
    logic                                   ie;
} type_ycr_cicsr_s;
```

vd and idx are used the prioroty encoder mentioned later, for searching the leading one position. ip means interrupt pending, ie means interrupt enable, im means interrupt mode(edge trigger or level trigger), inv means interrupt inversion, is means the current interrupt in service or not, line means the external interrupt. cicsr means current in service csr. 

```
//-------------------------------------------------------------------------------
// Local functions declaration
//-------------------------------------------------------------------------------
```
This part is to search the leading one position, it is the same as the priority encoder. 

```
//------------------------------------------------------------------------------
// IRQ lines handling
//------------------------------------------------------------------------------

`ifdef YCR_IPIC_SYNC_EN
// IRQ lines synchronization
//------------------------------------------------------------------------------

always_ff @(posedge clk, negedge rst_n) begin
    if (~rst_n) begin
        irq_lines_sync <= '0;
        irq_lines      <= '0;
    end else begin
        irq_lines_sync <= soc2ipic_irq_lines_i;
        irq_lines      <= irq_lines_sync;
    end
end
`else // YCR_IPIC_SYNC_EN
assign irq_lines = soc2ipic_irq_lines_i;
`endif // YCR_IPIC_SYNC_EN

```

This is the optional sync stage.

![ipic](img/ipic_0.PNG)

The following code is for interrupt level detection or edge detection. 

```
// IRQ lines level detection
//------------------------------------------------------------------------------

assign irq_lvl = irq_lines ^ ipic_iinvr_next;

// IRQ lines edge detection
//------------------------------------------------------------------------------

always_ff @(negedge rst_n, posedge clk) begin
    if (~rst_n) begin
        irq_lines_dly <= '0;
    end else begin
        irq_lines_dly <= irq_lines;
    end
end

assign irq_edge_detected = (irq_lines_dly ^ irq_lines) & irq_lvl;
```

The following, first, the reader is csr, csr read the status of IPIC. CSR needs to know a detailed information, for example, the interrupt index in the table, is the interrupt is pending, is it enabled, interrupt mode, is it in service. 
```
//------------------------------------------------------------------------------
// IPIC registers read/write interface
//------------------------------------------------------------------------------

// Read Logic
//------------------------------------------------------------------------------
```

for the write logic 
```
// Write logic
//------------------------------------------------------------------------------
// Register selection
always_comb begin
    cicsr_wr_req = 1'b0;
    eoi_wr_req   = 1'b0;
    soi_wr_req   = 1'b0;
    idxr_wr_req  = 1'b0;
    icsr_wr_req  = 1'b0;
    if (csr2ipic_w_req_i) begin
        case (csr2ipic_addr_i)
            YCR_IPIC_CISV : begin end // Quiet Read-Only
            YCR_IPIC_CICSR: cicsr_wr_req = 1'b1;
            YCR_IPIC_IPR  : begin end
            YCR_IPIC_ISVR : begin end // Quiet Read-Only
            YCR_IPIC_EOI  : eoi_wr_req   = 1'b1;
            YCR_IPIC_SOI  : soi_wr_req   = 1'b1;
            YCR_IPIC_IDX  : idxr_wr_req  = 1'b1;
            YCR_IPIC_ICSR : icsr_wr_req  = 1'b1;
            default : begin // Illegal IPIC register address
                cicsr_wr_req = 'x;
                eoi_wr_req   = 'x;
                soi_wr_req   = 'x;
                idxr_wr_req  = 'x;
                icsr_wr_req  = 'x;
            end
        endcase
    end
end
```

Quiet read means directly read from ipic2csr_rdata_o, ipic2csr_rdata_o is send directly to CSR. 

from line 395 to 607 are the action of each register, it is based on each type of register

In the CISV register, irq_serv_valid means no void interrupt vector(0x10), the idx is the vector index, which is the low 4-bit. IRQ in service is totally 16. CISV updating strategy is at the starting of the interrupt request or ending of the interrupt request.

In the CICSR register, CICSR needs to know which interrupt is currently served and the status of the current register, is it in pending or enabled.

In the EOI register, writing any value to EOI register and the irq service is valid, means the request is end of interrupt.

Down to the Priority IRQ generation acts like the same function as the functions declared in the local function. Those are to get the index parameter as the priority encoder. 


### CSR

CSR is a module to keep the status of CPU, especially machine mode. Let's go to an example of assembly code.

```
.section .text
.globl _start
_start:

    # Reading CSR
    csrr a0, mtime       # Read mtime into register a0
    sw a0, mtime_val    # Store the mtime value into memory

    # Writing to CSR
    li a1, 0xFF        # Load immediate value into a1
    csrw mtime, a1     # Write the value in a1 to mtime

    # Reading CSR after writing to confirm
    csrr a0, mtime
    sw a0, mtime_val_after_write  # Store the new mtime value into memory

    # End of the program
    j _start

.section .data
mtime_val: .word 0
mtime_val_after_write: .word 0
```

So, firstly, csr is a type of register to peripheral, in the example, csr read the mtime info and write back to mtime.

Another example is how csr handles interrupts. 

```
.section .text
.globl _start
_start:

    # Enable Machine Interrupts in mstatus (set MIE bit)
    csrr t0, mstatus    # Read mstatus into temporary register t0
    li t1, 0x8          # Load immediate with the value of MIE bit
    or t0, t0, t1       # Set the MIE bit in mstatus
    csrw mstatus, t0    # Write back to mstatus

    # Enable Machine Timer Interrupts in mie (set MTIE bit)
    csrr t0, mie        # Read mie into temporary register t0
    li t1, 0x80         # Load immediate with the value of MTIE bit
    or t0, t0, t1       # Set the MTIE bit in mie
    csrw mie, t0        # Write back to mie

    # The Machine Timer Interrupt is now enabled.
    # The system will jump to the Machine Trap-Vector Base-Address (mtvec) when a Machine Timer Interrupt occurs.
```

In this example, the code first reads the mstatus CSR into a temporary register, sets the MIE bit (bit 3) to enable machine interrupts, and then writes the value back to the mstatus CSR. It then reads the mie CSR, sets the MTIE bit (bit 7) to enable machine timer interrupts, and writes the value back to the mie CSR.

When a machine timer interrupt occurs, the system will jump to the address stored in the mtvec CSR. You would need to set up the mtvec register and provide an interrupt service routine at the specified address. 

Another example is how csr handles trap or exceptions:

```
.section .text
.globl _start
_start:

    # Set up the Machine Trap-Vector Base-Address (mtvec) to point to our trap handler
    la t0, trap_handler
    csrw mtvec, t0

    # An illegal instruction to cause a trap
    .word 0x0

trap_handler:
    # Save the registers that we're going to use
    addi sp, sp, -16
    sw ra, 12(sp)
    sw a0, 8(sp)

    # Load the mcause CSR into a0 to find the cause of the trap
    csrr a0, mcause

    # Print the mcause value (you'll need to provide the print_dword function)
    call print_dword

    # Load the mtval CSR into a0 to find additional information about the trap
    csrr a0, mtval

    # Print the mtval value
    call print_dword

    # Restore the saved registers and return from the trap handler
    lw a0, 8(sp)
    lw ra, 12(sp)
    addi sp, sp, 16
    mret

.section .data
```

From the software perspective, csr is a kind of module to keep the status of CPU, did the following: 1. peripheral register 2. interrupt 3. handle the execptions. If hypervisor included, cases would be harder. SCRv1 has no hypervisor.  

Below is the hardware design of csr module.

Here are the inputs and outputs description

```

input   logic                                       soc2csr_irq_ext_i,          // External interrupt request
input   logic                                       soc2csr_irq_soft_i,         // Software interrupt request
input   logic                                       soc2csr_irq_mtimer_i,       // External timer interrupt request
```

external interrupt, for example, is the keyboard or mouse input. Software interrupt means some program finished. Timer interrupt is the 1ms or 1us timer.

```
// Memory-mapped external timer
input   logic [63:0]                                soc2csr_mtimer_val_i,       // External timer value
```

Timer value is the timer value

```
// MHARTID fuse
input   logic [`YCR_XLEN-1:0]                      soc2csr_fuse_mhartid_i,     // MHARTID fuse
```
mhartid is the register value of the current thread id.

```
// CSR <-> EXU read/write interface
input   logic                                       exu2csr_r_req_i,            // CSR read/write address
input   logic [YCR_CSR_ADDR_WIDTH-1:0]             exu2csr_rw_addr_i,          // CSR read request
output  logic [`YCR_XLEN-1:0]                      csr2exu_r_data_o,           // CSR read data
input   logic                                       exu2csr_w_req_i,            // CSR write request
input   type_ycr_csr_cmd_sel_e                     exu2csr_w_cmd_i,            // CSR write command
input   logic [`YCR_XLEN-1:0]                      exu2csr_w_data_i,           // CSR write data
output  logic                                       csr2exu_rw_exc_o,           // CSR read/write access exception

// CSR <-> EXU event interface
input   logic                                       exu2csr_take_irq_i,         // Take IRQ trap
input   logic                                       exu2csr_take_exc_i,         // Take exception trap
input   logic                                       exu2csr_mret_update_i,      // MRET update CSR
input   logic                                       exu2csr_mret_instr_i,       // MRET instruction
input   logic [YCR_EXC_CODE_WIDTH_E-1:0]          exu2csr_exc_code_i,         // Exception code (see ycr_arch_types.svh) - cp.7
input   logic [`YCR_XLEN-1:0]                      exu2csr_trap_val_i,         // Trap value
output  logic                                       csr2exu_irq_o,              // IRQ request
output  logic                                       csr2exu_ip_ie_o,            // Some IRQ pending and locally enabled
output  logic                                       csr2exu_mstatus_mie_up_o,   // MSTATUS or MIE update in the current cycle

```
Execution unit in ycr core is essential. There are two kinds of interface, one is read/write interface, read/write interface allows other unit to write, set or clear csr status, the other kind of interface is event interface, for example, some csr event, 















































