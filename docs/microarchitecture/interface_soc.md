## Common Interfaces in SOC Level


### Reg Bus Interface

IO or peripherals could be configured by wishbone bus. Below is the common register bus interface. reg_cs is 'chip select'. reg_be is 'byte enable'. 
reg_ack is pulled up when reg_rdata is ready, then pulled down. 

```
input logic           reg_cs,  
input logic           reg_wr,
input logic [3:0]     reg_addr,
input logic [31:0]    reg_wdata,
input logic [3:0]     reg_be,

// Outputs
output logic [31:0]   reg_rdata,
output logic          reg_ack,
```







