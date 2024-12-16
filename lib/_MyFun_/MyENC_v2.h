#include <stdint.h>
#include "CustomStructures.h"
#include "COMMON_SETTINGS.h"

#ifndef _MY_ENC_V2_H
#define _MY_ENC_V2_H

/*************************************************************************************
*   Macros and definitions
**************************************************************************************/
// Configuration in TimersConfig.h
// F = 16MHz, C:{ 1024, 256, 64, 8, 1 }, xC:[ 0x00, 0xFF ], 
// C = 1024, xC = 255
// T = (C*xC)/F = ~16,32[ms]
// Additional software prescaler
#define MY_ENC_TIME_LONGPRESS   122    // (n * T) + T = t; T(~16,32ms);  (122 * 16,32ms) + 16,32ms ~= 2 007,36ms; 
#define MY_ENC_TIME_DEBOUNCE    0x1    // (n * T) + T = t; T(~16,32ms);  (  1 * 16,32ms) + 16,32ms ~= 32,64ms; 

#define MY_ENC_STATE_STABLEOFF  0x00
#define MY_ENC_STATE_RISING     0x11
#define MY_ENC_STATE_ON_DB      0x22

#define MY_ENC_STATE_STABLEON   0xFF
#define MY_ENC_STATE_FALLING    0xEE
#define MY_ENC_STATE_OFF_DB     0xDD

#define MY_ENC_STATE_LONGPRSS   0x77
#define MY_ENC_MODE_READ        0x88
#define MY_ENC_MODE_WRITE       0x99

#define MY_MODE_MENAG_READ      0x11
#define MY_MODE_MENAG_WRITE     0x22

#define MY_ENC_PARAM_SWIPETHROUGHT 0x11
//====================================================================================


/*************************************************************************************
 * @name    RottaryButton
 * @brief   Screen menegment and parameters edit.
 * @note    This class is using references to existing structures.
*/
class RottaryButton
{
private:
/*  Reference to interface data. Takes less memory. */
    LCD_ENC_interface_t& refLcdEncInterface;    
    limitValues_t&      refLimitValues;    
    uint8_t             rotationValue;

    uint8_t ChangeValueWithinRange(uint8_t inValue, uint8_t inChangeBy, uint8_t inMIN, uint8_t inMAX);
    int16_t ChangeValueByStepAmount(int16_t inValue, uint8_t inRotAmount, uint8_t inStepBy);

public:
/*  Class constructor and deconstructor */
    RottaryButton(LCD_ENC_interface_t& lcdEncInterface, limitValues_t& limitValues);
    ~RottaryButton(void); // Decontructor

    uint8_t DirectionDetection(uint8_t inCLK, uint8_t inDT);
    uint8_t ButtonDebounce(uint8_t input, uint8_t inMASK);

    void RotInc(void);
    void RotDec(void);
    void RotRes(void);
    uint8_t GetTurnNumber(void);
    
    bool TimerTx_ISR_Call11(uint8_t inButton);
    bool TimerTx_ISR_Call22(uint8_t inButton);
};
//====================================================================================
#endif // _MY_ENC_H