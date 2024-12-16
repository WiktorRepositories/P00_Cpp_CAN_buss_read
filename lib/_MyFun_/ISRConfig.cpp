#include "ISRConfig.h"

void SetupExternalInterrupt(uint8_t inDDDx, uint8_t inPORTDx, uint8_t inPCINTx)
{
    cli();

    // Configure PD2 as an input using the Data Direction Register D (DDRD)
    DDRD &= ~_BV(DDD2);
    DDRD &= ~_BV(DDD3);

    // Enable the pull-up resistor on PD2 using the Port D Data Register (PORTD)
    PORTD |= _BV(PORTD2);
    PORTD |= _BV(PORTD3);

    // Enable pin change interrupt on the PCINT18 pin using Pin Change Mask Register 2 (PCMSK2)
    PCMSK2 |= _BV(PCINT18);
    PCMSK2 |= _BV(PCINT19);

    // Enable pin change interrupt 2 using the Pin Change Interrrupt Control Register (PCICR)
    PCICR |= _BV(PCIE2);

    // Enable interrupts
    sei();
}

/*******************************************************************************************************************
* @fn          SetupTimerT2
* @brief       Setup configuration for 8 bit timer T2 interrupt
* @param       inRegCmpVal Value for compare mode
* @param       inMODE Timer operation mode: normal, compare etc
* @param       inPresShift Prescaler value
* @return      void
* @note        For normal mode first parametr is not important
*/
void SetupTimerT2(uint8_t inRegCmpVal,uint8_t inMODE, uint8_t inPresShift)
{
    uint8_t tREG;
    cli();//stop interrupts before configuration

    TCCR2A = 0x00; // set entire TCCR0A register to 0
    TCCR2B = 0x00; // same for TCCR2B
    TCNT2  = 0x00; // initialize counter value to 0

    // (must be OCR0A < 256) => (F) / (f * C - 1); C = 1024, 256, 64, 8, 1
    // OCR0A: Compare value setup
    if(inMODE != MYCFG_TIMER_MODE_NORMAL)
    {   // For overflow mode interrupt compare value is not valid
        if(inRegCmpVal >= 0xFE) 
            tREG = 0xFE;
        else
            tREG = inRegCmpVal;
        OCR2A = tREG;    
    }
    else{;}

    // TCCR0A: Operation mode setup
    switch (inMODE)
    {
    case MYCFG_TIMER_MODE_NORMAL: // Counting to full 0xFF value
        tREG = 0x00;
        break;

    case MYCFG_TIMER_MODE_COMPARE: // CTC, OCRA, Immediate, MAX
        tREG = 0x00;
        tREG |= (1 << WGM21);
        break;
    
    default: // Normal mode 
        tREG = 0x00;
        break;
    } 
    TCCR2A |= tREG;

    // TCCR0B: Prescaler setup
    switch (inPresShift)
    {// Set proper prescaler
    case MYCFG_TIMER_PRESCALER_0001x:    
        tREG = (1 << CS20); 
        break;

    case MYCFG_TIMER_PRESCALER_0008x:    
        tREG = (1 << CS21); 
        break;

    case MYCFG_TIMER_PRESCALER_0064x:
        tREG = (1 << CS21) | (1 << CS20); 
        break;
    
    case MYCFG_TIMER_PRESCALER_0256x:
        tREG = (1 << CS22);
        break;

    case MYCFG_TIMER_PRESCALER_1024x:
        tREG = (1 << CS22) | (1 << CS20); 
        break;

    default:
        tREG = (1 << CS22) | (1 << CS20);
        break;
    } 
    TCCR2B |= tREG;
    
    // enable timer interrupt
    switch (inMODE)
    {
    case MYCFG_TIMER_MODE_NORMAL:
        tREG = (1 << TOIE2);
        break;

    case MYCFG_TIMER_MODE_COMPARE:
        tREG = (1 << OCIE2A);
        break;
    
    default: // Normal mode 
        tREG = (1 << TOIE2);
        break;
    } 
    TIMSK2 |= tREG;

    sei();//allow interrupts
}
//==================================================================================================================
