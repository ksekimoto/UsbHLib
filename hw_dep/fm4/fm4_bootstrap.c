//
// Copyright (c) 2017, Kentaro Sekimoto
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//  -Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//  -Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#include "common.h"

void bootstrap(void)
{
    // MB9BF506N
    // P00      P10      (AN00)  P30 D6        P40       P50 D0   P60
    // P01      P11      (AN01)  P31 D7        P41       P51 D1   P61
    // P02      P12 A9   (AN02)  P32 MDQM0 LB  P42 A20   P52 D2   P62 A0
    // P03      P13 A8   (AN03)  P33 MDQM1 UB  P43 A19   P53 D3   P63 A1
    // P04      P14      (AN04)  P34           P44 A18   P54 D4
    // P05      P15      (AN05)  P35           P45 A17   P55 D5
    // P06      P16 MOEX (AN06)  P36 MCSX3     P46       P56
    // P07      P17 MWEX (AN07)  P37 MCSX2     P47       P57
    // P08      P18 D8   (AN08)  P38           P48 A16   P58
    // P09      P19 D9   (AN09)  P39           P49 A15   P59
    // P0A A7   P1A D10  (AN10)  P3A           P4A A14   P5A
    // P0B A6   P1B D11  (AN11)  P3B           P4B A13   P5B
    // P0C A5   P1C D12  (AN12)  P3C           P4C A12   P5C
    // P0D A4   P1D D13  (AN13)  P3D           P4D A11   P5D
    // P0E A3   P1E D14  (AN14)  P3E           P4E A10   P5E
    // P0F A2   P1F D15  (AN15)  P3F           P4F       P5F
#ifdef MB9BF506N
    FM3_GPIO->ADE = 0x00000033;   //AN00,AN01,,,AN04,AN05
    FM3_GPIO->PFR0 = 0xfc1f;//P0A...P0F/A7...A2
    FM3_GPIO->PFR1 = 0xffcc;//P12...P1F/A9,A8,,,OE,WE,D8..D15
    FM3_GPIO->PFR3 = 0x00cf;//D6,D7,LB,UB,,,CS3,CS2
    FM3_GPIO->PFR4 = 0x7f3c;//A20,A19,A18,A17,,,A16..A10
    FM3_GPIO->PFR5 = 0x003f;//D0..D5
    FM3_GPIO->PFR6 = 0x0008;//A1
    FM3_GPIO->EPFR10 = 0x0fff833b;
    // CS3 - SRAM 2MB
    FM3_EXBUS->MODE3 = (1 << 0)//WDTH 16bit
    | (1 << 2);//RBMON
//    FM3_EXBUS->TIM3     = (0 << 28)     //WIDLC
//                        | (2 << 24)     //WWEC
//                        | (0 << 20)     //WADC
//                        | (2 << 16)     //WACC
//                        | (1 << 12)     //RIDLC
//                        | (0 << 8)      //FRADC
//                        | (0 << 4)      //RADC
//                        | (6 << 0);     //RACC
    FM3_EXBUS->TIM3 = (0 << 28)//WIDLC
    | (5 << 24)//WWEC
    | (5 << 20)//WADC
    | (15 << 16)//WACC
    | (15 << 12)//RIDLC
    | (0 << 8)//FRADC
    | (0 << 4)//RADC
    | (15 << 0);//RACC
    FM3_EXBUS->AREA3 = (0x0f << 16)//MASK 2MB
    | (0x30 << 0);//ADDR 0x63000000
    // CS2 - LCD
    FM3_EXBUS->MODE2 = (0 << 0)//WDTH 8bit
    | (1 << 2);//RBMON
    FM3_EXBUS->TIM2 = (0 << 28)//WIDLC
    | (2 << 24)//WWEC
    | (0 << 20)//WADC
    | (2 << 16)//WACC
    | (1 << 12)//RIDLC
    | (0 << 8)//FRADC
    | (0 << 4)//RADC
    | (6 << 0);//RACC
    FM3_EXBUS->AREA2 = (0x0f << 16)//MASK 2MB
    | (0x20 << 0);//ADDR 0x62000000
#endif

    // MB9BF618T
    // P10              P20 MAD18        P40       P50 MOEX nRD  P70 D5    P90 MAD19 RST
    // P11              P21              P41       P51 MWEX nWR  P71 D6
    // P12              P22              P42       P52 MDQM0 LB  P72 D7
    // P13              P23              P43       P53 MDQM1 UB  P73 D8
    // P14 MAD00(AN04)  P24 MAD17(AN29)  P44       P54           P74 D9
    // P15 MAD01(AN05)  P25 MAD16(AN28)  P45       P55           P75 D10
    // P16 MAD02(AN06)  P26 MAD15(AN27)  P46       P56           P76 D11
    // P17 MAD03(AN07)  P27 MAD14(AN26)  P47       P57           P77 D12
    // P18 MAD04(AN08)  P28 MAD13(AN25)  P48 :     P58           P78 D13
    // P19 MAD05(AN09)  P29 MAD12(AN24)  P49 :     P59           P79 D14
    // P1A MAD06(AN10)  P2A              P4A D0    P5A nCS0      P7A D15
    // P1B MAD07(AN11)  P2B              P4B D1    P5B nCS1      P7B
    // P1C MAD08(AN12)  P2C              P4C D2    P5C           P7C
    // P1D MAD09(AN13)  P2D              P4D D3    P5D           P7D
    // P1E MAD10(AN14)  P2E              P4E D4    P5E           P7E
    // P1F MAD11(AN15)  P2F              P4F :     P5F           P7F
#ifdef MB9BF618T
    FM3_GPIO->ADE = 0;
    FM3_EXBUS->DCLKR = (0 << 4)      //MCLKON
    | (1 << 0);//MDIV
    FM3_EXBUS->DCLKR = 0x00000011;
    // CS0 - SRAM 2MB
    FM3_EXBUS->MODE0 = (1 << 0)//WDTH 16bit
    | (1 << 2);//RBMON
    FM3_EXBUS->TIM0 = (0 << 28)//WIDLC
    | (3 << 24)//WWEC
    | (0 << 20)//WADC
    | (4 << 16)//WACC
    | (0 << 12)//RIDLC
    | (0 << 8)//FRADC
    | (0 << 4)//RADC
    | (8 << 0);//RACC
    FM3_EXBUS->AREA0 = (0x0f << 16)//MASK
    | (0x00 << 0);//ADDR 0x60000000
    // CS1 - LCD via EXBUS
    FM3_EXBUS->MODE1 = (0 << 0);//WDTH 8bit
    FM3_EXBUS->TIM1 = (0 << 28)//WIDLC
    | (5 << 24)//WWEC
    | (5 << 20)//WADC
    | (15 << 16)//WACC
    | (15 << 12)//RIDLC
    | (0 << 8)//FRADC
    | (0 << 4)//RADC
    | (15 << 0);//RACC
    FM3_EXBUS->AREA1 = (0x0f << 16)//MASK
    | (0x10 << 0);//ADDR 0x61000000
    FM3_GPIO->PFR1 = 0xfff0;//P14...P1F/A0...A11
    FM3_GPIO->PFR2 = 0x03f1;//P20,P24...P29/A18,A12...A17
    FM3_GPIO->PFR4 = 0x7c00;//P4A...P4E/D0...D4
    FM3_GPIO->PFR5 = 0x0c0f;//P50...P53,P5A,P5B/MOEX,MWEX,MDQM0,MDQM1,CS0,CS1
    FM3_GPIO->PFR7 = 0x07ff;//P70...P7A/D5...D15
    FM3_GPIO->PFR9 = 0x0007;//P90,P91,P92
    FM3_GPIO->EPFR10 = 0x1fff80bb;
    FM3_GPIO->EPFR11 = 0x01fffffe;
#endif
    // MB9BF568R
    //
    //
    // P00        P10 MAD07(AN00)  P20 MAD24(AN18)  P30 MDTAA12 P40          P50 MDATA00   P70
    // P01        P11 MAD08(AN01)  P21 MAD23(AN17)  P31 MDATA13 P41          P51 MDATA01   P71
    // P02        P12 MAD09(AN02)  P22      (AN16)  P32 MDATA14 P42 MSDWEX   P52 MDATA02   P72
    // P03        P13 MAD10(AN03)  P23 MAD22(AN15)  P33 MDATA15 P43 MCSX08   P53 MDATA03   P73
    // P04        P14 MAD11(AN04)  P24              P34         P44          P54 MDATA04   P74
    // P05        P15 MAD12(AN05)  P25              P35         P45          P55 MDATA05   P75
    // P06        P16 MAD13(AN06)  P26              P36         P46          P56 MDATA06   P76
    // P07        P17 MAD14(AN07)  P27              P37         P47          P57 MDATA07   P77
    // P08        P18 MAD15(AN08)  P28              P38         P48          P58 MDATA08   P78
    // P09        P19 MAD16(AN09)  P29              P39 MSDCLK  P49          P59 MDATA09   P79
    // P0A        P1A MAD17(AN10)  P2A              P3A MSDCKE  P4A          P5A MDATA10   P7A
    // P0B        P1B MAD18(AN11)  P2B              P3B MRASX   P4B MAD03    P5B MDATA11   P7B
    // P0C        P1C MAD19(AN12)  P2C              P3C MCASX   P4C MAD04    P5C           P7C
    // P0D MDQM0  P1D MAD20(AN13)  P2D              P3D MAD00   P4D MAD05    P5D           P7D
    // P0E MDQM1  P1E MAD21(AN14)  P2E              P3E MAD01   P4E MAD06    P5E           P7E
    // P0F        P1F              P2F              P3F MAD02   P4F          P5F           P7F

#ifdef MB9BF568R
    volatile unsigned int t;
    FM4_GPIO->ADE = 0x00000000;

    // CS8 - SDRAM 32MB
    // Micron MT48LC16M16A2 Column 512[8:0] x Row 8K[12:0] x Bank 4[1:0]
    // BASEL: 100 [23:24]
    // RASEL: 100 [10:22]
    // CASEL: 0   [01:09]
    // CS8 - SDRAM 16MB
    // Micron MT48LC16M16A2 Column 512[8:0] x Row 8K[12:0] x Bank 4[1:0]
    // BASEL: 011 [22:23]
    // RASEL: 100 [10:21]
    // CASEL: 0   [01:09]
    FM4_EXBUS->SDMODE = 0x00010000;//MSDCLKOFF=1, SDON=0(OFF)
    FM4_EXBUS->DCLKR = (0 << 4)//MCLKON
    | (4 << 0);//MDIV 1/5 160MHz/5 = 32MHz
    //FM4_EXBUS->SDMODE |= 0x00004400;
    FM4_EXBUS->SDMODE |= 0x00003400;
    FM4_EXBUS->REFTIM = 0x00020033;//NREF=3,REFC=33
    //FM4_EXBUS->SDTIM = 0x00421151;
    FM4_EXBUS->SDTIM = 0x00421141;
    FM4_EXBUS->PWRDWN = 0xffff;
    FM4_EXBUS->AMODE = 0x0000;
    // GPIO
    //FM4_GPIO->PFR0      |= 0x6000;      //P0D:MDQM0,P0E:MDQM1
    //FM4_GPIO->PFR1      = 0x01ff;       //P10...P18/A7...A15
    //FM4_GPIO->PFR3      = 0xfe0f;
    //FM4_GPIO->PFR4      = 0x780c;
    //FM4_GPIO->PFR5      = 0x0fff;
    //FM4_GPIO->EPFR10    = 0x007fc013;
    //FM4_GPIO->EPFR11    = 0x01fffffc;
    //FM4_GPIO->EPFR20    = 0x0000003f;

    FM4_GPIO->PFR0 |= 0x6400;//P0D:MDQM0,P0E:MDQM1,P0A:CS1
    //FM4_GPIO->PFR1      = 0x05ff;       //P10...P18/A7...A15,A17
    FM4_GPIO->PFR1 = 0x01ff;//P10...P18/A7...A15
    FM4_GPIO->PFR3 = 0xfe0f;
    FM4_GPIO->PFR4 = 0x780c;
    FM4_GPIO->PFR5 = 0x0fff;
    FM4_GPIO->PFR6 = 0x000c;
    FM4_GPIO->EPFR10 = 0x0fffc0bb;//A17,CS1,OE,WE
    FM4_GPIO->EPFR11 = 0x01fffffc;
    FM4_GPIO->EPFR20 = 0x0000003f;

    FM4_EXBUS->SDMODE &= 0xfffeffff;
    t = 1000;
    while (t-- >0);//wait for clk stability
    FM4_EXBUS->SDMODE |= 0x00000001;
    // CS1 - LCD
    FM4_EXBUS->MODE1 = (0 << 0);//WDTH 8bit
    FM4_EXBUS->TIM1 = (0 << 28)//WIDLC
    | (5 << 24)//WWEC
    | (5 << 20)//WADC
    | (15 << 16)//WACC
    | (15 << 12)//RIDLC
    | (0 << 8)//FRADC
    | (0 << 4)//RADC
    | (15 << 0);//RACC
    FM4_EXBUS->AREA1 = (0x0f << 16)//MASK
    | (0x10 << 0);//ADDR 0x61000000
#endif
}

