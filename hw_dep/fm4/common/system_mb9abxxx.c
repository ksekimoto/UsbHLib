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

#include "mcu.h"

/** \file system_mb9abxxx.c
 **
 ** FM3 system initialization functions
 ** All adjustments can be done in belonging header file.
 **
 ** History:
 ** 2013-01-21  0.1  MWi  AI: Unification to be done
 ** 2013-01-23  0.2  MWi  mcu.h inclusion changed to pdl.h
 ** 2013-06-28  0.3  EH   Added Trace Buffer enable
 ******************************************************************************/

/**
 ******************************************************************************
 ** System Clock Frequency (Core Clock) Variable according CMSIS
 ******************************************************************************/
uint32_t SystemCoreClock = __HCLK;

/**
 ******************************************************************************
 ** \brief  Update the System Core Clock with current core Clock retrieved from
 ** cpu registers.
 ** \param  none
 ** \return none
 ******************************************************************************/
void SystemCoreClockUpdate (void) {
  uint32_t masterClk;
  uint32_t u32RegisterRead; // Workaround variable for MISRA C rule conformance

  switch ((FM4_CRG->SCM_CTL >> 5U) & 0x07U) {
    case 0u:                                 /* internal High-speed Cr osc.    */
      masterClk = __CLKHC;
      break;

    case 1u:                                 /* external main osc.             */
      masterClk = __CLKMO;
      break;

    case 2u:                                 /* PLL clock                      */
  // Workaround for preventing MISRA C:1998 Rule 46 (MISRA C:2004 Rule 12.2)
  // violation:
  //   "Unordered accesses to a volatile location"
      u32RegisterRead = (__CLKMO  * (((uint32_t)(FM4_CRG->PLL_CTL2) & 0x3Fu) + 1u));
      masterClk = (u32RegisterRead / ((((uint32_t)(FM4_CRG->PLL_CTL1) >> 4ul) & 0x0Fu) + 1u));
      break;

    case 4u:                                 /* internal Low-speed CR osc.     */
      masterClk = __CLKLC;
      break;

    case 5u:                                 /* external Sub osc.              */
      masterClk = __CLKSO;
      break;

    default:
      masterClk = 0ul;
      break;
  }

  switch (FM4_CRG->BSC_PSR & 0x07u) {
    case 0u:
      SystemCoreClock = masterClk;
      break;

    case 1u:
      SystemCoreClock = masterClk / 2u;
      break;

    case 2u:
      SystemCoreClock = masterClk / 3u;
      break;

    case 3u:
      SystemCoreClock = masterClk / 4u;
      break;

    case 4u:
      SystemCoreClock = masterClk / 6u;
      break;

    case 5u:
      SystemCoreClock = masterClk /8u;
      break;

    case 6u:
      SystemCoreClock = masterClk /16u;
      break;

    default:
      SystemCoreClock = 0ul;
      break;
  }

}

/**
 ******************************************************************************
 ** \brief  Setup the microcontroller system. Initialize the System and update
 ** the SystemCoreClock variable.
 **
 ** \param  none
 ** \return none
 ******************************************************************************/
void SystemInit (void) {

  static uint8_t u8IoRegisterRead;  // Workaround variable for MISRA C rule conformance
  
#if (HWWD_DISABLE)                                 /* HW Watchdog Disable */
  FM4_HWWDT->WDG_LCK = 0x1ACCE551u;                /* HW Watchdog Unlock */
  FM4_HWWDT->WDG_LCK = 0xE5331AAEu;
  FM4_HWWDT->WDG_CTL = 0u;                         /* HW Watchdog stop */
#endif

#if (TRACE_BUFFER_ENABLE)
  FM4_FLASH_IF->FBFCR = 0x01;                      /* Trace Buffer enable */
#endif

#if (CLOCK_SETUP)                                  /* Clock Setup */
  FM4_CRG->BSC_PSR   = (uint8_t)BSC_PSR_Val;                 /* set System Clock presacaler */
  FM4_CRG->APBC0_PSR = (uint8_t)APBC0_PSR_Val;               /* set APB0 presacaler */
  FM4_CRG->APBC1_PSR = (uint8_t)APBC1_PSR_Val;               /* set APB1 presacaler */
  FM4_CRG->APBC2_PSR = (uint8_t)APBC2_PSR_Val;               /* set APB2 presacaler */
  FM4_CRG->SWC_PSR   = (uint8_t)(SWC_PSR_Val | (1ul << 7u)); /* set SW Watchdog presacaler */
  FM4_CRG->TTC_PSR   = (uint8_t)TTC_PSR_Val;                 /* set Trace Clock presacaler */

  FM4_CRG->CSW_TMR   = (uint8_t)CSW_TMR_Val;                 /* set oscillation stabilization wait time */
  
  if (SCM_CTL_Val & (1ul << 1u)) {                 /* Main clock oscillator enabled ? */
    FM4_CRG->SCM_CTL |= (uint8_t)(1ul << 1u);            /* enable main oscillator */ 
    
    while (!((FM4_CRG->SCM_STR) & (uint8_t)(1ul << 1u))) /* wait for Main clock oscillation stable */ 
    {}
  }
  
  if (SCM_CTL_Val & (1UL << 3)) {                    /* Sub clock oscillator enabled ? */
    // Initialize VBAT (Temporary process)
    FM4_RTC->VDET = 0x00;
    FM4_RTC->VBPFR = 0x1C;
    FM4_RTC->CCB = 0x10;
    FM4_RTC->CCS = 0x08;

    // VB_CLK is less or equal to 1MHz (Temporary process)
    FM4_RTC->VB_CLKDIV = 0x4E;
    FM4_RTC->BOOST = 0x03;

    // Enable SUB CLK oscilation (Temporary process)
    FM4_RTC->WTOSCCNT_f.SOSCEX  = 0;
    FM4_RTC->WTOSCCNT_f.SOSCNTL = 1;

    // Transmit to VBAT domain (Temporary process)
    FM4_RTC->WTCR20_f.PWRITE = 1;

    // Wait to complete transmission
    while(0 != FM4_RTC->WTCR10_f.TRANS)

    FM4_CRG->SCM_CTL |= (1UL << 3);                /* enable sub oscillator */ 
    while (!(FM4_CRG->SCM_STR & (1UL << 3)));      /* wait for Sub clock oscillation stable */
  }

  FM4_CRG->PSW_TMR   =  (uint8_t)PSW_TMR_Val;                /* set PLL stabilization wait time */
  FM4_CRG->PLL_CTL1  = (uint8_t) PLL_CTL1_Val;               /* set PLLM and PLLK */
  FM4_CRG->PLL_CTL2  =  (uint8_t)PLL_CTL2_Val;               /* set PLLN          */
  
  if (SCM_CTL_Val &  (uint8_t)(1ul << 4u)) {                    /* PLL enabled ? */
    FM4_CRG->SCM_CTL  |=  (uint8_t)(1ul << 4u);               /* enable PLL */ 
    while (!(FM4_CRG->SCM_STR &  (uint8_t)(1ul << 4u)))      /* wait for PLL stable */
    {}
  }

  FM4_CRG->SCM_CTL  |=  (uint8_t)(SCM_CTL_Val & 0xE0u);       /* Set Master Clock switch */ 
  
  // Workaround for preventing MISRA C:1998 Rule 46 (MISRA C:2004 Rule 12.2)
  // violations:
  //   "Unordered reads and writes to or from same location" and
  //   "Unordered accesses to a volatile location"
  do                                              
  {                                               
    u8IoRegisterRead = (FM4_CRG->SCM_CTL & 0xE0u); 
  }while ((FM4_CRG->SCM_STR & 0xE0u) != u8IoRegisterRead);
#endif // (CLOCK_SETUP)
  
#if (CR_TRIM_SETUP)
  /* CR Trimming Data  */
  if( 0x000003FFu != (FM4_FLASH_IF->CRTRMM & 0x000003FFu) )
  {
    /* UnLock (MCR_FTRM) */
    FM4_CRTRIM->MCR_RLR =  (uint32_t)0x1ACCE554u;
    /* Set MCR_FTRM */
    FM4_CRTRIM->MCR_FTRM = (uint16_t)FM4_FLASH_IF->CRTRMM;
    /* Lock (MCR_FTRM) */
    FM4_CRTRIM->MCR_RLR =  (uint32_t)0x00000000u;
  }
#endif // (CR_TRIM_SETUP)
}



