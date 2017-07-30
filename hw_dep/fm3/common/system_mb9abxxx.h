/******************************************************************************
					USE AGREEMENT

By using the program contain in this document, file location or subdirectory
 (the "Deliverables"), you ("Licensee" are deemed to have agreed to the terms and conditions
set forth below (the "AGREEMENT") with Fujitsu Semiconductor Limited ("FUJITSU"). If Licensee
does not agree to this AGREEMENT then Licensee shall not use the Deliverables. 

1. Permitted Use

The Deliverables are intended for and must only be used for reference and in an evaluation
laboratory environment. The Deliverables are provided on an "AS-IS" basis without any charge
and is subject to this AGREEMENT.  FUJITSU may modify the Deliverables without notice to
Licensee.  It is Licensee's obligation to fully test the Deliverables in its environment and
to ensure proper functionality, qualification and compliance with component specifications.
In the event the Deliverables include open source components, the provisions of the governing
open source license agreement shall apply with respect to such Deliverables, and such open
source license agreement shall prevail over this AGREEMNT if there is any contradiction
between this AGREEMENT and such open source license agreement.

2. Disclaimer on Use

FUJITSU MAKES NO WARRANTIES, EITHER EXPRESS OR IMPLIED, REGARDING THE DELIVERABLES, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY PARTICULAR PURPOSE. 

FUJITSU DOES NOT WARRANT THAT THE DELIVERABLES DO NOT INFRINGE ANY THIRD PARTY’S INTELLECTUAL
PROPERTY RIGHTS ("IPRs"), INCLUDING BUT NOT LIMITED TO PATENTS, UTILITY MODELS, DESIGNS,
TRADEMARKS, TRADE SECRETS, COPYRIGHTS, AND MASK WORK RIGHTS.

In the event that the Deliverables infringe a third party's IPRs, it is the sole
responsibility of the LICENSEE to obtain necessary licenses to continue the usage of the
Deliverable and to pay any and all damages for its use of the Deliverables.

FUJITSU SHALL NOT BE OBLIGATED TO MAKE ANY BUG-FIXES, UPDATES, OR MODIFICATIONS OR TO PROVIDE
SUPPORT WITH RESPECT TO THE DELIVERABLES. 

Except as otherwise expressly provided herein, nothing in this AGREEMENT shall be construed
as obligating FUJITSU to grant licenses for the Deliverables to LICENSEE.

3. LIMITATION OF LIABILITY

THE DELIVERABLES ARE PROVIDED "AS-IS" FOR FREE AND IN NO EVENT WILL FUJITSU BE LIABLE FOR
LICENSEE’S USE OF THE DELIVERABLES. 

IN NO EVENT WILL FUJITSU BE LIABLE TO LICENSEE, CUSTOMERS, END USERS, OR ANY THIRD PARTIES FOR
ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL, PUNITIVE, CONSEQUENTIAL OR SIMILAR DAMAGE OF ANY
KIND OR CHARACTER, WHETHER IN CONTRACT OR TORT, ARISING OUT OF OR RELATING TO THIS AGREEMENT,
INCLUDING WITHOUT LIMITATION, LOST REVENUE OR PROFITS, LOST DATA OR USE, DAMAGE TO EQUIPMENT,
DOWNTIME COSTS AND CLAIMS BY ANY THIRD PARTY, REGARDLESS OF WHETHER FUJITSU HAS BEEN ADVISED
OF THE POSSIBILITY OF SUCH DAMAGES AND NOTWITHSTANDING THE FAILURE OF ANY REMEDY FOR AN
ESSENTIAL PURPOSE.

ALL LIABILITIES OF FUJITSUWHETHER IN CONTRACT, TORT, OR OTHERWISE, ARISING OUT OF OR IN
CONNECTION WITH THIS AGREEMENT, SHALL NOT EXCEED THE TOTAL AMOUNTS PAID TO FUJITSU BY
LICENSEE FOR THE DELIVERABLES UNDER THIS AGREEMENT. 

4. GOVERNING LAW AND ARBITRATION

It is expressly agreed that the validity and construction of this AGREEMENT, and performance
hereunder, shall be governed by the laws of Japan, without regard to its conflicts of laws
rules.  THE UNITED NATIONS CONVENTION ON CONTRACTS FOR THE INTERNATIONAL SALE OF GOODS AND
UNIFORM COMPUTER INFORMATION TRANSACTION ACT (UCITA) SHALL NOT APPLY.

In the event of any dispute, controversy or claim (collectively referred to as "Dispute")
arising out of or relating to this AGREEMENT, or the breach thereof, the parties shall use
their best endeavors to amicably settle any such disputes, controversies or within thirty (30)
days a party receives notice of a Dispute from the other party ("Complaining Party"). Any such
disputes, controversies or claim that cannot be settled between the parties within thirty (30)
days after a party received notice of the Dispute from Complaining Party, such Dispute shall
be finally settled under the Rules of Arbitration of the Japan Commercial Arbitration
Association ("Rules") by three arbitrators appointed in accordance with the said Rules.
The place of arbitration shall be Tokyo, Japan. The arbitration shall be conducted in English,
with Japanese translation used at the request of either party. The award rendered by the
arbitrators shall be final and binding upon both parties.

In addition to the remedies that parties may seek from an arbitration tribunal, each party
shall have the right to obtain, from a court of competent jurisdiction at any time, any and
all forms of equitable relief to prevent the infringement of an intellectual property right of
a party, including but not limited to injunctive relief and specific performance.  The seeking
of and/or the obtaining of such equitable relief will not be deemed to be incompatible with
the agreement to arbitrate.

5. Indemnity 

Licensee will hold harmless and indemnify FUJITSU and its affiliates, officers, agents, and
employees from any claim, suit or action arising from or related to the use of the
Deliverables or violation of the terms of this Agreement, including any liability or expense
arising from claims, losses, damages, suits, judgments, litigation costs and attorneys' fees.

6. General  

FUJITSU may modify this AGREEMENT and such modification will become effective 14 days after
the date FUJITSU provides notice of such modification to Licensee. If Licensee does not want
to accept any of such modifications then Licensee should discontinue the use of Deliverables
before the modification becomes effective.  If there is a conflict between the terms of this
AGREEMENT and the modified terms as made by FUJITSU, then the modified terms will control
for that conflict.

This AGREEMENT does not create any third party beneficiary rights. 

FUJITSU If you do not comply with these terms, and we don't take action right away, this
doesn't mean that we are giving up any rights that we may have (such as taking action in the
future). 

Licensee shall comply with all then-current applicable laws, regulations and other legal
requirements in its performance in connection with this AGREEMENT, including without
limitation, all applicable export control laws, rules and regulations of the United States,
Japan and any other relevant countries.

If any proper arbitration tribunal or court determines that a term in this AGREEMENT term is
not enforceable, then this determination will not affect any other terms in this AGREEMENT. 

The failure by FUJITSU to enforce or exercise any rights under or in connection with any term
and/or condition of this AGREEMENT shall not be construed as constituting a waiver of such
rights.
 ******************************************************************************/
/** \file system_mb9abxxx.h
 **
 ** Headerfile for FM4 system parameters
 **
 ** History: 
 ** 2013-01-21  0.1  MWi  AI: Unification to be done
 ** 2013-02-06  0.2  MWi  CMSIS coding restored; unifying still to be done
 ** 2013-06-28  0.3  EH   Added Trace Buffer enable definition
 ******************************************************************************/

#ifndef _SYSTEM_MB9ABXXX_H_
#define _SYSTEM_MB9ABXXX_H_

#ifdef __cplusplus
extern "C" {
#endif 

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include <stdint.h>

/******************************************************************************/
/* Global pre-processor symbols/macros ('define')                             */
/******************************************************************************/
   
/******************************************************************************/
/*                                                                            */
/*                      START OF USER SETTINGS HERE                           */
/*                      ===========================                           */   
/*                                                                            */
/*                 All lines with '<<<' can be set by user.                   */
/*                                                                            */
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief Clock Setup Enable
 **        <i>(USER SETTING)</i>
 **
 ** - 0 = No clock setup done by system_mb9xfxxx.c
 ** - 1 = Clock setup done by system_mb9xfxxx.c
 ******************************************************************************/ 
#define CLOCK_SETUP               1   // <<< Define clock setup here

/**
 ******************************************************************************
 ** \brief External Main Clock Frequency (in Hz, [value]ul)
 **        <i>(USER SETTING)</i>
 ******************************************************************************/    
#define __CLKMO        ( 4000000ul)   // <<< External   4MHz Crystal

/**
 ******************************************************************************
 ** \brief External Sub Clock Frequency (in Hz, [value]ul)
 **        <i>(USER SETTING)</i>
 ******************************************************************************/  
#define __CLKSO        (   32768ul)   // <<<  External  32KHz Crystal

/**
 ******************************************************************************
 ** \brief System Clock Mode Control Register value definition
 **        <i>(USER SETTING)</i>
 ** 
 ** SCM_CTL
 **
 ** Bit#7-5 : RCS[2:0]
 ** - 0 = Internal high-speed CR oscillation (default)
 ** - 1 = Main oscillation clock
 ** - 2 = PLL oscillation clock
 ** - 3 = (not allowed)
 ** - 4 = Internal low-speed CR oscillation
 ** - 5 = Sub clock oscillation
 ** - 6 = (not allowed)
 ** - 7 = (not allowed)
 **
 ** Bit#4 : PLLE
 ** - 0 = Disable PLL (default)
 ** - 1 = Enable PLL
 **
 ** Bit#3 : SOSCE
 ** - 0 = Disable sub oscillation (default)
 ** - 1 = Enable sub oscillation
 **
 ** Bit#2 : (reserved)
 **
 ** Bit#1 : MOSCE
 ** - 0 = Disable main oscillation (default)
 ** - 1 = Enable main oscillation  
 **
 ** Bit#0 : (reserved)  
 ******************************************************************************/ 
#define SCM_CTL_Val           0x00000052ul    // <<< Define SCM_CTL here

/**
 ******************************************************************************
 ** \brief Base Clock Prescaler Register value definition
 **        <i>(USER SETTING)</i>
 **
 ** BSC_PSR
 **
 ** Bit#7-3 : (reserved)
 **
 ** Bit#2-0 : BSR[2:0]
 ** - 0 = HCLK = Master Clock
 ** - 1 = HCLK = Master Clock / 2
 ** - 2 = HCLK = Master Clock / 3
 ** - 3 = HCLK = Master Clock / 4
 ** - 4 = HCLK = Master Clock / 6
 ** - 5 = HCLK = Master Clock / 8
 ** - 6 = HCLK = Master Clock / 16
 ** - 7 = (reserved)
 ******************************************************************************/    
#define BSC_PSR_Val           0x00000000ul    // <<< Define BSC_PSR here
 
/**
 ******************************************************************************
 ** \brief APB0 Prescaler Register value definition
 **        <i>(USER SETTING)</i>
 ** 
 ** APBC0_PSR
 **
 ** Bit#7-2 : (reserved)
 **
 ** Bit#1-0 : BSR[2:0] 
 ** - 0 = PCLK0 = HCLK
 ** - 1 = PCLK0 = HCLK / 2
 ** - 2 = PCLK0 = HCLK / 4
 ** - 3 = PCLK0 = HCLK / 8
 ******************************************************************************/    
#define APBC0_PSR_Val         0x00000001UL     // <<< Define APBC0_PSR here
   
/**
 ******************************************************************************
 ** \brief APB1 Prescaler Register value definition
 **        <i>(USER SETTING)</i>
 ** 
 ** APBC1_PSR
 **
 ** Bit#7 : APBC1EN
 ** - 0 = Disable PCLK1 output
 ** - 1 = Enables PCLK1 (default)
 **
 ** Bit#6-5 : (reserved)
 **
 ** Bit#4 : APBC1RST
 ** - 0 = APB1 bus reset, inactive (default)
 ** - 1 = APB1 bus reset, active  
 **
 ** Bit#3-2 : (reserved)
 **
 ** Bit#1-0 : APBC1[2:0]
 ** - 0 = PCLK1 = HCLK
 ** - 1 = PCLK1 = HCLK / 2
 ** - 2 = PCLK1 = HCLK / 4
 ** - 3 = PCLK1 = HCLK / 8
 ******************************************************************************/     
#define APBC1_PSR_Val         0x00000081ul    // <<< Define APBC1_PSR here
   
/**
 ******************************************************************************
 ** \brief APB2 Prescaler Register value definition
 **        <i>(USER SETTING)</i>
 ** 
 ** APBC2_PSR
 **
 ** Bit#7 : APBC2EN
 ** - 0 = Disable PCLK2 output
 ** - 1 = Enables PCLK2 (default)
 **
 ** Bit#6-5 : (reserved)
 ** 
 ** Bit#4 : APBC2RST
 ** - 0 = APB2 bus reset, inactive (default)
 ** - 1 = APB2 bus reset, active  
 **
 ** Bit#3-2 : (reserved)
 **
 ** Bit#1-0 : APBC2[1:0]
 ** - 0 = PCLK2 = HCLK
 ** - 1 = PCLK2 = HCLK / 2
 ** - 2 = PCLK2 = HCLK / 4
 ** - 3 = PCLK2 = HCLK / 8
 ******************************************************************************/    
#define APBC2_PSR_Val         0x00000081ul    // <<< Define APBC2_PSR here

/**
 ******************************************************************************
 ** \brief Software Watchdog Clock Prescaler Register value definition
 **        <i>(USER SETTING)</i>
 ** 
 ** SWC_PSR
 **
 ** Bit#7 : TESTB
 ** - 0 = (not allowed)
 ** - 1 = (always write "1" to this bit)
 **
 ** Bit#6-2 : (reserved)
 **
 ** Bit#1-0 : SWDS[2:0]
 ** - 0 = SWDGOGCLK = PCLK0
 ** - 1 = SWDGOGCLK = PCLK0 / 2
 ** - 2 = SWDGOGCLK = PCLK0 / 4
 ** - 3 = SWDGOGCLK = PCLK0 / 8
 ******************************************************************************/  
#define SWC_PSR_Val           0x00000003ul    // <<< Define SWC_PSR here

/**
 ******************************************************************************
 ** \brief Trace Clock Prescaler Register value definition
 **        <i>(USER SETTING)</i>
 ** 
 ** TTC_PSR
 **
 ** Bit#7-1 : (reserved)
 **
 ** Bit#0 : TTC
 ** - 0 = TPIUCLK = HCLK
 ** - 1 = TPIUCLK = HCLK / 2
 ******************************************************************************/  
#define TTC_PSR_Val           0x00000000ul    // <<< Define TTC_PSR here
 
/**
 ******************************************************************************
 ** \brief Clock Stabilization Wait Time Register value definition
 **        <i>(USER SETTING)</i>
 ** 
 ** CSW_TMR
 **
 ** Bit#7 : (reserved)
 **
 ** Bit#6-4 : SOWT[2:0]
 ** - 0 = ~10.3 ms (default)
 ** - 1 = ~20.5 ms
 ** - 2 = ~41 ms
 ** - 3 = ~82 ms
 ** - 4 = ~164 ms
 ** - 5 = ~327 ms
 ** - 6 = ~655 ms
 ** - 7 = ~1.31 s 
 **
 ** Bit#3-0 : MOWT[3:0]
 ** - 0 = ~500 ns (default)
 ** - 1 = ~8 us
 ** - 2 = ~16 us
 ** - 3 = ~32 us
 ** - 4 = ~64 us
 ** - 5 = ~128 us
 ** - 6 = ~256 us
 ** - 7 = ~512 us
 ** - 8 = ~1.0 ms
 ** - 9 = ~2.0 ms
 ** - 10 = ~4.0 ms
 ** - 11 = ~8.0 ms
 ** - 12 = ~33.0 ms
 ** - 13 = ~131 ms
 ** - 14 = ~524 ms
 ** - 15 = ~2.0 s
 ******************************************************************************/     
#define CSW_TMR_Val           0x0000005Cul    // <<< Define CSW_TMR here

/**
 ******************************************************************************
 ** \brief PLL Clock Stabilization Wait Time Setup Register value definition
 **        <i>(USER SETTING)</i>
 ** 
 ** PSW_TMR
 **
 ** Bit#7-5 : (reserved)
 **
 ** Bit#4 : PINC
 ** - 0 = Selects CLKMO (main oscillation) (default)
 ** - 1 = (setting diabled)
 **
 ** Bit#3 : (reserved)
 **
 ** Bit#2-0 : POWT[2:0]
 ** - 0 = ~128 us (default) 
 ** - 1 = ~256 us
 ** - 2 = ~512 us
 ** - 3 = ~1.02 ms
 ** - 4 = ~2.05 ms
 ** - 5 = ~4.10 ms
 ** - 6 = ~8.20 ms
 ** - 7 = ~16.40 ms
 ******************************************************************************/    
#define PSW_TMR_Val           0x00000000ul    // <<< Define PSW_TMR here

/**
 ******************************************************************************
 ** \brief PLL Control Register 1 value definition
 **        <i>(USER SETTING)</i>
 ** 
 ** PLL_CTL1
 **
 ** Bit#7-4 : PLLK[3:0]
 ** - 0 = Division(PLLK) = 1/1 (default)
 ** - 1 = Division(PLLK) = 1/2
 ** - 2 = Division(PLLK) = 1/3
 ** - . . .
 ** - 15 = Division(PLLK) = 1/16
 **
 ** Bit#3-0 : PLLM[3:0]
 ** - 0 = Division(PLLM) = 1/1 (default)
 ** - 1 = Division(PLLM) = 1/2
 ** - 2 = Division(PLLM) = 1/3
 ** - . . .
 ** - 15 = Division(PLLM) = 1/16
 ******************************************************************************/    
#define PLL_CTL1_Val          0x00000001ul    // <<< Define PLL_CTL1 here

/**
 ******************************************************************************
 ** \brief PLL Control Register 2 value definition
 **        <i>(USER SETTING)</i>
 ** 
 ** PLL_CTL2
 **
 ** Bit#7-6 : (reserved)
 **
 ** Bit#5-0 : PLLN[5:0]
 ** - 0 = Division(PLLN) = 1/1 (default)
 ** - 1 = Division(PLLN) = 1/2
 ** - 2 = Division(PLLN) = 1/3
 ** - . . .
 ** - 63 = Division(PLLN) = 1/64
 ******************************************************************************/    
#define PLL_CTL2_Val          0x00000027ul    // <<< Define PLL_CTL2 here
   
/**
 ******************************************************************************
 ** \brief Hardware Watchdog disable definition
 **        <i>(USER SETTING)</i>
 **
 ** - 0 = Hardware Watchdog enable
 ** - 1 = Hardware Watchdog disable
 ******************************************************************************/  
#define HWWD_DISABLE          1   // <<< Define HW Watach dog enable here

/**
 ******************************************************************************
 ** \brief Trimming CR
 **        <i>(USER SETTING)</i>
 **
 ** - 0 = CR is not trimmed at startup
 ** - 1 = CR is trimmed at startup
 ******************************************************************************/  
#define CR_TRIM_SETUP         1   // <<< Define CR trimming at startup enable here

/**
 ******************************************************************************
 ** brief Trace Buffer enable definition
 **        <i>(USER SETTING)</i>
 **
 ** - 0 = Trace Buffer disable
 ** - 1 = Trace Buffer enable
 ******************************************************************************/  
#define TRACE_BUFFER_ENABLE   1   // <<< Define Trace Buffer enable here

   
/******************************************************************************/
/*                                                                            */
/*                         END OF USER SETTINGS HERE                          */
/*                         =========================                          */ 
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Device dependent System Clock absolute maximum ranges                      */
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief Internal High-Speed CR Oscillator Frequency (in Hz, [value]ul)
 **        <i>(USER SETTING)</i>
 ******************************************************************************/    
#define __CLKHC        ( 4000000ul)         /* Internal   4MHz CR Oscillator  */
   
/**
 ******************************************************************************
 ** \brief Internal Low-Speed CR Oscillator Frequency (in Hz, [value]ul)
 **        <i>(USER SETTING)</i>
 ******************************************************************************/ 
#define __CLKLC        (  100000ul)         /* Internal 100KHz CR Oscillator  */  

/**
 ******************************************************************************
 ** \brief Any case minimum Main Clock frequency (in Hz, [value]ul)
 **        <i>(DEVICE DEPENDENT SETTING)</i>
 ******************************************************************************/ 
#define __CLKMOMIN    (  4000000ul)
   
/**
 ******************************************************************************
 ** \brief Maximum Main Clock frequency using external clock (in Hz, [value]ul)
 **        <i>(DEVICE DEPENDENT SETTING)</i>
 ******************************************************************************/ 
#define __CLKMOMAX    ( 48000000ul)

/**
 ******************************************************************************
 ** \brief Any case minimum Sub Clock frequency (in Hz, [value]ul)
 **        <i>(DEVICE DEPENDENT SETTING)</i>
 ******************************************************************************/ 
#define __CLKSOMIN    (    32000ul)
   
/**
 ******************************************************************************
 ** \brief Maximum Sub Clock frequency using external clock (in Hz, [value]ul)
 **        <i>(DEVICE DEPENDENT SETTING)</i>
 ******************************************************************************/ 
#define __CLKSOMAX    (   100000ul)
   
/**
 ******************************************************************************
 ** \brief Absolute minimum PLL input frequency (in Hz, [value]ul)
 **        <i>(DEVICE DEPENDENT SETTING)</i>
 ******************************************************************************/ 
#define __PLLCLKINMIN (  4000000ul)
   
/**
 ******************************************************************************
 ** \brief Absolute maximum PLL input frequency (in Hz, [value]ul)
 **        <i>(DEVICE DEPENDENT SETTING)</i>
 ******************************************************************************/ 
#define __PLLCLKINMAX ( 16000000ul)

/**
 ******************************************************************************
 ** \brief Absolute minimum PLL oscillation frequency (in Hz, [value]ul)
 **        <i>(DEVICE DEPENDENT SETTING)</i>
 ******************************************************************************/ 
#define __PLLCLKMIN   (200000000ul)
   
/**
 ******************************************************************************
 ** \brief Absolute maximum PLL oscillation  frequency (in Hz, [value]ul)
 **        <i>(DEVICE DEPENDENT SETTING)</i>
 ******************************************************************************/ 
#define __PLLCLKMAX   (320000000ul)

/**
 ******************************************************************************
 ** \brief Absolute maximum System Clock frequency (HCLK) (in Hz, [value]ul)
 **        <i>(DEVICE DEPENDENT SETTING)</i>
 ******************************************************************************/ 
#define __HCLKMAX     (160000000ul)

/**
 ******************************************************************************
 ** \brief Preprocessor macro for checking range (clock settings).
 **
 ** \return 0    Within range
 ** \return 1    Out of range
 ******************************************************************************/ 
#define CHECK_RANGE(val, min, max)          (((val) < (min)) || ((val) > (max)))
   
/**
 ******************************************************************************
 ** \brief Preprocessor macro for checking bits with mask (clock settings).
 **        Prevents from setting reserved bits by mistake.
 **
 ** \return 0    All bits within mask
 ** \return 1    One or more bits out of mask
 ******************************************************************************/ 
#define CHECK_RSVD(val, mask)                ((val) & (mask))


/******************************************************************************/
/* Check register settings                                                    */
/******************************************************************************/
#if (CHECK_RSVD((SCM_CTL_Val),    ~0x000000FAul))
   #error "SCM_CTL: Invalid values of reserved bits!"
#endif

#if ((SCM_CTL_Val & 0xE0ul) == 0x40ul) && ((SCM_CTL_Val & 0x10ul) != 0x10ul)
   #error "SCM_CTL: CLKPLL is selected but PLL is not enabled!"
#endif

#if (CHECK_RSVD((CSW_TMR_Val),    ~0x0000007Ful))
   #error "CSW_TMR: Invalid values of reserved bits!"
#endif

#if ((SCM_CTL_Val & 0x10ul))       /* if PLL is used */
  #if (CHECK_RSVD((PSW_TMR_Val),  ~0x00000017ul))
     #error "PSW_TMR: Invalid values of reserved bits!"
  #endif

  #if (CHECK_RSVD((PLL_CTL1_Val), ~0x000000FFul))
     #error "PLL_CTL1: Invalid values of reserved bits!"
  #endif

  #if (CHECK_RSVD((PLL_CTL2_Val), ~0x0000003Ful))
    #error "PLL_CTL2: Invalid values of reserved bits!"
  #endif
#endif

#if (CHECK_RSVD((BSC_PSR_Val),    ~0x00000007ul))
  #error "BSC_PSR: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((APBC0_PSR_Val),  ~0x00000003ul))
  #error "APBC0_PSR: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((APBC1_PSR_Val),  ~0x00000093ul))
  #error "APBC1_PSR: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((APBC2_PSR_Val),  ~0x00000093ul))
  #error "APBC2_PSR: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((SWC_PSR_Val),    ~0x00000003ul))
  #error "SWC_PSR: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((TTC_PSR_Val),    ~0x00000003ul))
  #error "TTC_PSR: Invalid values of reserved bits!"
#endif

/******************************************************************************/
/* Define clocks with checking settings                                       */
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief Calculate PLL K factor from settings
 ******************************************************************************/
#define __PLLK         (((PLL_CTL1_Val >> 4ul) & 0x0Ful) + 1ul)

/**
 ******************************************************************************
 ** \brief Calculate PLL N factor from settings
 ******************************************************************************/
#define __PLLN         (((PLL_CTL2_Val     ) & 0x3Ful) + 1ul)

/**
 ******************************************************************************
 ** \brief Calculate PLL M factor from settings
 ******************************************************************************/
#define __PLLM         (((PLL_CTL1_Val     ) & 0x0Ful) + 1ul)

/**
 ******************************************************************************
 ** \brief Calculate PLL output frequency from settings
 ******************************************************************************/
#define __PLLCLK       ((__CLKMO  * __PLLN) / __PLLK)

/******************************************************************************/
/* Determine core clock frequency according to settings                       */
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief Define Master Clock from settings
 ******************************************************************************/
#if   (((SCM_CTL_Val >> 5ul) & 0x07UL) == 0ul)
  #define __MASTERCLK     (__CLKHC)
#elif (((SCM_CTL_Val >> 5ul) & 0x07UL) == 1ul)
  #define __MASTERCLK     (__CLKMO)
#elif (((SCM_CTL_Val >> 5ul) & 0x07UL) == 2ul)
  #define __MASTERCLK     (__PLLCLK)
#elif (((SCM_CTL_Val >> 5ul) & 0x07UL) == 4ul)
  #define __MASTERCLK     (__CLKLC)
#elif (((SCM_CTL_Val >> 5ul) & 0x07UL) == 5ul)
  #define __MASTERCLK     (__CLKSO)
#else
  #define __MASTERCLK     (0UL)
#endif

/**
 ******************************************************************************
 ** \brief Define System Clock Frequency (Core Clock) from settings
 ******************************************************************************/
#if   ((BSC_PSR_Val & 0x07UL) == 0ul)
  #define __HCLK         (__MASTERCLK / 1ul)
#elif ((BSC_PSR_Val & 0x07UL) == 1ul)
  #define __HCLK         (__MASTERCLK / 2ul)
#elif ((BSC_PSR_Val & 0x07UL) == 2ul)
  #define __HCLK         (__MASTERCLK / 3ul)
#elif ((BSC_PSR_Val & 0x07UL) == 3ul)
  #define __HCLK         (__MASTERCLK / 4ul)
#elif ((BSC_PSR_Val & 0x07UL) == 4ul)
  #define __HCLK         (__MASTERCLK / 6ul)
#elif ((BSC_PSR_Val & 0x07UL) == 5ul)
  #define __HCLK         (__MASTERCLK / 8ul)
#elif ((BSC_PSR_Val & 0x07UL) == 6ul)
  #define __HCLK         (__MASTERCLK /16ul)
#else
  #define __HCLK         (0ul)
#endif

/******************************************************************************/
/* HCLK range check                                                           */
/******************************************************************************/
#if (CHECK_RANGE(__CLKMO, __CLKMOMIN, __CLKMOMAX))
  #error "Main Oscillator Clock (CLKMO) out of range!"
#endif   

#if (CHECK_RANGE(__CLKSO, __CLKSOMIN, __CLKSOMAX))
  #error "Sub Oscillator Clock (CLKMO) out of range!"
#endif   

#if (CHECK_RANGE((__CLKMO / __PLLK), __PLLCLKINMIN, __PLLCLKINMAX))
  #error "PLL input frequency out of range!"
#endif  

#if (CHECK_RANGE(((__CLKMO * __PLLN * __PLLM) / __PLLK), __PLLCLKMIN, __PLLCLKMAX))
  #error "PLL oscillation frequency out of range!"
#endif  
   
#if (CHECK_RANGE(__HCLK, 0UL, __HCLKMAX))
  #error "System Clock (HCLK) out of range!"
#endif
   
/******************************************************************************/
/* Global function prototypes ('extern', definition in C source)              */
/******************************************************************************/

extern uint32_t SystemCoreClock;          // System Clock Frequency (Core Clock)
   
extern void SystemInit (void);            // Initialize the system

extern void SystemCoreClockUpdate (void); // Update SystemCoreClock variable

#ifdef __cplusplus
}
#endif

#endif /* _SYSTEM_MB9ABXXX_H_ */
