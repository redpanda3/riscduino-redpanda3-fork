## YCR Core

### Common Link

EAS: https://github.com/syntacore/scr1/blob/master/docs/scr1_eas.pdf

### ICPC

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








