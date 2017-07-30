/***************************************************************/
/*                                                             */
/*      PROJECT NAME :  sh2alib                                */
/*      FILE         :  vector_table.c                         */
/*      DESCRIPTION  :  Vector Table                           */
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

typedef void (*fp)(void);
//extern void PowerON_Reset(void);
extern void stack(void);
extern void startup(void);

#define VECT_SECT          __attribute__ ((section (".vects")))

const void *HardwareVectors[] VECT_SECT = {
//;<<VECTOR DATA PowerON_Reset  (POWER ON RESET)>>
//;0 Power On Reset PC
        //PowerON_Reset,
        startup,
//;<<VECTOR DATA END (POWER ON RESET)>>
// 1 Power On Reset SP
        (fp)stack,
//;<<VECTOR DATA PowerON_Reset  (MANUAL RESET)>>
//;2 Manual Reset PC
        INT_Manual_Reset_PC,
//;<<VECTOR DATA END (MANUAL RESET)>>
// 3 Manual Reset SP
        INT_Manual_Reset_SP,
// 4 Illegal code
        INT_Illegal_code,
// 5 Reserved
        (fp)0,
// 6 Illegal slot
        INT_Illegal_slot,
// 7 Reserved
        (fp)0,
// 8 Reserved
        (fp)0,
// 9 CPU Address error
        INT_CPU_Address,
// 10 BUS error
        INT_DMAC_Address,
// 11 NMI
        INT_NMI,
// 12 Reserved
        (fp)0,
// 13 INT_FPU
        INT_FPU,
// 14 H-UDI
        INT_HUDI,
// 15 Bank Overflow
        INT_Bank_Overflow,
// 16 Bank Underflow
        INT_Bank_Underflow,
// 17 Divide by zero
        INT_Divide_by_Zero,
// 18 Divide Overflow
        INT_Divide_Overflow,
// 19 Reserved
        (fp)0,
// 20 Reserved
        (fp)0,
// 21 Reserved
        (fp)0,
// 22 Reserved
        (fp)0,
// 23 Reserved
        (fp)0,
// 24 Reserved
        (fp)0,
// 25 Reserved
        (fp)0,
// 26 Reserved
        (fp)0,
// 27 Reserved
        (fp)0,
// 28 Reserved
        (fp)0,
// 29 Reserved
        (fp)0,
// 30 Reserved
        (fp)0,
// 31 Reserved
        (fp)0,
// 32 TRAPA (User Vecter)
        INT_TRAPA32,
// 33 TRAPA (User Vecter)
        INT_TRAPA33,
// 34 TRAPA (User Vecter)
        INT_TRAPA34,
// 35 TRAPA (User Vecter)
        INT_TRAPA35,
// 36 TRAPA (User Vecter)
        INT_TRAPA36,
// 37 TRAPA (User Vecter)
        INT_TRAPA37,
// 38 TRAPA (User Vecter)
        INT_TRAPA38,
// 39 TRAPA (User Vecter)
        INT_TRAPA39,
// 40 TRAPA (User Vecter)
        INT_TRAPA40,
// 41 TRAPA (User Vecter)
        INT_TRAPA41,
// 42 TRAPA (User Vecter)
        INT_TRAPA42,
// 43 TRAPA (User Vecter)
        INT_TRAPA43,
// 44 TRAPA (User Vecter)
        INT_TRAPA44,
// 45 TRAPA (User Vecter)
        INT_TRAPA45,
// 46 TRAPA (User Vecter)
        INT_TRAPA46,
// 47 TRAPA (User Vecter)
        INT_TRAPA47,
// 48 TRAPA (User Vecter)
        INT_TRAPA48,
// 49 TRAPA (User Vecter)
        INT_TRAPA49,
// 50 TRAPA (User Vecter)
        INT_TRAPA50,
// 51 TRAPA (User Vecter)
        INT_TRAPA51,
// 52 TRAPA (User Vecter)
        INT_TRAPA52,
// 53 TRAPA (User Vecter)
        INT_TRAPA53,
// 54 TRAPA (User Vecter)
        INT_TRAPA54,
// 55 TRAPA (User Vecter)
        INT_TRAPA55,
// 56 TRAPA (User Vecter)
        INT_TRAPA56,
// 57 TRAPA (User Vecter)
        INT_TRAPA57,
// 58 TRAPA (User Vecter)
        INT_TRAPA58,
// 59 TRAPA (User Vecter)
        INT_TRAPA59,
// 60 TRAPA (User Vecter)
        INT_TRAPA60,
// 61 TRAPA (User Vecter)
        INT_TRAPA61,
// 62 TRAPA (User Vecter)
        INT_TRAPA62,
// 63 TRAPA (User Vecter)
        INT_TRAPA63,
// 64 Interrupt IRQ0
        INT_IRQ_IRQ0,
// 65 Interrupt IRQ1
        INT_IRQ_IRQ1,
// 66 Interrupt IRQ2
        INT_IRQ_IRQ2,
// 67 Interrupt IRQ3
        INT_IRQ_IRQ3,
// 68 Interrupt IRQ4
        INT_IRQ_IRQ4,
// 69 Interrupt IRQ5
        INT_IRQ_IRQ5,
// 70 Interrupt IRQ6
        INT_IRQ_IRQ6,
// 71 Interrupt IRQ7
        INT_IRQ_IRQ7,
// 72 Reserved
        (fp)0,
// 73 Reserved
        (fp)0,
// 74 Reserved
        (fp)0,
// 75 Reserved
        (fp)0,
// 76 Reserved
        (fp)0,
// 77 Reserved
        (fp)0,
// 78 Reserved
        (fp)0,
// 79 Reserved
        (fp)0,
// 80 Interrupt PINT0
        INT_PINT_PINT0,
// 81 Interrupt PINT1
        INT_PINT_PINT1,
// 82 Interrupt PINT2
        INT_PINT_PINT2,
// 83 Interrupt PINT3
        INT_PINT_PINT3,
// 84 Interrupt PINT4
        INT_PINT_PINT4,
// 85 Interrupt PINT5
        INT_PINT_PINT5,
// 86 Interrupt PINT6
        INT_PINT_PINT6,
// 87 Interrupt PINT7
        INT_PINT_PINT7,
// 88 Reserved
        (fp)0,
// 89 Reserved
        (fp)0,
// 90 Reserved
        (fp)0,
// 91 Reserved
        (fp)0,
// 92 Reserved
        (fp)0,
// 93 Reserved
        (fp)0,
// 94 Reserved
        (fp)0,
// 95 Reserved
        (fp)0,
// 96 Reserved
        (fp)0,
// 97 Reserved
        (fp)0,
// 98 Reserved
        (fp)0,
// 99 Reserved
        (fp)0,
// 100 Reserved
        (fp)0,
// 101 Reserved
        (fp)0,
// 102 Reserved
        (fp)0,
// 103 Reserved
        (fp)0,
// 104 Reserved
        (fp)0,
// 105 Reserved
        (fp)0,
// 106 Reserved
        (fp)0,
// 107 Reserved
        (fp)0,
// 108 DMAC0 DEI0
        INT_DMAC0_DEI0,
// 109 DMAC0 HEI0
        INT_DMAC0_HEI0,
// 110 Reserved
        (fp)0,
// 111 Reserved
        (fp)0,
// 112 DMAC1 DEI1
        INT_DMAC1_DEI1,
// 113 DMAC1 HEI1
        INT_DMAC1_HEI1,
// 114 Reserved
        (fp)0,
// 115 Reserved
        (fp)0,
// 116 DMAC2 DEI2
        INT_DMAC2_DEI2,
// 117 DMAC2 HEI2
        INT_DMAC2_HEI2,
// 118 Reserved
        (fp)0,
// 119 Reserved
        (fp)0,
// 120 DMAC3 DEI3
        INT_DMAC3_DEI3,
// 121 DMAC3 HEI3
        INT_DMAC3_HEI3,
// 122 Reserved
        (fp)0,
// 123 Reserved
        (fp)0,
// 124 DMAC4 DEI4
        INT_DMAC4_DEI4,
// 125 DMAC4 HEI4
        INT_DMAC4_HEI4,
// 126 Reserved
        (fp)0,
// 127 Reserved
        (fp)0,
// 128 DMAC5 DEI5
        INT_DMAC5_DEI5,
// 129 DMAC5 HEI5
        INT_DMAC5_HEI5,
// 130 Reserved
        (fp)0,
// 131 Reserved
        (fp)0,
// 132 DMAC6 DEI6
        INT_DMAC6_DEI6,
// 133 DMAC6 HEI6
        INT_DMAC6_HEI6,
// 134 Reserved
        (fp)0,
// 135 Reserved
        (fp)0,
// 136 DMAC7 DEI7
        INT_DMAC7_DEI7,
// 137 DMAC7 HEI7
        INT_DMAC7_HEI7,
// 138 Reserved
        (fp)0,
// 139 Reserved
        (fp)0,
// 140 DMAC8 DEI8
        INT_DMAC8_DEI8,
// 141 DMAC8 HEI8
        INT_DMAC8_HEI8,
// 142 Reserved
        (fp)0,
// 143 Reserved
        (fp)0,
// 144 DMAC9 DEI9
        INT_DMAC9_DEI9,
// 145 DMAC9 HEI9
        INT_DMAC9_HEI9,
// 146 Reserved
        (fp)0,
// 147 Reserved
        (fp)0,
// 148 DMAC10 DEI10
        INT_DMAC10_DEI10,
// 149 DMAC10 HEI10
        INT_DMAC10_HEI10,
// 150 Reserved
        (fp)0,
// 151 Reserved
        (fp)0,
// 152 DMAC11 DEI11
        INT_DMAC11_DEI11,
// 153 DMAC11 HEI11
        INT_DMAC11_HEI11,
// 154 Reserved
        (fp)0,
// 155 Reserved
        (fp)0,
// 156 DMAC12 DEI12
        INT_DMAC12_DEI12,
// 157 DMAC12 HEI12
        INT_DMAC12_HEI12,
// 158 Reserved
        (fp)0,
// 159 Reserved
        (fp)0,
// 160 DMAC13 DEI13
        INT_DMAC13_DEI13,
// 161 DMAC13 HEI13
        INT_DMAC13_HEI13,
// 162 Reserved
        (fp)0,
// 163 Reserved
        (fp)0,
// 164 DMAC14 DEI14
        INT_DMAC14_DEI14,
// 165 DMAC14 HEI14
        INT_DMAC14_HEI14,
// 166 Reserved
        (fp)0,
// 167 Reserved
        (fp)0,
// 168 DMAC15 DEI15
        INT_DMAC15_DEI15,
// 169 DMAC15 HEI15
        INT_DMAC15_HEI15,
// 170 USB USBI
        INT_USB_USBI,
// 171 VDC3 VIVSYNCJ
        INT_VDC3_VIVSYNCJ,
// 172 VDC3 VBUFERRJ
        INT_VDC3_VBUFERR,
// 173 VDC3 VIFIELDE
        INT_VDC3_VIFIELDE,
// 174 VDC3 VOLINE
        INT_VDC3_VOLINE,
// 175 CMT CMT0 CMI0
        INT_CMT0_CMI0,
// 176 CMT CMT1 CMI1
        INT_CMT1_CMI1,
// 177 BSC CMI
        INT_BSC_CMI,
// 178 WDT
        INT_WDT_ITI,
// 179 MTU2 MTU0 TGI0A
        INT_MTU2_TGI0A,
// 180 MTU2 MTU0 TGI0B
        INT_MTU2_TGI0B,
// 181 MTU2 MTU0 TGI0C
        INT_MTU2_TGI0C,
// 182 MTU2 MTU0 TGI0D
        INT_MTU2_TGI0D,
// 183 MTU2 MTU0 TCI0V
        INT_MTU2_TCI0V,
// 184 MTU2 MTU0 TGI0E
        INT_MTU2_TGI0E,
// 185 MTU2 MTU0 TGI0F
        INT_MTU2_TGI0F,
// 186 MTU2 MTU1 TGI1A
        INT_MTU2_TGI1A,
// 187 MTU2 MTU1 TGI1B
        INT_MTU2_TGI1B,
// 188 MTU2 MTU1 TCI1V
        INT_MTU2_TCI1V,
// 189 MTU2 MTU1 TCI1U
        INT_MTU2_TCI1U,
// 190 MTU2 MTU2 TGI2A
        INT_MTU2_TGI2A,
// 191 MTU2 MTU2 TGI2B
        INT_MTU2_TGI2B,
// 192 MTU2 MTU2 TCI2V
        INT_MTU2_TCI2V,
// 193 MTU2 MTU2 TCI2U
        INT_MTU2_TCI2U,
// 194 MTU2 MTU3 TGI3A
        INT_MTU2_TGI3A,
// 195 MTU2 MTU3 TGI3B
        INT_MTU2_TGI3B,
// 196 MTU2 MTU3 TGI3C
        INT_MTU2_TGI3C,
// 197 MTU2 MTU3 TGI3D
        INT_MTU2_TGI3D,
// 198 MTU2 MTU3 TCI3V
        INT_MTU2_TCI3V,
// 199 MTU2 MTU4 TGI4A
        INT_MTU2_TGI4A,
// 200 MTU2 MTU4 TGI4B
        INT_MTU2_TGI4B,
// 201 MTU2 MTU4 TGI4C
        INT_MTU2_TGI4C,
// 202 MTU2 MTU4 TGI4D
        INT_MTU2_TGI4D,
// 203 MTU2 MTU4 TCI4V
        INT_MTU2_TCI4V,
// 204 PWM PWM1 
        INT_PWM1,
// 205 PWM PWM2
        INT_PWM2,
// 206 A/D ADI
        INT_AD_ADI,
// 207 SSI0 SSIF0
        INT_SSI0_SSIF0,
// 208 SSI0 SSIRXI0
        INT_SSI0_SSIRXI0,
// 209 SSI0 SSITXI0
        INT_SSI0_SSITXI0,
// 210 SSI1 SSII1
        INT_SSI1_SSII1,
// 211 SSI1 SSIRTI1
        INT_SSI1_SSIRTI1,
// 212 SSI2 SSII2
        INT_SSI2_SSII2,
// 213 SSI2 SSIRTI2
        INT_SSI2_SSIRTI2,
// 214 SSI3 SSII3
        INT_SSI3_SSII3,
// 215 SSI3 SSIRTI3
        INT_SSI3_SSIRTI3,
// 216 SPDIF SPDIFI
        INT_SPDIF_SPDIFI,
// 217 IIC3 IIC0 STPI0
        INT_IIC3_STPI0,
// 218 IIC3 IIC0 NAKI0
        INT_IIC3_NAKI0,
// 219 IIC3 IIC0 RXI0
        INT_IIC3_RXI0,
// 220 IIC3 IIC0 TXI0
        INT_IIC3_TXI0,
// 221 IIC3 IIC0 TEI0
        INT_IIC3_TEI0,
// 222 IIC3 IIC1 STPI1
        INT_IIC3_STPI1,
// 223 IIC3 IIC1 NAKI1
        INT_IIC3_NAKI1,
// 224 IIC3 IIC1 RXI1
        INT_IIC3_RXI1,
// 225 IIC3 IIC1 TXI1
        INT_IIC3_TXI1,
// 226 IIC3 IIC1 TEI1
        INT_IIC3_TEI1,
// 227 IIC3 IIC2 STPI2
        INT_IIC3_STPI2,
// 228 IIC3 IIC2 NAKI2
        INT_IIC3_NAKI2,
// 229 IIC3 IIC2 RXI2
        INT_IIC3_RXI2,
// 230 IIC3 IIC2 TXI2
        INT_IIC3_TXI2,
// 231 IIC3 IIC2 TEI2
        INT_IIC3_TEI2,
// 232 SCIF SCIF0 BRI0
        INT_SCIF0_BRI0,
// 233 SCIF SCIF0 ERI0
        INT_SCIF0_ERI0,
// 234 SCIF SCIF0 RXI0
        INT_SCIF0_RXI0,
// 235 SCIF SCIF0 TXI0
        INT_SCIF0_TXI0,
// 236 SCIF SCIF1 BRI1
        INT_SCIF1_BRI1,
// 237 SCIF SCIF1 ERI1
        INT_SCIF1_ERI1,
// 238 SCIF SCIF1 RXI1
        INT_SCIF1_RXI1,
// 239 SCIF SCIF1 TXI1
        INT_SCIF1_TXI1,
// 240 SCIF SCIF2 BRI2
        INT_SCIF2_BRI2,
// 241 SCIF SCIF2 ERI2
        INT_SCIF2_ERI2,
// 242 SCIF SCIF2 RXI2
        INT_SCIF2_RXI2,
// 243 SCIF SCIF2 TXI2
        INT_SCIF2_TXI2,
// 244 SCIF SCIF3 BRI3
        INT_SCIF3_BRI3,
// 245 SCIF SCIF3 ERI3
        INT_SCIF3_ERI3,
// 246 SCIF SCIF3 RXI3
        INT_SCIF3_RXI3,
// 247 SCIF SCIF3 TXI3
        INT_SCIF3_TXI3,
// 248 SCIF SCIF4 BRI4
        INT_SCIF4_BRI4,
// 249 SCIF SCIF4 ERI4
        INT_SCIF4_ERI4,
// 250 SCIF SCIF4 RXI4
        INT_SCIF4_RXI4,
// 251 SCIF SCIF4 TXI4
        INT_SCIF4_TXI4,
// 252 SCIF SCIF5 BRI5
        INT_SCIF5_BRI5,
// 253 SCIF SCIF5 ERI5
        INT_SCIF5_ERI5,
// 254 SCIF SCIF5 RXI5
        INT_SCIF5_RXI5,
// 255 SCIF SCIF5 TXI5
        INT_SCIF5_TXI5,
// 256 SCIF SCIF6 BRI6
        INT_SCIF6_BRI6,
// 257 SCIF SCIF6 ERI6
        INT_SCIF6_ERI6,
// 258 SCIF SCIF6 RXI6
        INT_SCIF6_RXI6,
// 259 SCIF SCIF6 TXI6
        INT_SCIF6_TXI6,
// 260 SCIF SCIF7 BRI7
        INT_SCIF7_BRI7,
// 261 SCIF SCIF7 ERI7
        INT_SCIF7_ERI7,
// 262 SCIF SCIF7 RXI7
        INT_SCIF7_RXI7,
// 263 SCIF SCIF7 TXI7
        INT_SCIF7_TXI7,
// 264 SIOF SIOFI
        INT_SIOF_SIOFI,
// 265 RSPI RSPI0 SPEI0
        INT_RSPI0_SPEI0,
// 266 RSPI RSPI0 SPRI0
        INT_RSPI0_SPRI0,
// 267 RSPI RSPI0 SPTI0
        INT_RSPI0_SPTI0,
// 268 RSPI RSPI1 SPEI1
        INT_RSPI1_SPEI1,
// 269 RSPI RSPI1 SPRI1
        INT_RSPI1_SPRI1,
// 270 RSPI RSPI1 SPTI1
        INT_RSPI1_SPTI1,
// 271 RCAN RCAN0 ERS0 
        INT_RCAN0_ERS0,
// 272 RCAN RCAN0 OVR0 
        INT_RCAN0_OVR0,
// 273 RCAN RCAN0 RM00 
        INT_RCAN0_RM00,
// 274 RCAN RCAN0 RM10 
        INT_RCAN0_RM10,
// 275 RCAN RCAN0 SLE0 
        INT_RCAN0_SLE0,
// 276 RCAN RCAN1 ERS1 
        INT_RCAN1_ERS1,
// 277 RCAN RCAN1 OVR1 
        INT_RCAN1_OVR1,
// 278 RCAN RCAN1 RM01 
        INT_RCAN1_RM01,
// 279 RCAN RCAN1 RM11 
        INT_RCAN1_RM11,
// 280 RCAN RCAN1 SLE1 
        INT_RCAN1_SLE1,
// 281 IEB IEB 
        INT_IEB_IEB,
// 282 ROMDEC ISY 
        INT_ROMDEC_ISY,
// 283 ROMDEC IERR 
        INT_ROMDEC_IERR,
// 284 ROMDEC ITARG 
        INT_ROMDEC_ITARG,
// 285 ROMDEC ISEC 
        INT_ROMDEC_ISEC,
// 286 ROMDEC IBUF 
        INT_ROMDEC_IBUF,
// 287 ROMDEC IREADY 
        INT_ROMDEC_IREADY,
// 288 FLCTL FLSTEI 
        INT_FLCTL_FLSTEI,
// 289 FLCTL FLTENDI 
        INT_FLCTL_FLTENDI,
// 290 FLCTL FLTREQ0I 
        INT_FLCTL_FLTREQ0I,
// 291 FLCTL FLTREQ1I 
        INT_FLCTL_FLTREQ1I,
// 292 SDHI SDHI3 
        INT_SDHI_SDHI3,
// 293 SDHI SDHI0 
        INT_SDHI_SDHI0,
// 294 SDHI SDHI1 
        INT_SDHI_SDHI1,
// 295 Reserved
        (fp)0,
// 296 RTC ARM 
        INT_RTC_ARM,
// 297 RTC PRD 
        INT_RTC_PRD,
// 298 RTC CUP 
        INT_RTC_CUP,
// 299 SRC SRC0 OVF0 
        INT_SRC0_OVF0,
// 300 SRC SRC0 UDF0 
        INT_SRC0_UDF0,
// 301 SRC SRC0 CEF0 
        INT_SRC0_CEF0,
// 302 SRC SRC0 ODFI0 
        INT_SRC0_ODFI0,
// 303 SRC SRC0 IDEI0 
        INT_SRC0_IDEI0,
// 304 SRC SRC1 OVF1 
        INT_SRC1_OVF1,
// 305 SRC SRC1 UDF1 
        INT_SRC1_UDF1,
// 306 SRC SRC1 CEF1 
        INT_SRC1_CEF1,
// 307 SRC SRC1 ODFI1 
        INT_SRC1_ODFI1,
// 308 SRC SRC1 IDEI1 
        INT_SRC1_IDEI1,
// 309 Reserved
        (fp)0,
// 310 DCU DCUEI 
        INT_DCU_DCUEI,
// 311 DCU OFFI 
        INT_DCU_OFFI,
// 312 DCU IFEI 
        INT_DCU_IFEI, };

typedef void (*FUNCPTR)(void);

void CPU_IRQInterrupt(int no)
{
    //if ((no >= 4) && (no << 310))
    (*(FUNCPTR)HardwareVectors[no])();
}

/* End of File */