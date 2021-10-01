//////////////////////////////////////////////////////////////////////////////
// SPDX-FileCopyrightText: 2021, Dinesh Annayya
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// SPDX-License-Identifier: Apache-2.0
// SPDX-FileContributor: Dinesh Annayya <dinesha@opencores.org>
// //////////////////////////////////////////////////////////////////////////
#define SC_SIM_OUTPORT (0xf0000000)
#define uint32_t  long

#define reg_mprj_globl_reg0  (*(volatile uint32_t*)0x10030000)
#define reg_mprj_globl_reg1  (*(volatile uint32_t*)0x10030004)
#define reg_mprj_globl_reg2  (*(volatile uint32_t*)0x10030008)
#define reg_mprj_globl_reg3  (*(volatile uint32_t*)0x1003000C)
#define reg_mprj_globl_reg4  (*(volatile uint32_t*)0x10030010)
#define reg_mprj_globl_reg5  (*(volatile uint32_t*)0x10030014)
#define reg_mprj_globl_reg6  (*(volatile uint32_t*)0x10030018)
#define reg_mprj_globl_reg7  (*(volatile uint32_t*)0x1003001C)
#define reg_mprj_globl_reg8  (*(volatile uint32_t*)0x10030020)
#define reg_mprj_globl_reg9  (*(volatile uint32_t*)0x10030024)
#define reg_mprj_globl_reg10 (*(volatile uint32_t*)0x10030028)
#define reg_mprj_globl_reg11 (*(volatile uint32_t*)0x1003002C)
#define reg_mprj_globl_reg12 (*(volatile uint32_t*)0x10030030)
#define reg_mprj_globl_reg13 (*(volatile uint32_t*)0x10030034)
#define reg_mprj_globl_reg14 (*(volatile uint32_t*)0x10030038)
#define reg_mprj_globl_reg15 (*(volatile uint32_t*)0x1003003C)
#define reg_mprj_globl_reg16 (*(volatile uint32_t*)0x10030040)
#define reg_mprj_globl_reg17 (*(volatile uint32_t*)0x10030044)
#define reg_mprj_globl_reg18 (*(volatile uint32_t*)0x10030048)
#define reg_mprj_globl_reg19 (*(volatile uint32_t*)0x1003004C)
#define reg_mprj_globl_reg20 (*(volatile uint32_t*)0x10030050)
#define reg_mprj_globl_reg21 (*(volatile uint32_t*)0x10030054)
#define reg_mprj_globl_reg22 (*(volatile uint32_t*)0x10030058)
#define reg_mprj_globl_reg23 (*(volatile uint32_t*)0x1003005C)
#define reg_mprj_globl_reg24 (*(volatile uint32_t*)0x10030060)
#define reg_mprj_globl_reg25 (*(volatile uint32_t*)0x10030064)
#define reg_mprj_globl_reg26 (*(volatile uint32_t*)0x10030068)
#define reg_mprj_globl_reg27 (*(volatile uint32_t*)0x1003006C)

int main()
{

    //volatile long *out_ptr = (volatile long*)SC_SIM_OUTPORT;
    //*out_ptr = 0xAABBCCDD;
    //*out_ptr = 0xBBCCDDEE;
    //*out_ptr = 0xCCDDEEFF;
    //*out_ptr = 0xDDEEFF00;

    // Write software Write & Read Register
    reg_mprj_globl_reg22  = 0x11223344; 
    reg_mprj_globl_reg23  = 0x22334455; 
    reg_mprj_globl_reg24  = 0x33445566; 
    reg_mprj_globl_reg25  = 0x44556677; 
    reg_mprj_globl_reg26 = 0x55667788; 
    reg_mprj_globl_reg27 = 0x66778899; 
    //reg_mprj_globl_reg12 = 0x778899AA; 
    //reg_mprj_globl_reg13 = 0x8899AABB; 
    //reg_mprj_globl_reg14 = 0x99AABBCC; 
    //reg_mprj_globl_reg15 = 0xAABBCCDD; 

    while(1) {}
    return 0;
}
