@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@  Licensed under the Apache License, Version 2.0 (the "License")@
@  you may not use this file except in compliance with the License.
@  You may obtain a copy of the License at http:@www.apache.org/licenses/LICENSE-2.0
@
@  Copyright (c) Microsoft Corporation. All rights reserved.
@  Implementation for STM32: Copyright (c) Oberon microsystems, Inc.
@  Modified by Kentaro Sekimoto
@
@  CORTEX-M3 Standard Entry Code
@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    .syntax unified
	.cpu cortex-m3
    .thumb
    .code 16

  	.global startup
@  	.global _start

	.extern SystemInit
	.extern main

@_estack = 0x20010000;
_estack = 0x20010000;

@Stack_Size	=	0x00000200
@
@	.section	.bss,       "a", %progbits
@Stack_Mem:
@	.space	Stack_Size
@__initial_sp:

	.section	.isr_vector, "ax", %progbits

EntryPoint:

@ The first word has several functions:
@ - It is the entry point of the application
@ - it contains a signature word used to identify application blocks
@ - out of reset it contains the initial stack pointer value
@ - it is the first entry of the initial exception handler table
@ The actual word used is 0x2000E00C

@	.word	_estack
@	.word	startup			@ Reset
@	.word	Fault_Handler	@ NMI
@	.word	Fault_Handler	@ Hard Fault
@	.word	Fault_Handler	@ MMU Fault
@	.word	Fault_Handler	@ Bus Fault
@	.word	Fault_Handler	@ Usage Fault

	.word	_estack				@ Top of Stack
@	.word	Reset_Handler		@ Reset
	.word	startup				@ Reset
	.word	NMI_Handler			@ NMI
	.word	HardFault_Handler	@ Hard Fault
	.word	MemManage_Handler	@ MPU Fault
	.word	BusFault_Handler	@ Bus Fault
	.word	UsageFault_Handler	@ Usage Fault
	.word	0					@ Reserved
	.word	0					@ Reserved
	.word	0					@ Reserved
	.word	0					@ Reserved
	.word	SVC_Handler			@ SVCall
	.word	DebugMon_Handler	@ Debug Monitor
	.word	0					@ Reserved
	.word	PendSV_Handler		@ PendSV
	.word	SysTick_Handler		@ SysTick

	.word	CSV_IRQHandler		@ 0: Clock Super Visor
	.word	SWDT_IRQHandler		@ 1: Software Watchdog Timer
	.word	LVD_IRQHandler		@ 2: Low Voltage Detector
	.word	IRQ003SEL_IRQHandler	@ 3:
	.word	IRQ004SEL_IRQHandler	@ 4:
	.word	IRQ005SEL_IRQHandler	@ 5:
	.word	IRQ006SEL_IRQHandler	@ 6:
	.word	IRQ007SEL_IRQHandler	@ 7:
	.word	IRQ008SEL_IRQHandler	@ 8:
	.word	IRQ009SEL_IRQHandler	@ 9:
	.word	IRQ010SEL_IRQHandler	@ 10:
	.word	EXINT0_IRQHandler		@ 11:
	.word	EXINT1_IRQHandler		@ 12:
	.word	EXINT2_IRQHandler		@ 13:
	.word	EXINT3_IRQHandler		@ 14:
	.word	EXINT4_IRQHandler		@ 15:
	.word	EXINT5_IRQHandler		@ 16:
	.word	EXINT6_IRQHandler		@ 17:
	.word	EXINT7_IRQHandler		@ 18:
	.word	QPRC0_IRQHandler		@ 19:
	.word	QPRC1_IRQHandler		@ 20:
	.word	WFG0_DTIF0_IRQHandler	@ 21:
	.word	WFG1_DTIF1_IRQHandler	@ 22:
	.word	WFG2_DTIF2_IRQHandler	@ 23:
	.word	FRT0_PEAK_IRQHandler	@ 24:
	.word	FRT0_ZERO_IRQHandler	@ 25:
	.word	ICU0_IRQHandler			@ 26:
	.word	OCU0_IRQHandler			@ 27:
	.word	FRT1_PEAK_IRQHandler	@ 28:
	.word	FRT1_ZERO_IRQHandler	@ 29:
	.word	ICU1_IRQHandler			@ 30:
	.word	OCU1_IRQHandler			@ 31:
	.word	FRT2_PEAK_IRQHandler	@ 32:
	.word	FRT2_ZERO_IRQHandler	@ 33:
	.word	ICU2_IRQHandler			@ 34:
	.word	OCU2_IRQHandler			@ 35:
	.word	PPG00_02_04_IRQHandler	@ 36:
	.word	PPG08_10_12_IRQHandler	@ 37:
	.word	PPG16_18_20_IRQHandler	@ 38:
	.word	BT0_IRQHandler			@ 39:
	.word	BT1_IRQHandler			@ 40:
	.word	BT2_IRQHandler			@ 41:
	.word	BT3_IRQHandler			@ 42:
	.word	BT4_IRQHandler			@ 43:
	.word	BT5_IRQHandler			@ 44:
	.word	BT6_IRQHandler			@ 45:
	.word	BT7_IRQHandler			@ 46:
	.word	DT1_2_IRAHandler		@ 47:
	.word	WC_IRQHandler			@ 48:
	.word	EXTBUS_ERR_Handler		@ 49:
	.word	RTC_IRQHandler			@ 50:
	.word	EXTINT8_IRQHandler		@ 51:
	.word	EXTINT9_IRQHandler		@ 52:
	.word	EXTINT10_IRQHandler		@ 53:
	.word	EXTINT11_IRQHandler		@ 54:
	.word	EXTINT12_IRQHandler		@ 55:
	.word	EXTINT13_IRQHandler		@ 56:
	.word	EXTINT14_IRQHandler		@ 57:
	.word	EXTINT15_IRQHandler		@ 58:
	.word	TIM_IRQHandler			@ 59:
	.word	MFS0_RX_IRQHandler		@ 60:
	.word	MFS0_TX_IRQHandler		@ 61:
	.word	MFS1_RX_IRQHandler		@ 62:
	.word	MFS1_TX_IRQHandler		@ 63:
	.word	MFS2_RX_IRQHandler		@ 64:
	.word	MFS2_TX_IRQHandler		@ 65:
	.word	MFS3_RX_IRQHandler		@ 66:
	.word	MFS3_TX_IRQHandler		@ 67:
	.word	MFS4_RX_IRQHandler		@ 68:
	.word	MFS4_TX_IRQHandler		@ 69:
	.word	MFS5_RX_IRQHandler		@ 70:
	.word	MFS5_TX_IRQHandler		@ 71:
	.word	MFS6_RX_IRQHandler		@ 72:
	.word	MFS6_TX_IRQHandler		@ 73:
	.word	MFS7_RX_IRQHandler		@ 74:
	.word	MFS7_TX_IRQHandler		@ 75:
	.word	ADC0_IRQHandler			@ 76:
	.word	ADC1_IRQHandler			@ 77:
	.word	USB0_IRQHandler			@ 78:
	.word	USB0_HOST_IRQHandler	@ 79:
	.word	CAN0_IRQHandler			@ 80:
	.word	CAN1_IRQHandler			@ 81:
	.word	ETHER0_IRQHandler		@ 82:
	.word	DMAC0_IRQHandler		@ 83:
	.word	DMAC1_IRQHandler		@ 84:
	.word	DMAC2_IRQHandler		@ 85:
	.word	DMAC3_IRQHandler		@ 86:
	.word	DMAC4_IRQHandler		@ 87:
	.word	DMAC5_IRQHandler		@ 88:
	.word	DMAC6_IRQHandler		@ 89:
	.word	DMAC7_IRQHandler		@ 90:
	.word	DSTC_IRQHandler			@ 91:
	.word	EXINT16_17_18_19_IRQHandler @ 92:
	.word	EXINT20_21_22_23_IRQHandler	@ 93:
	.word	EXINT24_25_26_27_IRQHandler	@ 94:
	.word	EXINT28_29_30_31_IRQHandler	@ 95:
	.word	QPRC2_IRQHandler			@ 96:
	.word	QPRC3_IRQHandler			@ 97:
	.word	BT8_IRQHandler				@ 98:
	.word	BT9_IRQHandler				@ 99:
	.word	BT10_IRQHandler				@ 100:
	.word	BT11_IRQHandler				@ 101:
	.word	BT12_15_IRQHandler			@ 102:
	.word	MFS8_RX_IRQHandler			@ 103:
	.word	MFS8_TX_IRQHandler			@ 104:
	.word	MFS9_RX_IRQHandler			@ 105:
	.word	MFS9_TX_IRQHandler			@ 106:
	.word	MFS10_RX_IRQHandler			@ 107:
	.word	MFS10_TX_IRQHandler			@ 108:
	.word	MFS11_RX_IRQHandler			@ 109:
	.word	MFS11_TX_IRQHandler			@ 110:
	.word	ADC2_IRQHandler				@ 111:
	.word	Dummy						@ 112:
	.word	USB1_IRQHandler				@ 113:
	.word	USB1_HOST_IRQHandler		@ 114:
	.word	Dummy						@ 115:
	.word	Dummy						@ 116:
	.word	Dummy						@ 117:
	.word	SD_IRQHandler				@ 118:
	.word	FLASHIF_IRQHandler			@ 119:
	.word	MFS12_RX_IRQHandler			@ 120:
	.word	MFS12_TX_IRQHandler			@ 121:
	.word	MFS13_RX_IRQHandler			@ 122:
	.word	MFS13_TX_IRQHandler			@ 123:
	.word	MFS14_RX_IRQHandler			@ 124:
	.word	MFS14_TX_IRQHandler			@ 125:
	.word	MFS15_RX_IRQHandler			@ 126:
	.word	MFS15_TX_IRQHandler			@ 127:

	.thumb_func
	.type  startup, %function
startup:
@_start:
	ldr		r0, =_estack
	msr		msp, r0
@	===== memory initialize =====
	ldr		r0, = _sidata
	ldr		r1, = _sdata
	ldr		r2, = _edata
loop1:
	ldrb	r3, [r0]
	strb	r3, [r1, #0]
	add		r0, r0, #1
	add		r1, r1, #1
	cmp		r2, r1
	bgt		loop1
@
	eor		r0, r0
	ldr		r1, =_sbss
	ldr		r2, =_ebss
loop2:
	strb	r0, [r1, #0]
	add		r1, r1, #1
	cmp		r2, r1
	bgt		loop2
@	=============================
	ldr		r0, =SystemInit
	blx		r0
	ldr		r0, =main
	bx		r0
@	.align	4

	.thumb_func
	.weak	Fault_Handler
	.type	Fault_Handler, %function
Fault_Handler:
	b	Fault_Handler

	.thumb_func
	.weak	NMI_Handler
	.type	NMI_Handler, %function
NMI_Handler:
	b	NMI_Handler

	.thumb_func
	.weak	HardFault_Handler
	.type	HardFault_Handler, %function
HardFault_Handler:
	b	HardFault_Handler

	.thumb_func
	.weak	MemManage_Handler
	.type	MemManage_Handler, %function
MemManage_Handler:
	b	MemManage_Handler

	.thumb_func
	.weak	BusFault_Handler
	.type	BusFault_Handler, %function
BusFault_Handler:
	b	BusFault_Handler

	.thumb_func
	.weak	UsageFault_Handler
	.type	UsageFault_Handler, %function
UsageFault_Handler:
	b	UsageFault_Handler

	.thumb_func
	.weak	SVC_Handler
	.type	SVC_Handler, %function
SVC_Handler:
	b	SVC_Handler

	.thumb_func
	.weak	DebugMon_Handler
	.type	DebugMon_Handler, %function
DebugMon_Handler:
	b	DebugMon_Handler

	.thumb_func
	.weak	PendSV_Handler
	.type	PendSV_Handler, %function
PendSV_Handler:
	b	PendSV_Handler

	.thumb_func
	.weak	SysTick_Handler
	.type	SysTick_Handler, %function
SysTick_Handler:
	b	SysTick_Handler

	.thumb_func
	.weak	CSV_IRQHandlerr
	.type	CSV_IRQHandlerr, %function
CSV_IRQHandler:
	b	CSV_IRQHandlerr

	.thumb_func
	.weak	SWDT_IRQHandler
	.type	SWDT_IRQHandler, %function
SWDT_IRQHandler:
	b	SWDT_IRQHandler

	.thumb_func
	.weak	LVD_IRQHandler
	.type	LVD_IRQHandler, %function
LVD_IRQHandler:
	b	LVD_IRQHandler

	.thumb_func
	.weak	IRQ003SEL_IRQHandler
	.type	IRQ003SEL_IRQHandler, %function
IRQ003SEL_IRQHandler:
	b	IRQ003SEL_IRQHandler

	.thumb_func
	.weak	IRQ004SEL_IRQHandler
	.type	IRQ004SEL_IRQHandler, %function
IRQ004SEL_IRQHandler:
	b	IRQ004SEL_IRQHandler

	.thumb_func
	.weak	IRQ005SEL_IRQHandler
	.type	IRQ005SEL_IRQHandler, %function
IRQ005SEL_IRQHandler:
	b	IRQ005SEL_IRQHandler

	.thumb_func
	.weak	IRQ006SEL_IRQHandler
	.type	IRQ006SEL_IRQHandler, %function
IRQ006SEL_IRQHandler:
	b	IRQ006SEL_IRQHandler

	.thumb_func
	.weak	IRQ007SEL_IRQHandler
	.type	IRQ007SEL_IRQHandler, %function
IRQ007SEL_IRQHandler:
	b	IRQ007SEL_IRQHandler

	.thumb_func
	.weak	IRQ008SEL_IRQHandler
	.type	IRQ008SEL_IRQHandler, %function
IRQ008SEL_IRQHandler:
	b	IRQ008SEL_IRQHandler

	.thumb_func
	.weak	IRQ009SEL_IRQHandler
	.type	IRQ009SEL_IRQHandler, %function
IRQ009SEL_IRQHandler:
	b	IRQ009SEL_IRQHandler

	.thumb_func
	.weak	IRQ010SEL_IRQHandler
	.type	IRQ010SEL_IRQHandler, %function
IRQ010SEL_IRQHandler:
	b	IRQ010SEL_IRQHandler

	.thumb_func
	.weak	EXINT0_IRQHandler
	.type	EXINT0_IRQHandler, %function
EXINT0_IRQHandler:
	b	EXINT0_IRQHandler

	.thumb_func
	.weak	EXINT1_IRQHandler
	.type	EXINT1_IRQHandler, %function
EXINT1_IRQHandler:
	b	EXINT1_IRQHandler

	.thumb_func
	.weak	EXINT2_IRQHandler
	.type	EXINT2_IRQHandler, %function
EXINT2_IRQHandler:
	b	EXINT2_IRQHandler

	.thumb_func
	.weak	EXINT3_IRQHandler
	.type	EXINT3_IRQHandler, %function
EXINT3_IRQHandler:
	b	EXINT3_IRQHandler

	.thumb_func
	.weak	EXINT4_IRQHandler
	.type	EXINT4_IRQHandler, %function
EXINT4_IRQHandler:
	b	EXINT4_IRQHandler

	.thumb_func
	.weak	EXINT5_IRQHandler
	.type	EXINT5_IRQHandler, %function
EXINT5_IRQHandler:
	b	EXINT5_IRQHandler

	.thumb_func
	.weak	EXINT6_IRQHandler
	.type	EXINT6_IRQHandler, %function
EXINT6_IRQHandler:
	b	EXINT6_IRQHandler

	.thumb_func
	.weak	EXINT7_IRQHandler
	.type	EXINT7_IRQHandler, %function
EXINT7_IRQHandler:
	b	EXINT7_IRQHandler

	.thumb_func
	.weak	QPRC0_IRQHandler
	.type	QPRC0_IRQHandler, %function
QPRC0_IRQHandler:
	b	QPRC0_IRQHandler

	.thumb_func
	.weak	QPRC1_IRQHandler
	.type	QPRC1_IRQHandler, %function
QPRC1_IRQHandler:
	b	QPRC1_IRQHandler

	.thumb_func
	.weak	WFG0_DTIF0_IRQHandler
	.type	WFG0_DTIF0_IRQHandler, %function
WFG0_DTIF0_IRQHandler:
	b	WFG0_DTIF0_IRQHandler

	.thumb_func
	.weak	WFG1_DTIF1_IRQHandler
	.type	WFG1_DTIF1_IRQHandler, %function
WFG1_DTIF1_IRQHandler:
	b	WFG1_DTIF1_IRQHandler

	.thumb_func
	.weak	WFG2_DTIF2_IRQHandler
	.type	WFG2_DTIF2_IRQHandler, %function
WFG2_DTIF2_IRQHandler:
	b	WFG2_DTIF2_IRQHandler

	.thumb_func
	.weak	FRT0_PEAK_IRQHandler
	.type	FRT0_PEAK_IRQHandler, %function
FRT0_PEAK_IRQHandler:
	b	FRT0_PEAK_IRQHandler

	.thumb_func
	.weak	FRT0_ZERO_IRQHandler
	.type	FRT0_ZERO_IRQHandler, %function
FRT0_ZERO_IRQHandler:
	b	FRT0_ZERO_IRQHandler

	.thumb_func
	.weak	ICU0_IRQHandler
	.type	ICU0_IRQHandler	, %function
ICU0_IRQHandler:
	b	ICU0_IRQHandler

	.thumb_func
	.weak	OCU0_IRQHandler
	.type	OCU0_IRQHandler	, %function
OCU0_IRQHandler	:
	b	OCU0_IRQHandler

	.thumb_func
	.weak	FRT1_PEAK_IRQHandler
	.type	FRT1_PEAK_IRQHandler, %function
FRT1_PEAK_IRQHandler:
	b	FRT1_PEAK_IRQHandler

	.thumb_func
	.weak	FRT1_ZERO_IRQHandler
	.type	FRT1_ZERO_IRQHandler, %function
FRT1_ZERO_IRQHandler:
	b	FRT1_ZERO_IRQHandler

	.thumb_func
	.weak	ICU1_IRQHandler
	.type	ICU1_IRQHandler, %function
ICU1_IRQHandler:
	b	ICU1_IRQHandler

	.thumb_func
	.weak	OCU1_IRQHandler
	.type	OCU1_IRQHandler, %function
OCU1_IRQHandler:
	b	OCU1_IRQHandler

	.thumb_func
	.weak	FRT2_PEAK_IRQHandler
	.type	FRT2_PEAK_IRQHandler, %function
FRT2_PEAK_IRQHandler:
	b	FRT2_PEAK_IRQHandler

	.thumb_func
	.weak	FRT2_ZERO_IRQHandler
	.type	FRT2_ZERO_IRQHandler, %function
FRT2_ZERO_IRQHandler:
	b	FRT2_ZERO_IRQHandler

	.thumb_func
	.weak	ICU2_IRQHandler
	.type	ICU2_IRQHandler	, %function
ICU2_IRQHandler	:
	b	ICU2_IRQHandler

	.thumb_func
	.weak	OCU2_IRQHandler
	.type	OCU2_IRQHandler, %function
OCU2_IRQHandler:
	b	OCU2_IRQHandler

	.thumb_func
	.weak	PPG00_02_04_IRQHandler
	.type	PPG00_02_04_IRQHandler, %function
PPG00_02_04_IRQHandler:
	b	PPG00_02_04_IRQHandler

	.thumb_func
	.weak	PPG08_10_12_IRQHandler
	.type	PPG08_10_12_IRQHandler, %function
PPG08_10_12_IRQHandler:
	b	PPG08_10_12_IRQHandler

	.thumb_func
	.weak	PPG16_18_20_IRQHandler
	.type	PPG16_18_20_IRQHandler, %function
PPG16_18_20_IRQHandler:
	b	PPG16_18_20_IRQHandler

	.thumb_func
	.weak	BT0_IRQHandler
	.type	BT0_IRQHandler, %function
BT0_IRQHandler:
	b	BT0_IRQHandler

	.thumb_func
	.weak	BT1_IRQHandler
	.type	BT1_IRQHandler, %function
BT1_IRQHandler:
	b	BT1_IRQHandler

	.thumb_func
	.weak	BT2_IRQHandler
	.type	BT2_IRQHandler, %function
BT2_IRQHandler:
	b	BT2_IRQHandler

	.thumb_func
	.weak	BT3_IRQHandler
	.type	BT3_IRQHandler, %function
BT3_IRQHandler:
	b	BT3_IRQHandler

	.thumb_func
	.weak	BT4_IRQHandler
	.type	BT4_IRQHandler, %function
BT4_IRQHandler:
	b	BT4_IRQHandler

	.thumb_func
	.weak	BT5_IRQHandler
	.type	BT5_IRQHandler, %function
BT5_IRQHandler:
	b	BT5_IRQHandler

	.thumb_func
	.weak	BT6_IRQHandler
	.type	BT6_IRQHandler, %function
BT6_IRQHandler:
	b	BT6_IRQHandler

	.thumb_func
	.weak	BT7_IRQHandler
	.type	BT7_IRQHandler, %function
BT7_IRQHandler:
	b	BT7_IRQHandler

	.thumb_func
	.weak	DT1_2_IRAHandler
	.type	DT1_2_IRAHandler, %function
DT1_2_IRAHandler:
	b	DT1_2_IRAHandler

	.thumb_func
	.weak	WC_IRQHandler
	.type	WC_IRQHandler, %function
WC_IRQHandler:
	b	WC_IRQHandler

	.thumb_func
	.weak	EXTBUS_ERR_Handler
	.type	EXTBUS_ERR_Handler, %function
EXTBUS_ERR_Handler:
	b	EXTBUS_ERR_Handler

	.thumb_func
	.weak	RTC_IRQHandler
	.type	RTC_IRQHandler, %function
RTC_IRQHandler:
	b	RTC_IRQHandler

	.thumb_func
	.weak	EXTINT8_IRQHandler
	.type	EXTINT8_IRQHandler, %function
EXTINT8_IRQHandler:
	b	EXTINT8_IRQHandler

	.thumb_func
	.weak	EXTINT9_IRQHandler
	.type	EXTINT9_IRQHandler, %function
EXTINT9_IRQHandler:
	b	EXTINT9_IRQHandler

	.thumb_func
	.weak	EXTINT10_IRQHandler
	.type	EXTINT10_IRQHandler, %function
EXTINT10_IRQHandler:
	b	EXTINT10_IRQHandler

	.thumb_func
	.weak	EXTINT11_IRQHandler
	.type	EXTINT11_IRQHandler, %function
EXTINT11_IRQHandler:
	b	EXTINT11_IRQHandler

	.thumb_func
	.weak	EXTINT12_IRQHandler
	.type	EXTINT12_IRQHandler, %function
EXTINT12_IRQHandler:
	b	EXTINT12_IRQHandler

	.thumb_func
	.weak	EXTINT13_IRQHandler
	.type	EXTINT13_IRQHandler, %function
EXTINT13_IRQHandler:
	b	EXTINT13_IRQHandler

	.thumb_func
	.weak	EXTINT14_IRQHandler
	.type	EXTINT14_IRQHandler, %function
EXTINT14_IRQHandler:
	b	EXTINT14_IRQHandler

	.thumb_func
	.weak	EXTINT15_IRQHandler
	.type	EXTINT15_IRQHandler, %function
EXTINT15_IRQHandler:
	b	EXTINT15_IRQHandler

	.thumb_func
	.weak	TIM_IRQHandler
	.type	TIM_IRQHandler, %function
TIM_IRQHandler:
	b	TIM_IRQHandler

	.thumb_func
	.weak	MFS0_RX_IRQHandler
	.type	MFS0_RX_IRQHandler, %function
MFS0_RX_IRQHandler:
	b	MFS0_RX_IRQHandler

	.thumb_func
	.weak	MFS0_TX_IRQHandler
	.type	MFS0_TX_IRQHandler, %function
MFS0_TX_IRQHandler:
	b	MFS0_TX_IRQHandler

	.thumb_func
	.weak	MFS1_RX_IRQHandler
	.type	MFS1_RX_IRQHandler, %function
MFS1_RX_IRQHandler:
	b	MFS1_RX_IRQHandler

	.thumb_func
	.weak	MFS1_TX_IRQHandler
	.type	MFS1_TX_IRQHandler, %function
MFS1_TX_IRQHandler:
	b	MFS1_TX_IRQHandler

	.thumb_func
	.weak	MFS2_RX_IRQHandler
	.type	MFS2_RX_IRQHandler, %function
MFS2_RX_IRQHandler:
	b	MFS2_RX_IRQHandler

	.thumb_func
	.weak	MFS2_TX_IRQHandler
	.type	MFS2_TX_IRQHandler, %function
MFS2_TX_IRQHandler:
	b	MFS2_TX_IRQHandler

	.thumb_func
	.weak	MFS3_RX_IRQHandler
	.type	MFS3_RX_IRQHandler, %function
MFS3_RX_IRQHandler:
	b	MFS3_RX_IRQHandler

	.thumb_func
	.weak	MFS3_TX_IRQHandler
	.type	MFS3_TX_IRQHandler, %function
MFS3_TX_IRQHandler:
	b	MFS3_TX_IRQHandler

	.thumb_func
	.weak	MFS4_RX_IRQHandler
	.type	MFS4_RX_IRQHandler, %function
MFS4_RX_IRQHandler:
	b	MFS4_RX_IRQHandler

	.thumb_func
	.weak	MFS4_TX_IRQHandler
	.type	MFS4_TX_IRQHandler, %function
MFS4_TX_IRQHandler:
	b	MFS4_TX_IRQHandler

	.thumb_func
	.weak	MFS5_RX_IRQHandler
	.type	MFS5_RX_IRQHandler, %function
MFS5_RX_IRQHandler:
	b	MFS5_RX_IRQHandler

	.thumb_func
	.weak	MFS5_TX_IRQHandler
	.type	MFS5_TX_IRQHandler, %function
MFS5_TX_IRQHandler:
	b	MFS5_TX_IRQHandler

	.thumb_func
	.weak	MFS6_RX_IRQHandler
	.type	MFS6_RX_IRQHandler, %function
MFS6_RX_IRQHandler:
	b	MFS6_RX_IRQHandler

	.thumb_func
	.weak	MFS6_TX_IRQHandler
	.type	MFS6_TX_IRQHandler, %function
MFS6_TX_IRQHandler:
	b	MFS6_TX_IRQHandler

	.thumb_func
	.weak	MFS7_RX_IRQHandler
	.type	MFS7_RX_IRQHandler, %function
MFS7_RX_IRQHandler:
	b	MFS7_RX_IRQHandler

	.thumb_func
	.weak	MFS7_TX_IRQHandler
	.type	MFS7_TX_IRQHandler, %function
MFS7_TX_IRQHandler:
	b	MFS7_TX_IRQHandler

	.thumb_func
	.weak	ADC0_IRQHandler
	.type	ADC0_IRQHandler, %function
ADC0_IRQHandler:
	b	ADC0_IRQHandler

	.thumb_func
	.weak	ADC1_IRQHandler
	.type	ADC1_IRQHandler	, %function
ADC1_IRQHandler	:
	b	ADC1_IRQHandler

	.thumb_func
	.weak	USB0_IRQHandler
	.type	USB0_IRQHandler, %function
USB0_IRQHandler:
	b	USB0_IRQHandler

	.thumb_func
	.weak	USB0_HOST_IRQHandler
	.type	USB0_HOST_IRQHandler, %function
USB0_HOST_IRQHandler:
	b	USB0_HOST_IRQHandler

	.thumb_func
	.weak	CAN0_IRQHandler
	.type	CAN0_IRQHandler, %function
CAN0_IRQHandler:
	b	CAN0_IRQHandler

	.thumb_func
	.weak	CAN1_IRQHandler
	.type	CAN1_IRQHandler, %function
CAN1_IRQHandler:
	b	CAN1_IRQHandler

	.thumb_func
	.weak	ETHER0_IRQHandler
	.type	ETHER0_IRQHandler, %function
ETHER0_IRQHandler:
	b	ETHER0_IRQHandler

	.thumb_func
	.weak	DMAC0_IRQHandler
	.type	DMAC0_IRQHandler, %function
DMAC0_IRQHandler:
	b	DMAC0_IRQHandler

	.thumb_func
	.weak	DMAC1_IRQHandler
	.type	DMAC1_IRQHandler, %function
DMAC1_IRQHandler:
	b	DMAC1_IRQHandler

	.thumb_func
	.weak	DMAC2_IRQHandler
	.type	DMAC2_IRQHandler, %function
DMAC2_IRQHandler:
	b	DMAC2_IRQHandler

	.thumb_func
	.weak	DMAC3_IRQHandler
	.type	DMAC3_IRQHandler, %function
DMAC3_IRQHandler:
	b	DMAC3_IRQHandler

	.thumb_func
	.weak	DMAC4_IRQHandler
	.type	DMAC4_IRQHandler, %function
DMAC4_IRQHandler:
	b	DMAC4_IRQHandler

	.thumb_func
	.weak	DMAC5_IRQHandler
	.type	DMAC5_IRQHandler, %function
DMAC5_IRQHandler:
	b	DMAC5_IRQHandler

	.thumb_func
	.weak	DMAC6_IRQHandler
	.type	DMAC6_IRQHandler, %function
DMAC6_IRQHandler:
	b	DMAC6_IRQHandler

	.thumb_func
	.weak	DMAC7_IRQHandler
	.type	DMAC7_IRQHandler, %function
DMAC7_IRQHandler:
	b	DMAC7_IRQHandler

	.thumb_func
	.weak	DSTC_IRQHandler
	.type	DSTC_IRQHandler, %function
DSTC_IRQHandler:
	b	DSTC_IRQHandler

	.thumb_func
	.weak	EXINT16_17_18_19_IRQHandler
	.type	EXINT16_17_18_19_IRQHandler, %function
EXINT16_17_18_19_IRQHandler:
	b	EXINT16_17_18_19_IRQHandler

	.thumb_func
	.weak	EXINT20_21_22_23_IRQHandler
	.type	EXINT20_21_22_23_IRQHandler, %function
EXINT20_21_22_23_IRQHandler:
	b	EXINT20_21_22_23_IRQHandler

	.thumb_func
	.weak	EXINT24_25_26_27_IRQHandler
	.type	EXINT24_25_26_27_IRQHandler, %function
EXINT24_25_26_27_IRQHandler:
	b	EXINT24_25_26_27_IRQHandler

	.thumb_func
	.weak	EXINT28_29_30_31_IRQHandler
	.type	EXINT28_29_30_31_IRQHandler, %function
EXINT28_29_30_31_IRQHandler:
	b	EXINT28_29_30_31_IRQHandler

	.thumb_func
	.weak	QPRC2_IRQHandler
	.type	QPRC2_IRQHandler, %function
QPRC2_IRQHandler:
	b	QPRC2_IRQHandler

	.thumb_func
	.weak	QPRC3_IRQHandler
	.type	QPRC3_IRQHandler, %function
QPRC3_IRQHandler:
	b	QPRC3_IRQHandler

	.thumb_func
	.weak	BT8_IRQHandler
	.type	BT8_IRQHandler, %function
BT8_IRQHandler:
	b	BT8_IRQHandler

	.thumb_func
	.weak	BT9_IRQHandler
	.type	BT9_IRQHandler, %function
BT9_IRQHandler:
	b	BT9_IRQHandler

	.thumb_func
	.weak	BT10_IRQHandler
	.type	BT10_IRQHandler	, %function
BT10_IRQHandler	:
	b	BT10_IRQHandler

	.thumb_func
	.weak	BT11_IRQHandler
	.type	BT11_IRQHandler, %function
BT11_IRQHandler:
	b	BT11_IRQHandler

	.thumb_func
	.weak	BT12_15_IRQHandler
	.type	BT12_15_IRQHandler, %function
BT12_15_IRQHandler:
	b	BT12_15_IRQHandler

	.thumb_func
	.weak	MFS8_RX_IRQHandler
	.type	MFS8_RX_IRQHandler, %function
MFS8_RX_IRQHandler:
	b	MFS8_RX_IRQHandler

	.thumb_func
	.weak	MFS8_TX_IRQHandler
	.type	MFS8_TX_IRQHandler, %function
MFS8_TX_IRQHandler:
	b	MFS8_TX_IRQHandler

	.thumb_func
	.weak	MFS9_RX_IRQHandler
	.type	MFS9_RX_IRQHandler, %function
MFS9_RX_IRQHandler:
	b	MFS9_RX_IRQHandler

	.thumb_func
	.weak	MFS9_TX_IRQHandler
	.type	MFS9_TX_IRQHandler, %function
MFS9_TX_IRQHandler:
	b	MFS9_TX_IRQHandler

	.thumb_func
	.weak	MFS10_RX_IRQHandler
	.type	MFS10_RX_IRQHandler, %function
MFS10_RX_IRQHandler:
	b	MFS10_RX_IRQHandler

	.thumb_func
	.weak	MFS10_TX_IRQHandler
	.type	MFS10_TX_IRQHandler, %function
MFS10_TX_IRQHandler:
	b	MFS10_TX_IRQHandler

	.thumb_func
	.weak	MFS11_RX_IRQHandler
	.type	MFS11_RX_IRQHandler, %function
MFS11_RX_IRQHandler:
	b	MFS11_RX_IRQHandler

	.thumb_func
	.weak	MFS11_TX_IRQHandler
	.type	MFS11_TX_IRQHandler, %function
MFS11_TX_IRQHandler:
	b	MFS11_TX_IRQHandler

	.thumb_func
	.weak	ADC2_IRQHandler
	.type	ADC2_IRQHandler, %function
ADC2_IRQHandler:
	b	ADC2_IRQHandler

	.thumb_func
	.weak	Dummy
	.type	Dummy, %function
Dummy:
	b	Dummy

	.thumb_func
	.weak	USB1_IRQHandler
	.type	USB1_IRQHandler, %function
USB1_IRQHandler:
	b	USB1_IRQHandler

	.thumb_func
	.weak	USB1_HOST_IRQHandler
	.type	USB1_HOST_IRQHandler, %function
USB1_HOST_IRQHandler:
	b	USB1_HOST_IRQHandler

;	.thumb_func
;	.weak	Dummy115
;	.type	Dummy115, %function
;Dummy115:
;	b	Dummy115

	.thumb_func
	.weak	SD_IRQHandler
	.type	SD_IRQHandler, %function
SD_IRQHandler:
	b	SD_IRQHandler

	.thumb_func
	.weak	FLASHIF_IRQHandler
	.type	FLASHIF_IRQHandler, %function
FLASHIF_IRQHandler:
	b	FLASHIF_IRQHandler

	.thumb_func
	.weak	MFS12_RX_IRQHandler
	.type	MFS12_RX_IRQHandler, %function
MFS12_RX_IRQHandler:
	b	MFS12_RX_IRQHandler

	.thumb_func
	.weak	MFS12_TX_IRQHandler
	.type	MFS12_TX_IRQHandler, %function
MFS12_TX_IRQHandler:
	b	MFS12_TX_IRQHandler

	.thumb_func
	.weak	MFS13_RX_IRQHandler
	.type	MFS13_RX_IRQHandler, %function
MFS13_RX_IRQHandler:
	b	MFS13_RX_IRQHandler

	.thumb_func
	.weak	MFS13_TX_IRQHandler
	.type	MFS13_TX_IRQHandler, %function
MFS13_TX_IRQHandler:
	b	MFS13_TX_IRQHandler

	.thumb_func
	.weak	MFS14_RX_IRQHandler
	.type	MFS14_RX_IRQHandler, %function
MFS14_RX_IRQHandler:
	b	MFS14_RX_IRQHandler

	.thumb_func
	.weak	MFS14_TX_IRQHandler
	.type	MFS14_TX_IRQHandler, %function
MFS14_TX_IRQHandler:
	b	MFS14_TX_IRQHandler

	.thumb_func
	.weak	MFS15_RX_IRQHandler
	.type	MFS15_RX_IRQHandler, %function
MFS15_RX_IRQHandler:
	b	MFS15_RX_IRQHandler

	.thumb_func
	.weak	MFS15_TX_IRQHandler
	.type	MFS15_TX_IRQHandler, %function
MFS15_TX_IRQHandler:
	b	MFS15_TX_IRQHandler

    .end
