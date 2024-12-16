#include "Arduino.h"

#ifndef _ISR_CONFIG_H
#define _ISR_CONFIG_H

#define MYCFG_PIN_MODE_CHANGE_STATE     0x11

#define MYCFG_TIMER_PRESCALER_1024x    0xA
#define MYCFG_TIMER_PRESCALER_0256x    0x8
#define MYCFG_TIMER_PRESCALER_0064x    0x6
#define MYCFG_TIMER_PRESCALER_0008x    0x3
#define MYCFG_TIMER_PRESCALER_0001x    0x0

#define MYCFG_TIMER_MODE_NORMAL        0x11
#define MYCFG_TIMER_MODE_PWM           0x11
#define MYCFG_TIMER_MODE_CTF           0x33
#define MYCFG_TIMER_MODE_COMPARE       0x44

void SetupExternalInterrupt(uint8_t inDDDx, uint8_t inPORTDx, uint8_t inPCINTx);

/*************************************************************************
* @fn          SetupTimerT2
* @brief       Setup configuration for 8 bit timer T2 interrupt
* @param       inRegCmpVal Value for compare mode
* @param       inMODE Timer operation mode: normal, compare etc
* @param       inPresShift Prescaler value
* @return      void
* @note        For normal mode first parametr is not important
*/
void SetupTimerT2(uint8_t inRegCmpVal,uint8_t inMODE, uint8_t inPresShift);

#endif // _ISR_CONFIG_H