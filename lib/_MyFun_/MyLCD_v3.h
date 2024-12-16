#include <stdint.h>
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "CustomStructures.h"

#ifndef _MY_LCD_V3_H
#define _MY_LCD_V3_H

//====================================================================================================
// LCD configuration pin definitions
#define LCD_DISP_RES            0x07U // D5 - GPIO7 (Display Reset)
#define LCD_OLED_RESET          0x07U // Reset pin # (or -1 if sharing Arduino reset pin)
#define LCD_CP5                 0x05U // CP5 (Reserve)- GPIO6
#define LCD_CP6                 0x06U
//====================================================================================================

//====================================================================================================
// LCD position configuration
#define LCD_SCREEN_WIDTH        128     // OLED display width, in pixels
#define LCD_SCREEN_HEIGHT       64      // OLED display height, in pixels
#define LCD_SCREEN_STEEP_DIV    16      // Division factor 

#define LCD_SCREEN_STEP_X       (LCD_SCREEN_WIDTH  / LCD_SCREEN_STEEP_DIV) // Base step for X position
#define LCD_SCREEN_STEP_Y       (LCD_SCREEN_HEIGHT / LCD_SCREEN_STEEP_DIV) // Base step for Y position

#define LCD_SCREEN_SET_X(X, Xi) ((X * LCD_SCREEN_STEP_X) + Xi) // Macro to calculate start X position
#define LCD_SCREEN_SET_Y(Y, Yi) ((Y * LCD_SCREEN_STEP_Y) + Yi) // Macro to calculate start Y position
//====================================================================================================

//====================================================================================================
// LCD I2C pin configuration data 
#define I2C_PC4_SDA     18
#define I2C_PC5_SCL     19
//====================================================================================================

//====================================================================================================
// LCD SPI pin configuration data
#define SPI_PB5_SCK     13
#define SPI_PB4_MISO    12
#define SPI_PB3_MOSI    11
#define SPI_PB5_SS      10
//====================================================================================================

//====================================================================================================
// LCD display sequence
#define LCD_SEQ_CLEAR_DISPLAY       0x00
#define LCD_SEQ_NORMAL_DISPLAY_SET  0x11
#define LCD_SEQ_NORMAL_DISPLAY_SEND 0x22
#define LCD_SEQ_BLINK_DISPLAY_SET   0x33
#define LCD_SEQ_BLINK_DISPLAY_SEND  0x44
#define LCD_SEQ_SEND_DAT_TO_ROM     0x55
#define LCD_SEQ_WAIT                0xFF
//====================================================================================================

/******************************************************************************************************
 * @name    DisplayCanOnLcd
 * @brief   Preparation data for display.
 * @note    This class is using reference Adafruit_SSD1306&  to existing Class that make actual dispaly
*/
class DisplayCanOnLcd
{
private:
/*  Reference input data from other classes */
    Adafruit_SSD1306&       RefMyDisplay;       // Adafruit LCD display initialized class
    LCD_ENC_interface_t&    refLcdEncInterface; // Data from rottary button
    uint8_t _textSize;

/*  Reference to global decoded data from CAN buss */
    datShortCAN1_t& refPackage1;
    datShortCAN2_t& refPackage2;
    datShortCAN3_t& refPackage3;
    datShortCAN4_t& refPackage4;
    datShortCAN5_t& refPackage5;
    datShortCAN6_t& refPackage6;
    datShortCAN7_t& refPackage7;

/*  Prapare date for screen */
    void SetDisplayText(const String &inSTRING, uint8_t inX, uint8_t inY);   
    void SetDisplayValue(int16_t inValue, uint8_t inX, uint8_t inY); 
    void SetDisplayMark(uint8_t inX, uint8_t inY);
    bool SetDisplayErrorMarkVal(uint8_t inErrCode, uint8_t inX, uint8_t inY);
    bool SetDisplayErrorData(uint8_t inErrCode, uint8_t inX, uint8_t inY);

    void DisplaySettingsData(limitValues_t* pLimitVals, uint8_t* pParameterNumber);
    void DisplaySettingsMark(uint8_t* pParameterNumber);

public:
    DisplayCanOnLcd(Adafruit_SSD1306& inRefMyDisplay, LCD_ENC_interface_t& inRefLcdEncInterface, datShortCAN1_t& inRefPackage1, datShortCAN2_t& inRefPackage2, datShortCAN3_t& inRefPackage3, datShortCAN4_t& inRefPackage4, datShortCAN5_t& inRefPackage5, datShortCAN6_t& inRefPackage6, datShortCAN7_t& inRefPackage7);
    ~DisplayCanOnLcd(void);
    
    uint8_t CallDisplayScreens(void);
    void CallDisplaySettings(limitValues_t* pLimitVals);
};

#endif //_MY_CAN_H