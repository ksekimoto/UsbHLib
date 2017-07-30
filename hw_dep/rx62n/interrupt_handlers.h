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

#ifndef INTERRUPT_HANDLERS_H
#define INTERRUPT_HANDLERS_H

// Exception(Supervisor Instruction)
void INT_Excep_SuperVisorInst(void) __attribute__ ((interrupt));

// Exception(Access Instruction)
void INT_Excep_AccessInst(void) __attribute__ ((interrupt));

// Exception(Undefined Instruction)
void INT_Excep_UndefinedInst(void) __attribute__ ((interrupt));

// Exception(Floating Point)
void INT_Excep_FloatingPoint(void) __attribute__ ((interrupt));

// NMI
void INT_NonMaskableInterrupt(void) __attribute__ ((interrupt));

// Dummy
void Dummy(void) __attribute__ ((interrupt));

// BRK
void INT_Excep_BRK(void) __attribute__ ((interrupt));
//;0x0000  Reserved

//;0x0004  Reserved

//;0x0008  Reserved

//;0x000C  Reserved

//;0x0010  Reserved

//;0x0014  Reserved

//;0x0018  Reserved

//;0x001C  Reserved

//;0x0020  Reserved

//;0x0024  Reserved

//;0x0028  Reserved

//;0x002C  Reserved

//;0x0030  Reserved

//;0x0034  Reserved

//;0x0038  Reserved

//;0x003C  Reserved

//;0x0040  BUSERR
void INT_Excep_BSC_BUSERR(void) __attribute__ ((interrupt));
//;0x0044  Reserved

//;0x0048  Reserved

//;0x004C  Reserved

//;0x0050  Reserved

//;0x0054 FIFERR
void INT_Excep_FCU_FIFERR(void) __attribute__ ((interrupt));

//;0x0058  Reserved

//;0x005C  FRDYI
void INT_Excep_FCU_FRDYI(void) __attribute__ ((interrupt));

//;0x0060  Reserved

//;0x0064  Reserved

//;0x0068  Reserved

//;0x006C  SWINT
void INT_Excep_ICU_SWINT(void) __attribute__ ((interrupt));

//;0x0070  CMTU0_CMT0
void INT_Excep_CMT0_CMI0(void) __attribute__ ((interrupt));

//;0x0074  CMTU0_CMT1
void INT_Excep_CMT1_CMI1(void) __attribute__ ((interrupt));

//;0x0078  CMTU1_CMT2
void INT_Excep_CMT2_CMI2(void) __attribute__ ((interrupt));

//;0x007C  CMTU1_CMT3
void INT_Excep_CMT3_CMI3(void) __attribute__ ((interrupt));

//;0x0080  EINT
void INT_Excep_ETHER_EINT(void) __attribute__ ((interrupt));

//;0x0084  Reserved

//;0x0088  Reserved

//;0x008C  Reserved

//;0x0090  USB0_D0FIFO0
void INT_Excep_USB0_D0FIFO0(void) __attribute__ ((interrupt));

//;0x0094  USB0_D1FIFO0
void INT_Excep_USB0_D1FIFO0(void) __attribute__ ((interrupt));

//;0x0098  USB0_USBI0
void INT_Excep_USB0_USBI0(void) __attribute__ ((interrupt));

//;0x009C  Reserved

//;0x00A0  USB1_D0FIFO1
void INT_Excep_USB1_D0FIFO1(void) __attribute__ ((interrupt));

//;0x00A4  USB1_D1FIFO1
void INT_Excep_USB1_D1FIFO1(void) __attribute__ ((interrupt));

//;0x00A8  USB1_USBI1
void INT_Excep_USB1_USBI1(void) __attribute__ ((interrupt));

//;0x00AC  Reserved

//;0x00B0  RSPI0_SPEI0
void INT_Excep_RSPI0_SPEI0(void) __attribute__ ((interrupt));

//;0x00B4  RSPI0_SPRI0
void INT_Excep_RSPI0_SPRI0(void) __attribute__ ((interrupt));

//;0x00B8  RSPI0_SPTI0
void INT_Excep_RSPI0_SPTI0(void) __attribute__ ((interrupt));

//;0x00BC  RSPI0_SPII0
void INT_Excep_RSPI0_SPII0(void) __attribute__ ((interrupt));

//;0x00C0  RSPI1_SPEI1
void INT_Excep_RSPI1_SPEI1(void) __attribute__ ((interrupt));

//;0x00C4  RSPI1_SPRI1
void INT_Excep_RSPI1_SPRI1(void) __attribute__ ((interrupt));

//;0x00C8  RSPI1_SPTI1
void INT_Excep_RSPI1_SPTI1(void) __attribute__ ((interrupt));

//;0x00CC  RSPI1_SPII1
void INT_Excep_RSPI1_SPII1(void) __attribute__ ((interrupt));

//;0x00D0  Reserved

//;0x00D4  Reserved

//;0x00D8  Reserved

//;0x00DC  Reserved

//;0x00E0  CAN0_ERS0
void INT_Excep_CAN0_ERS0(void) __attribute__ ((interrupt));

//;0x00E4  CAN0_RXF0
void INT_Excep_CAN0_RXF0(void) __attribute__ ((interrupt));

//;0x00E8  CAN0_TXF0
void INT_Excep_CAN0_TXF0(void) __attribute__ ((interrupt));

//;0x00EC  CAN0_RXM0
void INT_Excep_CAN0_RXM0(void) __attribute__ ((interrupt));

//;0x00F0  CAN0_TXM0
void INT_Excep_CAN0_TXM0(void) __attribute__ ((interrupt));

//;0x00F4  Reserved

//;0x00F8  RTC_PRD
void INT_Excep_RTC_PRD(void) __attribute__ ((interrupt));

//;0x00FC  RTC_CUP
void INT_Excep_RTC_CUP(void) __attribute__ ((interrupt));

//;0x0100  IRQ0
void INT_Excep_ICU_IRQ0(void) __attribute__ ((interrupt));

//;0x0104 IRQ1
void INT_Excep_ICU_IRQ1(void) __attribute__ ((interrupt));

//;0x0108 IRQ2
void INT_Excep_ICU_IRQ2(void) __attribute__ ((interrupt));

//;0x010C IRQ3
void INT_Excep_ICU_IRQ3(void) __attribute__ ((interrupt));

//;0x0110 IRQ4
void INT_Excep_ICU_IRQ4(void) __attribute__ ((interrupt));

//;0x0114 IRQ5
void INT_Excep_ICU_IRQ5(void) __attribute__ ((interrupt));

//;0x0118 IRQ6
void INT_Excep_ICU_IRQ6(void) __attribute__ ((interrupt));

//;0x011C IRQ7
void INT_Excep_ICU_IRQ7(void) __attribute__ ((interrupt));

//;0x0120 IRQ8
void INT_Excep_ICU_IRQ8(void) __attribute__ ((interrupt));

//;0x0124 IRQ9
void INT_Excep_ICU_IRQ9(void) __attribute__ ((interrupt));

//;0x0128 IRQ10
void INT_Excep_ICU_IRQ10(void) __attribute__ ((interrupt));

//;0x012C IRQ11
void INT_Excep_ICU_IRQ11(void) __attribute__ ((interrupt));

//;0x0130 IRQ12
void INT_Excep_ICU_IRQ12(void) __attribute__ ((interrupt));

//;0x0134 IRQ13
void INT_Excep_ICU_IRQ13(void) __attribute__ ((interrupt));

//;0x0138 IRQ14
void INT_Excep_ICU_IRQ14(void) __attribute__ ((interrupt));

//;0x013C IRQ15
void INT_Excep_ICU_IRQ15(void) __attribute__ ((interrupt));

//;0x0140  Reserved

//;0x0144  Reserved

//;0x0148  Reserved

//;0x014C  Reserved

//;0x0150  Reserved

//;0x0154  Reserved

//;0x0158  Reserved

//;0x015C  Reserved

//;0x0160  Reserved

//;0x0164  Reserved

//;0x0168  USB_USBR0
void INT_Excep_USB_USBR0(void) __attribute__ ((interrupt));

//;0x016C  USB_USBR1
void INT_Excep_USB_USBR1(void) __attribute__ ((interrupt));

//;0x0170  RTC_ALM
void INT_Excep_RTC_ALM(void) __attribute__ ((interrupt));

//;0x0174  Reserved

//;0x0178  Reserved

//;0x017C  Reserved

//;0x0180  WDT_WOVI
void INT_Excep_WDT_WOVI(void) __attribute__ ((interrupt));

//;0x0184  Reserved

//;0x0188  AD0_ADI0
void INT_Excep_AD0_ADI0(void) __attribute__ ((interrupt));

//;0x018C  AD1_ADI1
void INT_Excep_AD1_ADI1(void) __attribute__ ((interrupt));

//;0x0190  Reserved

//;0x0194  Reserved

//;0x0198  S12AD_ADI
void INT_Excep_S12AD_ADI(void) __attribute__ ((interrupt));

//;0x019C  Reserved

//;0x01A0  Reserved

//;0x01A4  Reserved

//;0x01A8  Reserved

//;0x01AC  Reserved

//;0x01B0  Reserved

//;0x01B4  Reserved

//;0x01B8  Reserved

//;0x01BC  Reserved

//;0x01C0  Reserved

//;0x01C4  Reserved

//;0x01C8  MTU0_TGIA0
void INT_Excep_MTU0_TGIA0(void) __attribute__ ((interrupt));

//;0x01CC  MTU0_TGIB0
void INT_Excep_MTU0_TGIB0(void) __attribute__ ((interrupt));

//;0x01D0  MTU0_TGIC0
void INT_Excep_MTU0_TGIC0(void) __attribute__ ((interrupt));

//;0x01D4  MTU0_TGID0
void INT_Excep_MTU0_TGID0(void) __attribute__ ((interrupt));

//;0x01D8  MTU0_TCIV0
void INT_Excep_MTU0_TCIV0(void) __attribute__ ((interrupt));

//;0x01DC  MTU0_TGIE0
void INT_Excep_MTU0_TGIE0(void) __attribute__ ((interrupt));

//;0x01E0  MTU0_TGIF0
void INT_Excep_MTU0_TGIF0(void) __attribute__ ((interrupt));

//;0x01E4  MTU1_TGIA1
void INT_Excep_MTU1_TGIA1(void) __attribute__ ((interrupt));

//;0x01E8  MTU1_TGIB1
void INT_Excep_MTU1_TGIB1(void) __attribute__ ((interrupt));

//;0x01EC  MTU1_TCIV1
void INT_Excep_MTU1_TCIV1(void) __attribute__ ((interrupt));

//;0x01F0  MTU1_TCIU1
void INT_Excep_MTU1_TCIU1(void) __attribute__ ((interrupt));

//;0x01F4  MTU2_TGIA2
void INT_Excep_MTU2_TGIA2(void) __attribute__ ((interrupt));

//;0x01F8  MTU2_TGIB2
void INT_Excep_MTU2_TGIB2(void) __attribute__ ((interrupt));

//;0x01FC  MTU2_TCIV2
void INT_Excep_MTU2_TCIV2(void) __attribute__ ((interrupt));

//;0x0200  MTU2_TCIU2
void INT_Excep_MTU2_TCIU2(void) __attribute__ ((interrupt));

//;0x0204  MTU3_TGIA3
void INT_Excep_MTU3_TGIA3(void) __attribute__ ((interrupt));

//;0x0208  MTU3_TGIB3
void INT_Excep_MTU3_TGIB3(void) __attribute__ ((interrupt));

//;0x020C MTU3_TGIC3
void INT_Excep_MTU3_TGIC3(void) __attribute__ ((interrupt));

//;0x0210 MTU3_TGID3
void INT_Excep_MTU3_TGID3(void) __attribute__ ((interrupt));

//;0x0214  MTU3_TCIV3
void INT_Excep_MTU3_TCIV3(void) __attribute__ ((interrupt));

//;0x0218  MTU4_TGIA4
void INT_Excep_MTU4_TGIA4(void) __attribute__ ((interrupt));

//;0x021C  MTU4_TGIB4
void INT_Excep_MTU4_TGIB4(void) __attribute__ ((interrupt));

//;0x0220  MTU4_TGIC4
void INT_Excep_MTU4_TGIC4(void) __attribute__ ((interrupt));

//;0x0224  MTU4_TGID4
void INT_Excep_MTU4_TGID4(void) __attribute__ ((interrupt));

//;0x0228  MTU4_TCIV4
void INT_Excep_MTU4_TCIV4(void) __attribute__ ((interrupt));

//;0x022C  MTU5_TGIU5
void INT_Excep_MTU5_TGIU5(void) __attribute__ ((interrupt));

//;0x0230  MTU5_TGIV5
void INT_Excep_MTU5_TGIV5(void) __attribute__ ((interrupt));

//;0x0234  MTU5_TGIW5
void INT_Excep_MTU5_TGIW5(void) __attribute__ ((interrupt));

//;0x0238  MTU6_TGIA6
void INT_Excep_MTU6_TGIA6(void) __attribute__ ((interrupt));

//;0x023C  MTU6_TGIB6
void INT_Excep_MTU6_TGIB6(void) __attribute__ ((interrupt));

//;0x0240  MTU6_TGIC6
void INT_Excep_MTU6_TGIC6(void) __attribute__ ((interrupt));

//;0x0244  MTU6_TGID6
void INT_Excep_MTU6_TGID6(void) __attribute__ ((interrupt));

//;0x0248  MTU6_TCIV6
void INT_Excep_MTU6_TCIV6(void) __attribute__ ((interrupt));

//;0x024C  MTU6_TGIE6
void INT_Excep_MTU6_TGIE6(void) __attribute__ ((interrupt));

//;0x0250  MTU6_TGIF6
void INT_Excep_MTU6_TGIF6(void) __attribute__ ((interrupt));

//;0x0254  MTU7_TGIA7
void INT_Excep_MTU7_TGIA7(void) __attribute__ ((interrupt));

//;0x0258  MTU7_TGIB7
void INT_Excep_MTU7_TGIB7(void) __attribute__ ((interrupt));

//;0x025C  MTU7_TCIV7
void INT_Excep_MTU7_TCIV7(void) __attribute__ ((interrupt));

//;0x0260  MTU7_TCIU7
void INT_Excep_MTU7_TCIU7(void) __attribute__ ((interrupt));

//;0x0264  MTU8_TGIA8
void INT_Excep_MTU8_TGIA8(void) __attribute__ ((interrupt));

//;0x0268  MTU8_TGIB8
void INT_Excep_MTU8_TGIB8(void) __attribute__ ((interrupt));

//;0x026C  MTU8_TCIV8
void INT_Excep_MTU8_TCIV8(void) __attribute__ ((interrupt));

//;0x0270  MTU8_TCIU8
void INT_Excep_MTU8_TCIU8(void) __attribute__ ((interrupt));

//;0x0274  MTU9_TGIA9
void INT_Excep_MTU9_TGIA9(void) __attribute__ ((interrupt));

//;0x0278  MTU9_TGIB9
void INT_Excep_MTU9_TGIB9(void) __attribute__ ((interrupt));

//;0x027C  MTU9_TGIC9
void INT_Excep_MTU9_TGIC9(void) __attribute__ ((interrupt));

//;0x0280  MTU9_TGID9
void INT_Excep_MTU9_TGID9(void) __attribute__ ((interrupt));

//;0x0284  MTU9_TCIV9
void INT_Excep_MTU9_TCIV9(void) __attribute__ ((interrupt));

//;0x0288  MTU10_TGIA10
void INT_Excep_MTU10_TGIA10(void) __attribute__ ((interrupt));

//;0x028C  MTU10_TGIB10
void INT_Excep_MTU10_TGIB10(void) __attribute__ ((interrupt));

//;0x0290  MTU10_TGIC10
void INT_Excep_MTU10_TGIC10(void) __attribute__ ((interrupt));

//;0x0294  MTU10_TGID10
void INT_Excep_MTU10_TGID10(void) __attribute__ ((interrupt));

//;0x0298  MTU10_TCIV10
void INT_Excep_MTU10_TCIV10(void) __attribute__ ((interrupt));

//;0x029C  MTU11_TGIU11
void INT_Excep_MTU11_TGIU11(void) __attribute__ ((interrupt));

//;0x02A0  MTU11_TGIV11
void INT_Excep_MTU11_TGIV11(void) __attribute__ ((interrupt));

//;0x02A4  MTU11_TGIW11
void INT_Excep_MTU11_TGIW11(void) __attribute__ ((interrupt));

//;0x02A8  POE_OEI1
void INT_Excep_POE_OEI1(void) __attribute__ ((interrupt));

//;0x02AC  POE_OEI2
void INT_Excep_POE_OEI2(void) __attribute__ ((interrupt));

//;0x02B0  POE_OEI3
void INT_Excep_POE_OEI3(void) __attribute__ ((interrupt));

//;0x02B4  POE_OEI4
void INT_Excep_POE_OEI4(void) __attribute__ ((interrupt));

//;0x02B8  TMR0_CMI0A
void INT_Excep_TMR0_CMIA0(void) __attribute__ ((interrupt));

//;0x02BC  TMR0_CMI0B
void INT_Excep_TMR0_CMIB0(void) __attribute__ ((interrupt));

//;0x02C0  TMR0_OV0I
void INT_Excep_TMR0_OVI0(void) __attribute__ ((interrupt));

//;0x02C4  TMR1_CMI1A
void INT_Excep_TMR1_CMIA1(void) __attribute__ ((interrupt));

//;0x02C8  TMR1_CMI1B
void INT_Excep_TMR1_CMIB1(void) __attribute__ ((interrupt));

//;0x02CC  TMR1_OV1I
void INT_Excep_TMR1_OVI1(void) __attribute__ ((interrupt));

//;0x02D0 TMR2_CMI2A
void INT_Excep_TMR2_CMIA2(void) __attribute__ ((interrupt));

//;0x02D4  TMR2_CMI2B
void INT_Excep_TMR2_CMIB2(void) __attribute__ ((interrupt));

//;0x02D8  TMR2_OV2I
void INT_Excep_TMR2_OVI2(void) __attribute__ ((interrupt));

//;0x02DC  TMR3_CMI3A
void INT_Excep_TMR3_CMIA3(void) __attribute__ ((interrupt));

//;0x02E0 TMR3_CMI3B
void INT_Excep_TMR3_CMIB3(void) __attribute__ ((interrupt));

//;0x02E4  TMR3_OV3I
void INT_Excep_TMR3_OVI3(void) __attribute__ ((interrupt));

//;0x02E8  Reserved

//;0x02EC  Reserved

//;0x02F0  Reserved

//;0x02F4  Reserved

//;0x02F8  Reserved

//;0x02FC  Reserved

//;0x0300  Reserved

//;0x0304  Reserved

//;0x0308  Reserved

//;0x030C  Reserved

//;0x0310  Reserved

//;0x0314  Reserved

//;0x0318  DMAC_DMTEND0
void INT_Excep_DMAC_DMAC0I(void) __attribute__ ((interrupt));

//;0x031C  DMAC_DMTEND1
void INT_Excep_DMAC_DMAC1I(void) __attribute__ ((interrupt));

//;0x0320  DMAC_DMTEND2
void INT_Excep_DMAC_DMAC2I(void) __attribute__ ((interrupt));

//;0x0324  DMAC_DMTEND3
void INT_Excep_DMAC_DMAC3I(void) __attribute__ ((interrupt));

//;0x0328  Reserved
void INT_Excep_EXDMAC_EXDMAC0I(void) __attribute__ ((interrupt));

//;0x032C  Reserved
void INT_Excep_EXDMAC_EXDMAC1I(void) __attribute__ ((interrupt));

//;0x0330  Reserved

//;0x0334  Reserved

//;0x0338  Reserved

//;0x033C  Reserved

//;0x0340  Reserved

//;0x0344  Reserved

//;0x0348  Reserved

//;0x034C  Reserved

//;0x0350  Reserved

//;0x0354  Reserved

//;0x0358  SCI0_ERI0
void INT_Excep_SCI0_ERI0(void) __attribute__ ((interrupt));

//;0x035C  SCI0_RXI0
void INT_Excep_SCI0_RXI0(void) __attribute__ ((interrupt));

//;0x0360  SCI0_TXI0
void INT_Excep_SCI0_TXI0(void) __attribute__ ((interrupt));

//;0x0364  SCI0_TEI0
void INT_Excep_SCI0_TEI0(void) __attribute__ ((interrupt));

//;0x0368  SCI1_ERI1
void INT_Excep_SCI1_ERI1(void) __attribute__ ((interrupt));

//;0x036C  SCI1_RXI1
void INT_Excep_SCI1_RXI1(void) __attribute__ ((interrupt));

//;0x0370  SCI1_TXI1
void INT_Excep_SCI1_TXI1(void) __attribute__ ((interrupt));

//;0x0374  SCI1_TEI1
void INT_Excep_SCI1_TEI1(void) __attribute__ ((interrupt));

//;0x0378  SCI2_ERI2
void INT_Excep_SCI2_ERI2(void) __attribute__ ((interrupt));

//;0x037C  SCI2_RXI2
void INT_Excep_SCI2_RXI2(void) __attribute__ ((interrupt));

//;0x0380  SCI2_TXI2
void INT_Excep_SCI2_TXI2(void) __attribute__ ((interrupt));

//;0x0384  SCI2_TEI2
void INT_Excep_SCI2_TEI2(void) __attribute__ ((interrupt));

//;0x0388  SCI3_ERI3
void INT_Excep_SCI3_ERI3(void) __attribute__ ((interrupt));

//;0x038C  SCI3_RXI3
void INT_Excep_SCI3_RXI3(void) __attribute__ ((interrupt));

//;0x0390  SCI3_TXI3
void INT_Excep_SCI3_TXI3(void) __attribute__ ((interrupt));

//;0x0394  SCI3_TEI3
void INT_Excep_SCI3_TEI3(void) __attribute__ ((interrupt));

//;0x0398  Reserved

//;0x039C  Reserved

//;0x03A0  Reserved

//;0x03A4  Reserved

//;0x03A8  SCI5_ERI5
void INT_Excep_SCI5_ERI5(void) __attribute__ ((interrupt));

//;0x03AC  SCI5_RXI5
void INT_Excep_SCI5_RXI5(void) __attribute__ ((interrupt));

//;0x03B0  SCI5_TXI5
void INT_Excep_SCI5_TXI5(void) __attribute__ ((interrupt));

//;0x03B4  SCI5_TEI5
void INT_Excep_SCI5_TEI5(void) __attribute__ ((interrupt));

//;0x03B8  SCI6_ERI6
void INT_Excep_SCI6_ERI6(void) __attribute__ ((interrupt));

//;0x03BC  SCI6_RXI6
void INT_Excep_SCI6_RXI6(void) __attribute__ ((interrupt));

//;0x03C0  SCI6_TXI6
void INT_Excep_SCI6_TXI6(void) __attribute__ ((interrupt));

//;0x03C4  SCI6_TEI6
void INT_Excep_SCI6_TEI6(void) __attribute__ ((interrupt));

//;0x03C8  Reserved

//;0x03CC  Reserved

//;0x03D0  Reserved

//;0x03D4  Reserved

//;0x03D8  RIIC0_ICEEI0
void INT_Excep_RIIC0_ICEEI0(void) __attribute__ ((interrupt));

//;0x03DC  RIIC0_ICRXI0
void INT_Excep_RIIC0_ICRXI0(void) __attribute__ ((interrupt));

//;0x03E0  RIIC0_ICTXI0
void INT_Excep_RIIC0_ICTXI0(void) __attribute__ ((interrupt));

//;0x03E4  RIIC0_ICTEI0
void INT_Excep_RIIC0_ICTEI0(void) __attribute__ ((interrupt));

//;0x03E8  RIIC1_ICEEI1
void INT_Excep_RIIC1_ICEEI1(void) __attribute__ ((interrupt));

//;0x03EC  RIIC1_ICRXI1
void INT_Excep_RIIC1_ICRXI1(void) __attribute__ ((interrupt));

//;0x03F0  RIIC1_ICTXI1
void INT_Excep_RIIC1_ICTXI1(void) __attribute__ ((interrupt));

//;0x03F4  RIIC1_ICTEI1
void INT_Excep_RIIC1_ICTEI1(void) __attribute__ ((interrupt));

//;0x03F8  Reserved

//;0x03FC  Reserved

extern void PowerON_Reset(void) __attribute__ ((interrupt));

//;<<VECTOR DATA END (POWER ON RESET)>>

#endif
