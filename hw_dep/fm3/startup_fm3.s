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

	.word	CSV_Handler			@ 0: Clock Super Visor
	.word	SWDT_Handler		@ 1: Software Watchdog Timer
	.word	LVD_Handler			@ 2: Low Voltage Detector
	.word	MFT_WG_IRQHandler	@ 3: Wave Form Generator / DTIF
	.word	INT0_7_Handler		@ 4: External Interrupt Request ch.0 to ch.7
	.word	INT8_31_Handler		@ 5: External Interrupt Request ch.8 to ch.31
	.word	DT_Handler			@ 6: Dual Timer / Quad Decoder
	.word	MFS0RX_IRQHandler	@ 7: MultiFunction Serial ch.0
	.word	MFS0TX_IRQHandler	@ 8: MultiFunction Serial ch.0
	.word	MFS1RX_IRQHandler	@ 9: MultiFunction Serial ch.1
	.word	MFS1TX_IRQHandler	@ 10: MultiFunction Serial ch.1
	.word	MFS2RX_IRQHandler	@ 11: MultiFunction Serial ch.2
	.word	MFS2TX_IRQHandler	@ 12: MultiFunction Serial ch.2
	.word	MFS3RX_IRQHandler	@ 13: MultiFunction Serial ch.3
	.word	MFS3TX_IRQHandler	@ 14: MultiFunction Serial ch.3
	.word	MFS4RX_IRQHandler	@ 15: MultiFunction Serial ch.4
	.word	MFS4TX_IRQHandler	@ 16: MultiFunction Serial ch.4
	.word	MFS5RX_IRQHandler	@ 17: MultiFunction Serial ch.5
	.word	MFS5TX_IRQHandler	@ 18: MultiFunction Serial ch.5
	.word	MFS6RX_IRQHandler	@ 19: MultiFunction Serial ch.6
	.word	MFS6TX_IRQHandler	@ 20: MultiFunction Serial ch.6
	.word	MFS7RX_IRQHandler	@ 21: MultiFunction Serial ch.7
	.word	MFS7TX_IRQHandler	@ 22: MultiFunction Serial ch.7
	.word	PPG_Handler			@ 23: PPG
	.word	TIM_IRQHandler		@ 24: OSC / PLL / Watch Counter
	.word	ADC0_IRQHandler		@ 25: ADC0
	.word	ADC1_IRQHandler		@ 26: ADC1
	.word	ADC2_IRQHandler		@ 27: ADC2
	.word	MFT_FRT_IRQHandler	@ 28: Free-run Timer
	.word	MFT_IPC_IRQHandler	@ 29: Input Capture
	.word	MFT_OPC_IRQHandler	@ 30: Output Compare
	.word	BT_IRQHandler		@ 31: Base Timer ch.0 to ch.7
@	.word	CAN0_IRQHandler		@ 32: CAN ch.0
@	.word	CAN1_IRQHandler		@ 33: CAN ch.1
	.word	ETH0_IRQHandler		@ 32: ETH ch.0
	.word	ETH1_IRQHandler		@ 33: ETH ch.1
	.word	USBF0_Handler		@ 34: USB Function
	.word	USB0_Handler		@ 35: USB Function / USB HOST
	.word	USBF1_Handler		@ 36: USB Function
	.word	USB1_Handler		@ 37: USB Function / USB HOST
	.word	DMAC0_Handler		@ 38: DMAC ch.0
	.word	DMAC1_Handler		@ 39: DMAC ch.1
	.word	DMAC2_Handler		@ 40: DMAC ch.2
	.word	DMAC3_Handler		@ 41: DMAC ch.3
	.word	DMAC4_Handler		@ 42: DMAC ch.4
	.word	DMAC5_Handler		@ 43: DMAC ch.5
	.word	DMAC6_Handler		@ 44: DMAC ch.6
	.word	DMAC7_Handler		@ 45: DMAC ch.7
	.word	DummyHandler		@ 46: Reserved
	.word	DummyHandler		@ 47: Reserved

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
	.weak	CSV_Handler
	.type	CSV_Handler, %function
CSV_Handler:
	b	CSV_Handler

	.thumb_func
	.weak	SWDT_Handler
	.type	SWDT_Handler, %function
SWDT_Handler:
	b	SWDT_Handler

	.thumb_func
	.weak	LVD_Handler
	.type	LVD_Handler, %function
LVD_Handler:
	b	LVD_Handler

	.thumb_func
	.weak	MFT_WG_IRQHandler
	.type	MFT_WG_IRQHandler, %function
MFT_WG_IRQHandler:
	b	MFT_WG_IRQHandler

	.thumb_func
	.weak	INT0_7_Handler
	.type	INT0_7_Handler, %function
INT0_7_Handler:
	b	INT0_7_Handler

	.thumb_func
	.weak	INT8_31_Handler
	.type	INT8_31_Handler, %function
INT8_31_Handler:
	b	INT8_31_Handler

	.thumb_func
	.weak	DT_Handler
	.type	DT_Handler, %function
DT_Handler:
	b	DT_Handler

	.thumb_func
	.weak	MFS0RX_IRQHandler
	.type	MFS0RX_IRQHandler, %function
MFS0RX_IRQHandler:
	b	MFS0RX_IRQHandler

	.thumb_func
	.weak	MFS0TX_IRQHandler
	.type	MFS0TX_IRQHandler, %function
MFS0TX_IRQHandler:
	b	MFS0TX_IRQHandler

	.thumb_func
	.weak	MFS1RX_IRQHandler
	.type	MFS1RX_IRQHandler, %function
MFS1RX_IRQHandler:
	b	MFS1RX_IRQHandler

	.thumb_func
	.weak	MFS1TX_IRQHandler
	.type	MFS1TX_IRQHandler, %function
MFS1TX_IRQHandler:
	b	MFS1TX_IRQHandler

	.thumb_func
	.weak	MFS2RX_IRQHandler
	.type	MFS2RX_IRQHandler, %function
MFS2RX_IRQHandler:
	b	MFS2RX_IRQHandler

	.thumb_func
	.weak	MFS2TX_IRQHandler
	.type	MFS2TX_IRQHandler, %function
MFS2TX_IRQHandler:
	b	MFS2TX_IRQHandler

	.thumb_func
	.weak	MFS3RX_IRQHandler
	.type	MFS3RX_IRQHandler, %function
MFS3RX_IRQHandler:
	b	MFS3RX_IRQHandler

	.thumb_func
	.weak	MFS3TX_IRQHandler
	.type	MFS3TX_IRQHandler, %function
MFS3TX_IRQHandler:
	b	MFS3TX_IRQHandler

	.thumb_func
	.weak	MFS4RX_IRQHandler
	.type	MFS4RX_IRQHandler, %function
MFS4RX_IRQHandler:
	b	MFS4RX_IRQHandler

	.thumb_func
	.weak	MFS4TX_IRQHandler
	.type	MFS4TX_IRQHandler, %function
MFS4TX_IRQHandler:
	b	MFS4TX_IRQHandler

	.thumb_func
	.weak	MFS5RX_IRQHandler
	.type	MFS5RX_IRQHandler, %function
MFS5RX_IRQHandler:
	b	MFS5RX_IRQHandler

	.thumb_func
	.weak	MFS5TX_IRQHandler
	.type	MFS5TX_IRQHandler, %function
MFS5TX_IRQHandler:
	b	MFS5TX_IRQHandler

	.thumb_func
	.weak	MFS6RX_IRQHandler
	.type	MFS6RX_IRQHandler, %function
MFS6RX_IRQHandler:
	b	MFS6RX_IRQHandler

	.thumb_func
	.weak	MFS6TX_IRQHandler
	.type	MFS6TX_IRQHandler, %function
MFS6TX_IRQHandler:
	b	MFS6TX_IRQHandler

	.thumb_func
	.weak	MFS7RX_IRQHandler
	.type	MFS7RX_IRQHandler, %function
MFS7RX_IRQHandler:
	b	MFS7RX_IRQHandler

	.thumb_func
	.weak	MFS7TX_IRQHandler
	.type	MFS7TX_IRQHandler, %function
MFS7TX_IRQHandler:
	b	MFS7TX_IRQHandler

	.thumb_func
	.weak	PPG_Handler
	.type	PPG_Handler, %function
PPG_Handler:
	b	PPG_Handler

	.thumb_func
	.weak	TIM_IRQHandler
	.type	TIM_IRQHandler, %function
TIM_IRQHandler:
	b	TIM_IRQHandler

	.thumb_func
	.weak	ADC0_IRQHandler
	.type	ADC0_IRQHandler, %function
ADC0_IRQHandler:
	b	ADC0_IRQHandler

	.thumb_func
	.weak	ADC1_IRQHandler
	.type	ADC1_IRQHandler, %function
ADC1_IRQHandler:
	b	ADC1_IRQHandler

	.thumb_func
	.weak	ADC2_IRQHandler
	.type	ADC2_IRQHandler, %function
ADC2_IRQHandler:
	b	ADC2_IRQHandler

	.thumb_func
	.weak	MFT_FRT_IRQHandler
	.type	MFT_FRT_IRQHandler, %function
MFT_FRT_IRQHandler:
	b	MFT_FRT_IRQHandler

	.thumb_func
	.weak	MFT_IPC_IRQHandler
	.type	MFT_IPC_IRQHandler, %function
MFT_IPC_IRQHandler:
	b	MFT_IPC_IRQHandler

	.thumb_func
	.weak	MFT_OPC_IRQHandler
	.type	MFT_OPC_IRQHandler, %function
MFT_OPC_IRQHandler:
	b	MFT_OPC_IRQHandler

	.thumb_func
	.weak	BT_IRQHandler
	.type	BT_IRQHandler, %function
BT_IRQHandler:
	b	BT_IRQHandler

@	.thumb_func
@	.type	CAN0_IRQHandler, %function
@CAN0_IRQHandler:
@	b	CAN0_IRQHandler
@
@	.thumb_func
@	.type	CAN1_IRQHandler, %function
@CAN1_IRQHandler:
@	b	CAN1_IRQHandler

	.thumb_func
	.weak	ETH0_IRQHandler
	.type	ETH0_IRQHandler, %function
ETH0_IRQHandler:
	b	ETH0_IRQHandler

	.thumb_func
	.weak	ETH1_IRQHandler
	.type	ETH1_IRQHandler, %function
ETH1_IRQHandler:
	b	ETH1_IRQHandler

	.thumb_func
	.weak	USBF0_Handler
	.type	USBF0_Handler, %function
USBF0_Handler:
	b	USBF0_Handler

	.thumb_func
	.weak	USB0_Handler
	.type	USB0_Handler, %function
USB0_Handler:
	b	USB0_Handler

	.thumb_func
	.weak	USBF1_Handler
	.type	USBF1_Handler, %function
USBF1_Handler:
	b	USBF1_Handler

	.thumb_func
	.weak	USB1_Handler
	.type	USB1_Handler, %function
USB1_Handler:
	b	USB1_Handler

	.thumb_func
	.weak	DMAC0_Handler
	.type	DMAC0_Handler, %function
DMAC0_Handler:
	b	DMAC0_Handler

	.thumb_func
	.weak	DMAC1_Handler
	.type	DMAC1_Handler, %function
DMAC1_Handler:
	b	DMAC1_Handler

	.thumb_func
	.weak	DMAC2_Handler
	.type	DMAC2_Handler, %function
DMAC2_Handler:
	b	DMAC2_Handler

	.thumb_func
	.weak	DMAC3_Handler
	.type	DMAC3_Handler, %function
DMAC3_Handler:
	b	DMAC3_Handler

	.thumb_func
	.weak	DMAC4_Handler
	.type	DMAC4_Handler, %function
DMAC4_Handler:
	b	DMAC4_Handler

	.thumb_func
	.weak	DMAC5_Handler
	.type	DMAC5_Handler, %function
DMAC5_Handler:
	b	DMAC5_Handler

	.thumb_func
	.weak	DMAC6_Handler
	.type	DMAC6_Handler, %function
DMAC6_Handler:
	b	DMAC6_Handler

	.thumb_func
	.weak	DMAC7_Handler
	.type	DMAC7_Handler, %function
DMAC7_Handler:
	b	DMAC7_Handler

	.thumb_func
	.weak	DummyHandler
	.type	DummyHandler, %function
DummyHandler:
	b	DummyHandler

    .end
