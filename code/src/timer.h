/****************************************************************************
 *   $Id:: timer.h 5823 2010-12-07 19:01:00Z usb00423                       $
 *   Project: NXP LPC17xx Timer example
 *
 *   Description:
 *     This file contains Timer code header definition.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/
#ifndef __TIMER_H 
#define __TIMER_H

extern void InitMRT();
extern void delayMs(uint32_t delayInMs);
extern void delayUs(uint32_t delayInUs);
extern void MRT_IRQHandler(void);

#endif /* end __TIMER_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
