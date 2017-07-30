/***************************************************************/
/*                                                             */
/*      PROJECT NAME :  sh2alib                                */
/*      FILE         :  interrupt_handlers.c                   */
/*      DESCRIPTION  :  Interrupt Handler                      */
/*      CPU SERIES   :  SH2a                                   */
/*      CPU TYPE     :  SH7262                                 */
/*                                                             */
/*      This file is generated by e2 studio.                   */
/*                                                             */
/***************************************************************/

/************************************************************************/
/*    File Version: V1.00                                               */
/*    Date Generated: 08/07/2013                                        */
/************************************************************************/

#include "interrupt_handlers.h"

#define __WEAK  __attribute__((weak))

// 2 Manual Reset PC
void __WEAK INT_Manual_Reset_PC(void)
{
}
// 3 Manual Reset SP
void __WEAK INT_Manual_Reset_SP(void)
{
}
// 4 Illegal code
void __WEAK INT_Illegal_code(void)
{
}
// 5 Reserved

// 6 Illegal slot
void __WEAK INT_Illegal_slot(void)
{
}
// 7 Reserved

// 8 Reserved

// 9 CPU Address error
void __WEAK INT_CPU_Address(void)
{
}
// 10 BUS error
void __WEAK INT_DMAC_Address(void)
{
}
// 11 NMI
void __WEAK INT_NMI(void)
{
}
// 12 Reserved

// 13 INT_FPU
void __WEAK INT_FPU(void)
{
}
// 14 H-UDI
void __WEAK INT_HUDI(void)
{
}
// 15 Bank Overflow
void __WEAK INT_Bank_Overflow(void)
{
}
// 16 Bank Underflow
void __WEAK INT_Bank_Underflow(void)
{
}
// 17 Divide by zero
void __WEAK INT_Divide_by_Zero(void)
{
}
// 18 Divide Overflow
void __WEAK INT_Divide_Overflow(void)
{
}
// 19 Reserved

// 20 Reserved

// 21 Reserved

// 22 Reserved

// 23 Reserved

// 24 Reserved

// 25 Reserved

// 26 Reserved

// 27 Reserved

// 28 Reserved

// 29 Reserved

// 30 Reserved

// 31 Reserved

// 32 TRAPA (User Vecter)
void __WEAK INT_TRAPA32(void)
{
}
// 33 TRAPA (User Vecter)
void __WEAK INT_TRAPA33(void)
{
}
// 34 TRAPA (User Vecter)
void __WEAK INT_TRAPA34(void)
{
}
// 35 TRAPA (User Vecter)
void __WEAK INT_TRAPA35(void)
{
}
// 36 TRAPA (User Vecter)
void __WEAK INT_TRAPA36(void)
{
}
// 37 TRAPA (User Vecter)
void __WEAK INT_TRAPA37(void)
{
}
// 38 TRAPA (User Vecter)
void __WEAK INT_TRAPA38(void)
{
}
// 39 TRAPA (User Vecter)
void __WEAK INT_TRAPA39(void)
{
}
// 40 TRAPA (User Vecter)
void __WEAK INT_TRAPA40(void)
{
}
// 41 TRAPA (User Vecter)
void __WEAK INT_TRAPA41(void)
{
}
// 42 TRAPA (User Vecter)
void __WEAK INT_TRAPA42(void)
{
}
// 43 TRAPA (User Vecter)
void __WEAK INT_TRAPA43(void)
{
}
// 44 TRAPA (User Vecter)
void __WEAK INT_TRAPA44(void)
{
}
// 45 TRAPA (User Vecter)
void __WEAK INT_TRAPA45(void)
{
}
// 46 TRAPA (User Vecter)
void __WEAK INT_TRAPA46(void)
{
}
// 47 TRAPA (User Vecter)
void __WEAK INT_TRAPA47(void)
{
}
// 48 TRAPA (User Vecter)
void __WEAK INT_TRAPA48(void)
{
}
// 49 TRAPA (User Vecter)
void __WEAK INT_TRAPA49(void)
{
}
// 50 TRAPA (User Vecter)
void __WEAK INT_TRAPA50(void)
{
}
// 51 TRAPA (User Vecter)
void __WEAK INT_TRAPA51(void)
{
}
// 52 TRAPA (User Vecter)
void __WEAK INT_TRAPA52(void)
{
}
// 53 TRAPA (User Vecter)
void __WEAK INT_TRAPA53(void)
{
}
// 54 TRAPA (User Vecter)
void __WEAK INT_TRAPA54(void)
{
}
// 55 TRAPA (User Vecter)
void __WEAK INT_TRAPA55(void)
{
}
// 56 TRAPA (User Vecter)
void __WEAK INT_TRAPA56(void)
{
}
// 57 TRAPA (User Vecter)
void __WEAK INT_TRAPA57(void)
{
}
// 58 TRAPA (User Vecter)
void __WEAK INT_TRAPA58(void)
{
}
// 59 TRAPA (User Vecter)
void __WEAK INT_TRAPA59(void)
{
}
// 60 TRAPA (User Vecter)
void __WEAK INT_TRAPA60(void)
{
}
// 61 TRAPA (User Vecter)
void __WEAK INT_TRAPA61(void)
{
}
// 62 TRAPA (User Vecter)
void __WEAK INT_TRAPA62(void)
{
}
// 63 TRAPA (User Vecter)
void __WEAK INT_TRAPA63(void)
{
}
// 64 Interrupt IRQ0
void __WEAK INT_IRQ_IRQ0(void)
{
}
// 65 Interrupt IRQ1
void __WEAK INT_IRQ_IRQ1(void)
{
}
// 66 Interrupt IRQ2
void __WEAK INT_IRQ_IRQ2(void)
{
}
// 67 Interrupt IRQ3
void __WEAK INT_IRQ_IRQ3(void)
{
}
// 68 Interrupt IRQ4
void __WEAK INT_IRQ_IRQ4(void)
{
}
// 69 Interrupt IRQ5
void __WEAK INT_IRQ_IRQ5(void)
{
}
// 70 Interrupt IRQ6
void __WEAK INT_IRQ_IRQ6(void)
{
}
// 71 Interrupt IRQ7
void __WEAK INT_IRQ_IRQ7(void)
{
}
// 72 Reserved

// 73 Reserved

// 74 Reserved

// 75 Reserved

// 76 Reserved

// 77 Reserved

// 78 Reserved

// 79 Reserved

// 80 Interrupt PINT0
void __WEAK INT_PINT_PINT0(void)
{
}

// 81 Interrupt PINT1
void __WEAK INT_PINT_PINT1(void)
{
}

// 82 Interrupt PINT2
void __WEAK INT_PINT_PINT2(void)
{
}

// 83 Interrupt PINT3
void __WEAK INT_PINT_PINT3(void)
{
}

// 84 Interrupt PINT4
void __WEAK INT_PINT_PINT4(void)
{
}

// 85 Interrupt PINT5
void __WEAK INT_PINT_PINT5(void)
{
}

// 86 Interrupt PINT6
void __WEAK INT_PINT_PINT6(void)
{
}

// 87 Interrupt PINT7
void __WEAK INT_PINT_PINT7(void)
{
}
// 88 Reserved

// 89 Reserved

// 90 Reserved

// 91 Reserved

// 92 Reserved

// 93 Reserved

// 94 Reserved

// 95 Reserved

// 96 Reserved

// 97 Reserved

// 98 Reserved

// 99 Reserved

// 100 Reserved

// 101 Reserved

// 102 Reserved

// 103 Reserved

// 104 Reserved

// 105 Reserved

// 106 Reserved

// 107 Reserved

// 108 DMAC0 DEI0
void __WEAK INT_DMAC0_DEI0(void)
{
}
// 109 DMAC0 HEI0
void __WEAK INT_DMAC0_HEI0(void)
{
}
// 110 Reserved

// 111 Reserved

// 112 DMAC1 DEI1
void __WEAK INT_DMAC1_DEI1(void)
{
}
// 113 DMAC1 HEI1
void __WEAK INT_DMAC1_HEI1(void)
{
}
// 114 Reserved

// 115 Reserved

// 116 DMAC2 DEI2
void __WEAK INT_DMAC2_DEI2(void)
{
}
// 117 DMAC2 HEI2
void __WEAK INT_DMAC2_HEI2(void)
{
}
// 118 Reserved

// 119 Reserved

// 120 DMAC3 DEI3
void __WEAK INT_DMAC3_DEI3(void)
{
}
// 121 DMAC3 HEI3
void __WEAK INT_DMAC3_HEI3(void)
{
}
// 122 Reserved

// 123 Reserved

// 124 DMAC4 DEI4
void __WEAK INT_DMAC4_DEI4(void)
{
}
// 125 DMAC4 HEI4
void __WEAK INT_DMAC4_HEI4(void)
{
}
// 126 Reserved

// 127 Reserved

// 128 DMAC5 DEI5
void __WEAK INT_DMAC5_DEI5(void)
{
}
// 129 DMAC5 HEI5
void __WEAK INT_DMAC5_HEI5(void)
{
}
// 130 Reserved

// 131 Reserved

// 132 DMAC6 DEI6
void __WEAK INT_DMAC6_DEI6(void)
{
}
// 133 DMAC6 HEI6
void __WEAK INT_DMAC6_HEI6(void)
{
}
// 134 Reserved

// 135 Reserved

// 136 DMAC7 DEI7
void __WEAK INT_DMAC7_DEI7(void)
{
}
// 137 DMAC7 HEI7
void __WEAK INT_DMAC7_HEI7(void)
{
}
// 138 Reserved

// 139 Reserved

// 140 DMAC8 DEI8
void __WEAK INT_DMAC8_DEI8(void)
{
}
// 141 DMAC8 HEI8
void __WEAK INT_DMAC8_HEI8(void)
{
}
// 142 Reserved

// 143 Reserved

// 144 DMAC9 DEI9
void __WEAK INT_DMAC9_DEI9(void)
{
}
// 145 DMAC9 HEI9
void __WEAK INT_DMAC9_HEI9(void)
{
}
// 146 Reserved

// 147 Reserved

// 148 DMAC10 DEI10
void __WEAK INT_DMAC10_DEI10(void)
{
}
// 149 DMAC10 HEI10
void __WEAK INT_DMAC10_HEI10(void)
{
}
// 150 Reserved

// 151 Reserved

// 152 DMAC11 DEI11
void __WEAK INT_DMAC11_DEI11(void)
{
}
// 153 DMAC11 HEI11
void __WEAK INT_DMAC11_HEI11(void)
{
}
// 154 Reserved

// 155 Reserved

// 156 DMAC12 DEI12
void __WEAK INT_DMAC12_DEI12(void)
{
}
// 157 DMAC12 HEI12
void __WEAK INT_DMAC12_HEI12(void)
{
}
// 158 Reserved

// 159 Reserved

// 160 DMAC13 DEI13
void __WEAK INT_DMAC13_DEI13(void)
{
}
// 161 DMAC13 HEI13
void __WEAK INT_DMAC13_HEI13(void)
{
}
// 162 Reserved

// 163 Reserved

// 164 DMAC14 DEI14
void __WEAK INT_DMAC14_DEI14(void)
{
}
// 165 DMAC14 HEI14
void __WEAK INT_DMAC14_HEI14(void)
{
}
// 166 Reserved

// 167 Reserved

// 168 DMAC15 DEI15
void __WEAK INT_DMAC15_DEI15(void)
{
}
// 169 DMAC15 HEI15
void __WEAK INT_DMAC15_HEI15(void)
{
}
// 170 USB USBI
void __WEAK INT_USB_USBI(void)
{
}
// 171 VDC3 VIVSYNCJ
void __WEAK INT_VDC3_VIVSYNCJ(void)
{
}
// 172 VDC3 VBUFERRJ
void __WEAK INT_VDC3_VBUFERR(void)
{
}
// 173 VDC3 VIFIELDE
void __WEAK INT_VDC3_VIFIELDE(void)
{
}
// 174 VDC3 VOLINE
void __WEAK INT_VDC3_VOLINE(void)
{
}
// 175 CMT CMT0 CMI0
void __WEAK INT_CMT0_CMI0(void)
{
}
// 176 CMT CMT1 CMI1
void __WEAK INT_CMT1_CMI1(void)
{
}
// 177 BSC CMI
void __WEAK INT_BSC_CMI(void)
{
}
// 178 WDT
void __WEAK INT_WDT_ITI(void)
{
}
// 179 MTU2 MTU0 TGI0A
void __WEAK INT_MTU2_TGI0A(void)
{
}
// 180 MTU2 MTU0 TGI0B
void __WEAK INT_MTU2_TGI0B(void)
{
}
// 181 MTU2 MTU0 TGI0C
void __WEAK INT_MTU2_TGI0C(void)
{
}
// 182 MTU2 MTU0 TGI0D
void __WEAK INT_MTU2_TGI0D(void)
{
}
// 183 MTU2 MTU0 TCI0V
void __WEAK INT_MTU2_TCI0V(void)
{
}
// 184 MTU2 MTU0 TGI0E
void __WEAK INT_MTU2_TGI0E(void)
{
}
// 185 MTU2 MTU0 TGI0F
void __WEAK INT_MTU2_TGI0F(void)
{
}
// 186 MTU2 MTU1 TGI1A
void __WEAK INT_MTU2_TGI1A(void)
{
}
// 187 MTU2 MTU1 TGI1B
void __WEAK INT_MTU2_TGI1B(void)
{
}
// 188 MTU2 MTU1 TCI1V
void __WEAK INT_MTU2_TCI1V(void)
{
}
// 189 MTU2 MTU1 TCI1U
void __WEAK INT_MTU2_TCI1U(void)
{
}
// 190 MTU2 MTU2 TGI2A
void __WEAK INT_MTU2_TGI2A(void)
{
}
// 191 MTU2 MTU2 TGI2B
void __WEAK INT_MTU2_TGI2B(void)
{
}
// 192 MTU2 MTU2 TCI2V
void __WEAK INT_MTU2_TCI2V(void)
{
}
// 193 MTU2 MTU2 TCI2U
void __WEAK INT_MTU2_TCI2U(void)
{
}
// 194 MTU2 MTU3 TGI3A
void __WEAK INT_MTU2_TGI3A(void)
{
}
// 195 MTU2 MTU3 TGI3B
void __WEAK INT_MTU2_TGI3B(void)
{
}
// 196 MTU2 MTU3 TGI3C
void __WEAK INT_MTU2_TGI3C(void)
{
}

// 197 MTU2 MTU3 TGI3D
void __WEAK INT_MTU2_TGI3D(void)
{
}
// 198 MTU2 MTU3 TCI3V
void __WEAK INT_MTU2_TCI3V(void)
{
}
// 199 MTU2 MTU4 TGI4A
void __WEAK INT_MTU2_TGI4A(void)
{
}
// 200 MTU2 MTU4 TGI4B
void __WEAK INT_MTU2_TGI4B(void)
{
}
// 201 MTU2 MTU4 TGI4C
void __WEAK INT_MTU2_TGI4C(void)
{
}
// 202 MTU2 MTU4 TGI4D
void __WEAK INT_MTU2_TGI4D(void)
{
}
// 203 MTU2 MTU4 TCI4V
void __WEAK INT_MTU2_TCI4V(void)
{
}
// 204 PWM PWM1
void __WEAK INT_PWM1(void)
{
}
// 205 PWM PWM2
void __WEAK INT_PWM2(void)
{
}
// 206 A/D ADI
void __WEAK INT_AD_ADI(void)
{
}
// 207 SSI0 SSIF0
void __WEAK INT_SSI0_SSIF0(void)
{
}
// 208 SSI0 SSIRXI0
void __WEAK INT_SSI0_SSIRXI0(void)
{
}
// 209 SSI0 SSITXI0
void __WEAK INT_SSI0_SSITXI0(void)
{
}
// 210 SSI1 SSII1
void __WEAK INT_SSI1_SSII1(void)
{
}
// 211 SSI1 SSIRTI1
void __WEAK INT_SSI1_SSIRTI1(void)
{
}
// 212 SSI2 SSII2
void __WEAK INT_SSI2_SSII2(void)
{
}
// 213 SSI2 SSIRTI2
void __WEAK INT_SSI2_SSIRTI2(void)
{
}
// 214 SSI3 SSII3
void __WEAK INT_SSI3_SSII3(void)
{
}
// 215 SSI3 SSIRTI3
void __WEAK INT_SSI3_SSIRTI3(void)
{
}
// 216 SPDIF SPDIFI
void __WEAK INT_SPDIF_SPDIFI(void)
{
}
// 217 IIC3 IIC0 STPI0
void __WEAK INT_IIC3_STPI0(void)
{
}
// 218 IIC3 IIC0 NAKI0
void __WEAK INT_IIC3_NAKI0(void)
{
}
// 219 IIC3 IIC0 RXI0
void __WEAK INT_IIC3_RXI0(void)
{
}
// 220 IIC3 IIC0 TXI0
void __WEAK INT_IIC3_TXI0(void)
{
}
// 221 IIC3 IIC0 TEI0
void __WEAK INT_IIC3_TEI0(void)
{
}
// 222 IIC3 IIC1 STPI1
void __WEAK INT_IIC3_STPI1(void)
{
}
// 223 IIC3 IIC1 NAKI1
void __WEAK INT_IIC3_NAKI1(void)
{
}
// 224 IIC3 IIC1 RXI1
void __WEAK INT_IIC3_RXI1(void)
{
}
// 225 IIC3 IIC1 TXI1
void __WEAK INT_IIC3_TXI1(void)
{
}
// 226 IIC3 IIC1 TEI1
void __WEAK INT_IIC3_TEI1(void)
{
}
// 227 IIC3 IIC2 STPI2
void __WEAK INT_IIC3_STPI2(void)
{
}
// 228 IIC3 IIC2 NAKI2
void __WEAK INT_IIC3_NAKI2(void)
{
}
// 229 IIC3 IIC2 RXI2
void __WEAK INT_IIC3_RXI2(void)
{
}
// 230 IIC3 IIC2 TXI2
void __WEAK INT_IIC3_TXI2(void)
{
}
// 231 IIC3 IIC2 TEI2
void __WEAK INT_IIC3_TEI2(void)
{
}
// 232 SCIF SCIF0 BRI0
void __WEAK INT_SCIF0_BRI0(void)
{
}
// 233 SCIF SCIF0 ERI0
void __WEAK INT_SCIF0_ERI0(void)
{
}
// 234 SCIF SCIF0 RXI0
void __WEAK INT_SCIF0_RXI0(void)
{
}
// 235 SCIF SCIF0 TXI0
void __WEAK INT_SCIF0_TXI0(void)
{
}
// 236 SCIF SCIF1 BRI1
void __WEAK INT_SCIF1_BRI1(void)
{
}
// 237 SCIF SCIF1 ERI1
void __WEAK INT_SCIF1_ERI1(void)
{
}
// 238 SCIF SCIF1 RXI1
void __WEAK INT_SCIF1_RXI1(void)
{
}
// 239 SCIF SCIF1 TXI1
void __WEAK INT_SCIF1_TXI1(void)
{
}
// 240 SCIF SCIF2 BRI2
void __WEAK INT_SCIF2_BRI2(void)
{
}
// 241 SCIF SCIF2 ERI2
void __WEAK INT_SCIF2_ERI2(void)
{
}
// 242 SCIF SCIF2 RXI2
void __WEAK INT_SCIF2_RXI2(void)
{
}
// 243 SCIF SCIF2 TXI2
void __WEAK INT_SCIF2_TXI2(void)
{
}
// 244 SCIF SCIF3 BRI3
void __WEAK INT_SCIF3_BRI3(void)
{
}
// 245 SCIF SCIF3 ERI3
void __WEAK INT_SCIF3_ERI3(void)
{
}
// 246 SCIF SCIF3 RXI3
void __WEAK INT_SCIF3_RXI3(void)
{
}
// 247 SCIF SCIF3 TXI3
void __WEAK INT_SCIF3_TXI3(void)
{
}
// 248 SCIF SCIF4 BRI4
void __WEAK INT_SCIF4_BRI4(void)
{
}
// 249 SCIF SCIF4 ERI4
void __WEAK INT_SCIF4_ERI4(void)
{
}
// 250 SCIF SCIF4 RXI4
void __WEAK INT_SCIF4_RXI4(void)
{
}
// 251 SCIF SCIF4 TXI4
void __WEAK INT_SCIF4_TXI4(void)
{
}
// 252 SCIF SCIF5 BRI5
void __WEAK INT_SCIF5_BRI5(void)
{
}
// 253 SCIF SCIF5 ERI5
void __WEAK INT_SCIF5_ERI5(void)
{
}
// 254 SCIF SCIF5 RXI5
void __WEAK INT_SCIF5_RXI5(void)
{
}
// 255 SCIF SCIF5 TXI5
void __WEAK INT_SCIF5_TXI5(void)
{
}
// 256 SCIF SCIF6 BRI6
void __WEAK INT_SCIF6_BRI6(void)
{
}
// 257 SCIF SCIF6 ERI6
void __WEAK INT_SCIF6_ERI6(void)
{
}
// 258 SCIF SCIF6 RXI6
void __WEAK INT_SCIF6_RXI6(void)
{
}
// 259 SCIF SCIF6 TXI6
void __WEAK INT_SCIF6_TXI6(void)
{
}
// 260 SCIF SCIF7 BRI7
void __WEAK INT_SCIF7_BRI7(void)
{
}
// 261 SCIF SCIF7 ERI7
void __WEAK INT_SCIF7_ERI7(void)
{
}
// 262 SCIF SCIF7 RXI7
void __WEAK INT_SCIF7_RXI7(void)
{
}
// 263 SCIF SCIF7 TXI7
void __WEAK INT_SCIF7_TXI7(void)
{
}
// 264 SIOF SIOFI
void __WEAK INT_SIOF_SIOFI(void)
{
}
// 265 RSPI RSPI0 SPEI0
void __WEAK INT_RSPI0_SPEI0(void)
{
}
// 266 RSPI RSPI0 SPRI0
void __WEAK INT_RSPI0_SPRI0(void)
{
}
// 267 RSPI RSPI0 SPTI0
void __WEAK INT_RSPI0_SPTI0(void)
{
}
// 268 RSPI RSPI1 SPEI1
void __WEAK INT_RSPI1_SPEI1(void)
{
}
// 269 RSPI RSPI1 SPRI1
void __WEAK INT_RSPI1_SPRI1(void)
{
}
// 270 RSPI RSPI1 SPTI1
void __WEAK INT_RSPI1_SPTI1(void)
{
}
// 271 RCAN RCAN0 ERS0
void __WEAK INT_RCAN0_ERS0(void)
{
}
// 272 RCAN RCAN0 OVR0
void __WEAK INT_RCAN0_OVR0(void)
{
}
// 273 RCAN RCAN0 RM00
void __WEAK INT_RCAN0_RM00(void)
{
}
// 274 RCAN RCAN0 RM10
void __WEAK INT_RCAN0_RM10(void)
{
}
// 275 RCAN RCAN0 SLE0
void __WEAK INT_RCAN0_SLE0(void)
{
}
// 276 RCAN RCAN1 ERS1
void __WEAK INT_RCAN1_ERS1(void)
{
}
// 277 RCAN RCAN1 OVR1
void __WEAK INT_RCAN1_OVR1(void)
{
}
// 278 RCAN RCAN1 RM01
void __WEAK INT_RCAN1_RM01(void)
{
}
// 279 RCAN RCAN1 RM11
void __WEAK INT_RCAN1_RM11(void)
{
}
// 280 RCAN RCAN1 SLE1
void __WEAK INT_RCAN1_SLE1(void)
{
}
// 281 IEB IEB
void __WEAK INT_IEB_IEB(void)
{
}
// 282 ROMDEC ISY
void __WEAK INT_ROMDEC_ISY(void)
{
}
// 283 ROMDEC IERR
void __WEAK INT_ROMDEC_IERR(void)
{
}
// 284 ROMDEC ITARG
void __WEAK INT_ROMDEC_ITARG(void)
{
}
// 285 ROMDEC ISEC
void __WEAK INT_ROMDEC_ISEC(void)
{
}
// 286 ROMDEC IBUF
void __WEAK INT_ROMDEC_IBUF(void)
{
}
// 287 ROMDEC IREADY
void __WEAK INT_ROMDEC_IREADY(void)
{
}
// 288 FLCTL FLSTEI
void __WEAK INT_FLCTL_FLSTEI(void)
{
}
// 289 FLCTL FLTENDI
void __WEAK INT_FLCTL_FLTENDI(void)
{
}
// 290 FLCTL FLTREQ0I
void __WEAK INT_FLCTL_FLTREQ0I(void)
{
}
// 291 FLCTL FLTREQ1I
void __WEAK INT_FLCTL_FLTREQ1I(void)
{
}
// 292 SDHI SDHI3
void __WEAK INT_SDHI_SDHI3(void)
{
}
// 293 SDHI SDHI0
void __WEAK INT_SDHI_SDHI0(void)
{
}
// 294 SDHI SDHI1
void __WEAK INT_SDHI_SDHI1(void)
{
}
// 295 Reserved

// 296 RTC ARM
void __WEAK INT_RTC_ARM(void)
{
}
// 297 RTC PRD
void __WEAK INT_RTC_PRD(void)
{
}
// 298 RTC CUP
void __WEAK INT_RTC_CUP(void)
{
}
// 299 SRC SRC0 OVF0
void __WEAK INT_SRC0_OVF0(void)
{
}
// 300 SRC SRC0 UDF0
void __WEAK INT_SRC0_UDF0(void)
{
}
// 301 SRC SRC0 CEF0
void __WEAK INT_SRC0_CEF0(void)
{
}
// 302 SRC SRC0 ODFI0
void __WEAK INT_SRC0_ODFI0(void)
{
}
// 303 SRC SRC0 IDEI0
void __WEAK INT_SRC0_IDEI0(void)
{
}
// 304 SRC SRC1 OVF1
void __WEAK INT_SRC1_OVF1(void)
{
}
// 305 SRC SRC1 UDF1
void __WEAK INT_SRC1_UDF1(void)
{
}
// 306 SRC SRC1 CEF1
void __WEAK INT_SRC1_CEF1(void)
{
}
// 307 SRC SRC1 ODFI1
void __WEAK INT_SRC1_ODFI1(void)
{
}
// 308 SRC SRC1 IDEI1
void __WEAK INT_SRC1_IDEI1(void)
{
}
// 309 Reserved

// 310 DCU DCUEI
void __WEAK INT_DCU_DCUEI(void)
{
}
// 311 DCU OFFI
void __WEAK INT_DCU_OFFI(void)
{
}
// 312 DCU IFEI
void __WEAK INT_DCU_IFEI(void)
{
}

void __WEAK Dummy(void)
{
}

/* End of File */