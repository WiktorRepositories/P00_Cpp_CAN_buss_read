#include "MyLCD_v3.h"
#include "CustomStructures.h"
#include "COMMON_SETTINGS.h"

#define DISPLAY_MIN_MAX

/********************************************************************************************************************************************
   @brief   Class constructor
   @param   inRefMyDisplay Reference to LCD display class
   @param   inRefLcdEncInterface Reference to LCD and ENC interface
   @param   inRefPackage1 Reference to decoded CAN data1 structure
   @param   inRefPackage2 Reference to decoded CAN data2 structure
   @param   inRefPackage3 Reference to decoded CAN data3 structure
   @param   inRefPackage4 Reference to decoded CAN data4 structure
   @param   inRefPackage5 Reference to decoded CAN data5 structure
   @param   inRefPackage6 Reference to decoded CAN data6 structure
   @param   inRefPackage7 Reference to decoded CAN data7 structure
   @note    This class is conected to intialized LCD object
*/
DisplayCanOnLcd::DisplayCanOnLcd(Adafruit_SSD1306& inRefMyDisplay, LCD_ENC_interface_t& inRefLcdEncInterface, datShortCAN1_t& inRefPackage1, datShortCAN2_t& inRefPackage2, datShortCAN3_t& inRefPackage3, datShortCAN4_t& inRefPackage4, datShortCAN5_t& inRefPackage5, datShortCAN6_t& inRefPackage6, datShortCAN7_t& inRefPackage7):
    RefMyDisplay(inRefMyDisplay),
    refLcdEncInterface(inRefLcdEncInterface),
    refPackage1(inRefPackage1),
    refPackage2(inRefPackage2),
    refPackage3(inRefPackage3),
    refPackage4(inRefPackage4),
    refPackage5(inRefPackage5),
    refPackage6(inRefPackage6),
    refPackage7(inRefPackage7)
{
    _textSize = 1;
}
//===========================================================================================================================================

/********************************************************************************************************************************************
   @brief    Class deconstructor
   @param    void 
*/
DisplayCanOnLcd::~DisplayCanOnLcd(void)
{;}
//===========================================================================================================================================

/********************************************************************************************************************************************
   @brief   Uniwersal display string for all parameters
   @param   inSTRING Constant string literal
   @param   inX Place in X to display. 
   @param   inY Place in Y to display. 
   @param   _textSize Size of text to display
   @return  void
   @note    Asign certain parameters of text and data display
*/
void DisplayCanOnLcd::SetDisplayText(const String &inSTRING, uint8_t inX, uint8_t inY)
{
    inX = LCD_SCREEN_SET_X(inX, 2);
    inY = LCD_SCREEN_SET_Y(inY, 2);

    RefMyDisplay.setTextSize(_textSize);
    RefMyDisplay.setCursor(inX, inY);
    RefMyDisplay.println(inSTRING);
}
//===========================================================================================================================================

/********************************************************************************************************************************************
   @brief   Uniwersal display value for all parameters
   @param   inValue Value to display
   @param   inX Place in X to display. 
   @param   inY Place in Y to display. 
   @param   _textSize Size of text to display
   @return  void
   @note    Asign certain parameters of text and data display
*/
void DisplayCanOnLcd::SetDisplayValue(int16_t inValue, uint8_t inX, uint8_t inY)
{
    inX = LCD_SCREEN_SET_X(inX, 2);
    inY = LCD_SCREEN_SET_Y(inY, 2);

    RefMyDisplay.setTextSize(_textSize);
    RefMyDisplay.setCursor(inX, inY);
    RefMyDisplay.println(inValue, 1);
}
//===========================================================================================================================================

/********************************************************************************************************************************************
   @brief   Uniwersal display error on value when error code is present
   @param   inErrCode Error code.
   @param   inX Place in X to display. 
   @param   inY Place in Y to display. 
   @return  void
   @note    Draw lines around wrong parameter value
*/
void DisplayCanOnLcd::SetDisplayMark(uint8_t inX, uint8_t inY)
{
    uint8_t tYi, tXi;

    inX = LCD_SCREEN_SET_X(inX, 0);
    inY = LCD_SCREEN_SET_Y(inY, 0);
    
    tXi = inX + (_textSize * 32) - 5;
    tYi = inY + (_textSize *  8) + 1;

    RefMyDisplay.drawLine(inX, inY,     inX, tYi,   WHITE);
    RefMyDisplay.drawLine(inX, tYi,     tXi, tYi,   WHITE);
}
//===========================================================================================================================================

/********************************************************************************************************************************************
   @brief   Uniwersal display error on value when error code is present
   @param   inErrCode Error code.
   @param   inX Place in X to display. 
   @param   inY Place in Y to display. 
   @return  void
   @note    Draw lines around wrong parameter value
*/
bool DisplayCanOnLcd::SetDisplayErrorMarkVal(uint8_t inErrCode, uint8_t inX, uint8_t inY)
{
    if(inErrCode == CAN_DAT_PAR_ERROR_OK)   
        return false;

    inX = LCD_SCREEN_SET_X(inX, 0);
    inY = LCD_SCREEN_SET_Y(inY, 0);

    uint8_t tYi, tXi;
    tXi = inX + (_textSize * 32) - 5;
    tYi = inY + (_textSize *  8) + 1;

    RefMyDisplay.drawLine(inX, inY,     tXi, inY, WHITE);
    RefMyDisplay.drawLine(inX, inY,     inX, tYi, WHITE);

    RefMyDisplay.drawLine(inX, tYi,     tXi, tYi, WHITE);
    RefMyDisplay.drawLine(tXi, inY,     tXi, tYi, WHITE);

    return true;
}
//===========================================================================================================================================

/********************************************************************************************************************************************
   @brief   Uniwersal display error  when error code is present
   @param   inErrCode Error code.
   @param   inX Place in X to display. 
   @param   inY Place in Y to display. 
   @return  void
   @note    Display MIN, MAX, or NO DAT
*/
bool DisplayCanOnLcd::SetDisplayErrorData(uint8_t inErrCode, uint8_t inX, uint8_t inY)
{
    if(inErrCode == CAN_DAT_PAR_ERROR_OK)   
        return false;

    inX = LCD_SCREEN_SET_X(inX, 2);
    inY = LCD_SCREEN_SET_Y(inY, 2);

    RefMyDisplay.setTextSize(_textSize);
    RefMyDisplay.setCursor(inX, inY);

    switch (inErrCode)
    {
    case CAN_DAT_PAR_ERROR_NODAT:
        RefMyDisplay.println(F("NoDat"));
        break;

    case CAN_DAT_PAR_ERROR_MAX:
        RefMyDisplay.println(F("MAX!!"));
        break;

    case CAN_DAT_PAR_ERROR_MIN:
        RefMyDisplay.println(F("MIN!!"));
        break;
    
    default: return false;
    }

    return true;
}
//===========================================================================================================================================

/********************************************************************************************************************************************
   @brief   Display LCD sequence 
   @param   inScreen
   @return  current display sequence
   @note    Full display takes 5 to 10 calls.
*/
uint8_t DisplayCanOnLcd::CallDisplayScreens(void)
{
    static uint8_t sDispSate = LCD_SEQ_CLEAR_DISPLAY;
    static uint8_t keepdisplayed;
    static uint8_t sScrNum;

    switch (sDispSate)// Sequence for LCD display data
    {
    case LCD_SEQ_CLEAR_DISPLAY: // Clear LCD screen
    /************************************************************************************/
        RefMyDisplay.clearDisplay();
        keepdisplayed = 0x00;
        sDispSate = LCD_SEQ_NORMAL_DISPLAY_SET;
        sScrNum = refLcdEncInterface.numberOfScreen;
        break;
    //==================================================================================

    case LCD_SEQ_NORMAL_DISPLAY_SET: // Prepare text and data for display
    /************************************************************************************/
        // Set display for a screen.
        _textSize = 1;
        SetDisplayText(F("SCR:"),   0x0, 0xC);
        SetDisplayValue(sScrNum,    0x6, 0xC);
        switch (sScrNum)
        {
        case ENC_LCD_OVERVIEW_FIRST_SCREEN:// Display for a creen number 1
            _textSize = 2;
            SetDisplayText(F("VSPD"),               0x0, 0x0);
            SetDisplayText(F("RPM"),                0x0, 0x5);

            SetDisplayValue(refPackage1.VSPD.value, 0x8, 0x0);
            SetDisplayValue(refPackage1.RPM.value,  0x8, 0x5);
            break;

        case ENC_LCD_OVERVIEW_SECOND_SCREEN:// Display for a screen number 2
            _textSize = 1;
            SetDisplayText(F("OILP"),               0x0, 0x0);
            SetDisplayText(F("OILT"),               0x0, 0x4);
            SetDisplayText(F("CLT"),                0x0, 0x8);

            SetDisplayValue(refPackage3.OILP.value, 0x8, 0x0);
            SetDisplayValue(refPackage3.OILT.value, 0x8, 0x4);
            SetDisplayValue(refPackage3.CLT.value,  0x8, 0x8);
            break;

        case ENC_LCD_OVERVIEW_THIRD_SCREEN:// Display for a creen number 3  OK
            _textSize = 1;
            SetDisplayText(F("RPM"),                0x0, 0x0);     
            SetDisplayText(F("OILP"),               0x0, 0x4);
            SetDisplayText(F("FUELP"),              0x0, 0x8);

            SetDisplayValue(refPackage1.RPM.value,  0x8, 0x0);     
            SetDisplayValue(refPackage3.OILP.value, 0x8, 0x4);
            SetDisplayValue(refPackage3.FUELP.value,0x8, 0x8);
            break;

        case ENC_LCD_OVERVIEW_FOURTH_SCREEN:// Display for a creen number 4
            _textSize = 1;
            SetDisplayText(F("EGT-1"),              0x0, 0x0);     
            SetDisplayText(F("EGT-2"),              0x0, 0x3);     
            SetDisplayText(F("LAMBDA"),             0x0, 0x6);
            SetDisplayText(F("AFR "),               0x0, 0x9);

            SetDisplayValue(refPackage4.EGT_1.value,0x6, 0x0);
            SetDisplayValue(refPackage4.EGT_2.value,0x6, 0x3);
            SetDisplayValue(refPackage4.Lambda.value,0x6, 0x6);
            SetDisplayValue(refPackage4.AFR.value,  0x6, 0x9);
            break;

        case ENC_LCD_OVERVIEW_ENTER_SETTINGS:// Enter change limit values
            _textSize = 1;
            SetDisplayText(F("Enter to limit"),     0x0, 0x0);  
            SetDisplayText(F("values settings"),    0x0, 0x4);   
            SetDisplayText(F("Press button..."),    0x0, 0x8);     
            break;
        
        default: sScrNum = 0x01;
        break;
        }
        sDispSate = LCD_SEQ_NORMAL_DISPLAY_SEND;
        break;
    //==================================================================================

    case LCD_SEQ_NORMAL_DISPLAY_SEND: // Send data to LCD and display
    /************************************************************************************/
        RefMyDisplay.display(); 
        sDispSate = LCD_SEQ_BLINK_DISPLAY_SET;
        break;
    //==================================================================================

    case LCD_SEQ_BLINK_DISPLAY_SET: // Prapare diplay for blinking display
    /************************************************************************************/
        // Set display for a screen when wrong value is present.
        switch (sScrNum)
        {
        case ENC_LCD_OVERVIEW_FIRST_SCREEN:// Display error for a screen number 1
        /****************************************************************/
            if( SetDisplayErrorMarkVal(refPackage1.VSPD.fault, 0x8, 0x0) | 
                SetDisplayErrorMarkVal(refPackage1.RPM.fault,  0x8, 0x5))
            {   sDispSate = LCD_SEQ_BLINK_DISPLAY_SEND;}
            else
            {   sDispSate = LCD_SEQ_WAIT;}
            break;
        //===============================================================

        case ENC_LCD_OVERVIEW_SECOND_SCREEN:// Display error for a screen number 2
        /****************************************************************/
            if( SetDisplayErrorMarkVal(refPackage3.OILP.fault, 0x8, 0x0) |
                SetDisplayErrorMarkVal(refPackage3.OILT.fault, 0x8, 0x4) |
                SetDisplayErrorMarkVal(refPackage3.CLT.fault,  0x8, 0x8))
            {   sDispSate = LCD_SEQ_BLINK_DISPLAY_SEND;}
            else
            {   sDispSate = LCD_SEQ_WAIT;}
            break;
        //===============================================================

        case ENC_LCD_OVERVIEW_THIRD_SCREEN:// Display error for a screen number 3
        /****************************************************************/
            if( SetDisplayErrorMarkVal(refPackage3.OILP.fault,  0x8, 0x0) |   
                SetDisplayErrorMarkVal(refPackage3.OILT.fault,  0x8, 0x4) |
                SetDisplayErrorMarkVal(refPackage3.CLT.fault,   0x8, 0x8))
            {   sDispSate = LCD_SEQ_BLINK_DISPLAY_SEND;}
            else
            {   sDispSate = LCD_SEQ_WAIT;}
            break;
        //===============================================================

        case ENC_LCD_OVERVIEW_FOURTH_SCREEN:// Display error for a screen number 4
        /****************************************************************/
            if( SetDisplayErrorData(refPackage4.EGT_1.fault,   0xA, 0x0) |
                SetDisplayErrorData(refPackage4.EGT_2.fault,   0xA, 0x3) |
                SetDisplayErrorData(refPackage4.Lambda.fault,  0xA, 0x6) |
                SetDisplayErrorData(refPackage4.AFR.fault,     0xA, 0x9))
            {   sDispSate = LCD_SEQ_BLINK_DISPLAY_SEND;}
            else
            {   sDispSate = LCD_SEQ_WAIT;}
            break;
        //===============================================================

        case ENC_LCD_OVERVIEW_ENTER_SETTINGS:// Enter change limit values
        /****************************************************************/
            SetDisplayText(F("Press buttton to enter"), 0x0, 0x8);
            sDispSate = LCD_SEQ_WAIT;
            break;
        //===============================================================
        
        default:
            break;
        }
        break;
    //==================================================================================

    case LCD_SEQ_BLINK_DISPLAY_SEND: // second blinking display
    /************************************************************************************/
        RefMyDisplay.display(); 
        sDispSate = LCD_SEQ_WAIT;
        break;
    //==================================================================================

    case LCD_SEQ_WAIT: // Wait for some cycles. 
    /************************************************************************************/
        if(keepdisplayed < 0xA)
            keepdisplayed++;
        else
            sDispSate = LCD_SEQ_CLEAR_DISPLAY;
        break;
    //==================================================================================
    
    default: sDispSate = LCD_SEQ_CLEAR_DISPLAY;
        break;
    }
        
    return sDispSate;
}
//===========================================================================================================================================

/********************************************************************************************************************************************
 *    @brief   Helper method for display menu
*/
void DisplayCanOnLcd::DisplaySettingsData(limitValues_t* pLimitVals, uint8_t* pParameterNumber)
{
    uint8_t tParameterNumber;
    tParameterNumber = *pParameterNumber;

    switch (tParameterNumber)
    {
    case ENC_LCD_SETTINGS_FIRST_SCREEN:// First set of parameters uint8_t
    /********************************************************************************/
        _textSize = 1;
        SetDisplayText(F("OILP"),   0x0, 0x0);  
        SetDisplayText(F("OILT"),   0x0, 0x2);      
        SetDisplayText(F("FUELP"),  0x0, 0x4);  
        SetDisplayText(F("LAMBDA"), 0x0, 0x6);  
        SetDisplayText(F("AFR"),    0x0, 0x8);  
        
        // MAX values display for uint8_t variables
        SetDisplayValue(pLimitVals->dat1B[IDX_1B_OILP],  0x6, 0x0);
        SetDisplayValue(pLimitVals->dat1B[IDX_1B_OILT],  0x6, 0x2);
        SetDisplayValue(pLimitVals->dat1B[IDX_1B_FUELP], 0x6, 0x4);
        SetDisplayValue(pLimitVals->dat1B[IDX_1B_LAMBDA],0x6, 0x6);
        SetDisplayValue(pLimitVals->dat1B[IDX_1B_AFR],   0x6, 0x8);
        break;
    //===============================================================================

    case 0x02:// Second set of parameters int16_t
    /********************************************************************************/
        _textSize = 1;
        SetDisplayText(F("VSDP"),               0x0, 0x0);     
        SetDisplayText(F("RPM"),                0x0, 0x3);     
        SetDisplayText(F("EGT1"),               0x0, 0x6);
        SetDisplayText(F("EGT2"),               0x0, 0x9);

        // MAX values display for int16_t variables
        SetDisplayValue(pLimitVals->dat2B[IDX_2B_VSPD],0x6, 0x0);
        SetDisplayValue(pLimitVals->dat2B[IDX_2B_RPM], 0x6, 0x3);
        SetDisplayValue(pLimitVals->dat2B[IDX_2B_EGT1],0x6, 0x6);
        SetDisplayValue(pLimitVals->dat2B[IDX_2B_EGT2],0x6, 0x9);
        break;
    //===============================================================================

    case ENC_LCD_SETTINGS_LAST_SCREEN:
    /********************************************************************************/
        _textSize = 1;
        SetDisplayText(F("CLT"),                0x0, 0x0);
        SetDisplayText(F("MIN"),                0x0, 0x4);     
        SetDisplayText(F("MAX"),                0x0, 0x8);

        // MIN / MAX values for int16_t values
        SetDisplayValue(pLimitVals->CLT[0],     0x6, 0x4);
        SetDisplayValue(pLimitVals->CLT[1],     0x6, 0x8);
        break;
    //===============================================================================

    case ENC_LCD_SETTINGS_SAVE_DATA_TO_ROM:
    /********************************************************************************/
        _textSize = 2;
        // Display menu if save data or exit
        SetDisplayText(F("SAVE DATA?"),         0x0, 0x0);
        SetDisplayText(F("YES"),                0x0, 0x5);     
        SetDisplayText(F("NO"),                 0x6, 0x5);
        break;
    //===============================================================================

    case ENC_LCD_SETTINGS_DATA_SAVED:
    /********************************************************************************/
        _textSize = 1;
        // Display menu if save data or exit
        SetDisplayText(F("DATA SAVED"),         0x0, 0x0);
        SetDisplayText(F("TO EEPROM"),          0x0, 0x4);     
        break;
    //===============================================================================

    default:
    /********************************************************************************/
        tParameterNumber = 0x01;
        break;
    }
    //===============================================================================

    *pParameterNumber = tParameterNumber;
}
/********************************************************************************************************************************************
 *    @brief   Helper method for display current marked parameter
*/
void DisplayCanOnLcd::DisplaySettingsMark(uint8_t* pParameterNumber)
{
    uint8_t tParameterNumber, temp1;
    tParameterNumber = *pParameterNumber;

    switch (tParameterNumber)
    {
    case ENC_LCD_SETTINGS_FIRST_SCREEN:
    /********************************************************************************/
        temp1 = refLcdEncInterface.parMarkNumber - 0x1;
        temp1 = temp1 * 0x2;
        // temp1 = {0, 3, 6, 9, 12}
        if((temp1 >= 0) && (temp1 <= 8))
            SetDisplayMark(0x6, temp1);
        else{;}
        break;
    //===============================================================================

    case 0x02:
    /********************************************************************************/
        temp1 = refLcdEncInterface.parMarkNumber - 0x1;
        temp1 = temp1 * 0x3;
        // temp1 = {0, 4, 8, 12}
        if((temp1 >= 0) && (temp1 <= 9))
            SetDisplayMark(0x6, temp1);
        else{;}
        break;
    //===============================================================================

    case ENC_LCD_SETTINGS_LAST_SCREEN:
    /********************************************************************************/
        temp1 = refLcdEncInterface.parMarkNumber;
        temp1 = temp1 * 0x4;
        // temp1 = {5, 10}
        if((temp1 >= 4) && (temp1 <= 8))
            SetDisplayMark(0x6, temp1);
        else{;}
        break;
    //===============================================================================

    case ENC_LCD_SETTINGS_SAVE_DATA_TO_ROM:
    /********************************************************************************/
        temp1 = refLcdEncInterface.parMarkNumber;
        temp1 = temp1 * 0x6;
        //temp1 = {0, 6}
        if((temp1 == 0) || (temp1 == 6))
            SetDisplayMark(temp1, 0x5);
        else{;}
        break;
    //===============================================================================

    case ENC_LCD_SETTINGS_DATA_SAVED:
    /********************************************************************************/
        SetDisplayText(F("!!!"),          0x5, 0x6); 
        break;
    //===============================================================================
    
    default:
    /********************************************************************************/
        tParameterNumber = 0x01;
        break;
    //===============================================================================
    }
    *pParameterNumber = tParameterNumber;
}
/********************************************************************************************************************************************
   @brief   Display limit settings sequence 
   @param   pLimitVals Limit parameters to display
   @return  void
   @note    Full display takes 5 calls.
*/
void DisplayCanOnLcd::CallDisplaySettings(limitValues_t* pLimitVals)
{
    static uint8_t sDispSate = LCD_SEQ_CLEAR_DISPLAY;
    static uint8_t keepdisplayed;
    static uint8_t sParameterNumber, slevel;

    switch (sDispSate)
    {
    case LCD_SEQ_CLEAR_DISPLAY: // Clear LCD screen
    /************************************************************************************/
        RefMyDisplay.clearDisplay();
        keepdisplayed = 0x00;
        sParameterNumber = refLcdEncInterface.parameterScreen;
        slevel = refLcdEncInterface.menuLevel;
        slevel = (slevel >> 4);
        sDispSate = LCD_SEQ_NORMAL_DISPLAY_SET;
        break;
    //==================================================================================

    case LCD_SEQ_NORMAL_DISPLAY_SET: // Display data depend from current screen number 
    /************************************************************************************/
        _textSize = 1;
        SetDisplayText(F("level"),  0x0, 0xC);
        SetDisplayValue(slevel,     0x6, 0xC);
        
        // Helper method for display settings 
        DisplaySettingsData(pLimitVals, &sParameterNumber);
    
        sDispSate = LCD_SEQ_NORMAL_DISPLAY_SEND;
        break;
    //===================================================================================

    case LCD_SEQ_NORMAL_DISPLAY_SEND: // Send data to LCD and display
    /************************************************************************************/
        RefMyDisplay.display(); 
        if(slevel == 0x01)
        {   sDispSate = LCD_SEQ_WAIT;}
        else
        {   sDispSate = LCD_SEQ_BLINK_DISPLAY_SET;}
        break;
    //==================================================================================

    case LCD_SEQ_BLINK_DISPLAY_SET: // Prapare diplay for blinking display
    /************************************************************************************/
        // Helper method for display blink parameter
        DisplaySettingsMark(&sParameterNumber);

        sDispSate = LCD_SEQ_BLINK_DISPLAY_SEND;
        break;

    case LCD_SEQ_BLINK_DISPLAY_SEND: // second blinking display
    /************************************************************************************/
        RefMyDisplay.display(); 
        sDispSate = LCD_SEQ_WAIT;
        break;
    //==================================================================================

    case LCD_SEQ_WAIT: // Wait for some cycles
    /************************************************************************************/
        if(keepdisplayed < 0x0A)
            keepdisplayed++;
        else
            sDispSate = LCD_SEQ_CLEAR_DISPLAY;
        break;
    //==================================================================================
    
    default:
        break;
    }
}
//===========================================================================================================================================