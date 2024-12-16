#include "MyENC_v2.h"
#include "COMMON_SETTINGS.h"

/*********************************************************************************************************************************
   @brief   Class contructor.
   @brief   References to structures are set.
   @param   lcdEncInterface Referance to LCD class which operate display
   @param   inFirstScr First screen number
   @param   inLastScr Last screen number
   @param   inParScrNum Screen with paramater config mode
   @note    void
*/
RottaryButton::RottaryButton(LCD_ENC_interface_t& lcdEncInterface, limitValues_t& limitValues): 
    refLcdEncInterface(lcdEncInterface),
    refLimitValues(limitValues)
{
    refLcdEncInterface.updateDataInROM = false;
    rotationValue = 0;
}
//================================================================================================================================

/*********************************************************************************************************************************
   @brief    Class deconstructor
*/
RottaryButton::~RottaryButton(void)
{;}
//================================================================================================================================

/*********************************************************************************************************************************
   @brief   Increase value of encoder rotation
   @param   void 
   @return  void 
   @note    Rotation increase. Called in input interrupt
*/
void RottaryButton::RotInc(void)
{
    rotationValue++;
}
//================================================================================================================================

/*********************************************************************************************************************************
   @brief   Decrease value of encoder rotation
   @param   void 
   @return  void 
   @note    Rotation decrease. Called in input interrupt
*/
void RottaryButton::RotDec(void)
{
    rotationValue--;
}
//================================================================================================================================

/*********************************************************************************************************************************
   @brief   Reset value of encoder rotation
   @param   void 
   @return  void 
   @note    Reset value of input impulses. Call after read
*/
void RottaryButton::RotRes(void)
{
    rotationValue = 0x00;
}
//================================================================================================================================

/*********************************************************************************************************************************
   @brief   Get current turns amount
   @param   void
   @return  Amount of turns
   @note    Return value of turns
*/
uint8_t RottaryButton::GetTurnNumber(void)
{
    return rotationValue;
}
//================================================================================================================================

uint8_t RottaryButton::DirectionDetection(uint8_t inCLK, uint8_t inDT)
{
    static uint8_t sLastCLK = 0x00;
    static uint8_t sLastDT  = 0x00;
    uint8_t tDirection = 0x00;

    if(inCLK != inDT)
    {
        if(inCLK != sLastCLK)    // Direction +1
        {   tDirection = 0x01;}
        else if(inDT != sLastDT) // Direction -1
        {   tDirection = 0xFF;}
        else{;}
    }
    else{;}

    sLastCLK = inCLK;
    sLastDT = inDT;

    return tDirection;
}

/*********************************************************************************************************************************
   @name    ButtonDebounce
   @brief   Cyclid reading. This method need to be called in timer interrupt. There is 4 states of button.
   @param   inButton Input from button
   @return  Code of presed type. This is one cycle sygnal 
   @note    To work properly timer interupt and need to be configured. Check timer interrupt config.
*/
uint8_t RottaryButton::ButtonDebounce(uint8_t input, uint8_t inMASK)
{
    static uint8_t state;   // Event state 
    static uint8_t sDB;     // Button debounce
    static uint8_t sLP;     // Long press

    input = input & inMASK;
    switch (state)
    {
    case MY_ENC_STATE_STABLEOFF: // Stable relised button. Default state
        if(input == inMASK) // Button rising edge
        {   state = MY_ENC_STATE_RISING;}
        else{;}
        break;

    case MY_ENC_STATE_RISING:
        state = MY_ENC_STATE_ON_DB;
        sDB = 0x00; sLP = 0x00; 
    // Go to next case widhout break

    case MY_ENC_STATE_ON_DB: // Button debouncing delay
        if(sDB < MY_ENC_TIME_DEBOUNCE)  
        {   sDB++;}
        else
        {   state = MY_ENC_STATE_STABLEON;}
        break;

    case MY_ENC_STATE_STABLEON: // Stable presed. waiting for long press or relise.
        if(input == inMASK)
        {// State when button is pressed
            if(sLP < MY_ENC_TIME_LONGPRESS)
            {   sLP++;}
            else if(sLP == MY_ENC_TIME_LONGPRESS)
            {   sLP = 0xFF; 
                return MY_ENC_STATE_LONGPRSS;}
            else{;} // (sLP > MY_ENC_TIME_LONGPRESS)
        }
        else // Releesing button
        {   state = MY_ENC_STATE_FALLING;}
        break;

    case MY_ENC_STATE_FALLING:
        state = MY_ENC_STATE_OFF_DB;
        sDB = 0x00;
    // Go to next case widhout break

    case MY_ENC_STATE_OFF_DB: // Button debouncing delay
        if(sDB < MY_ENC_TIME_DEBOUNCE)  
        {   sDB++;}
        else    
        {   state = MY_ENC_STATE_STABLEOFF;}
        break;
    
    default: state = MY_ENC_STATE_STABLEOFF; 
        break;
    }

    return state;
}
//================================================================================================================================

/*********************************************************************************************************************************
   @brief   Changing value by amount of turns.
   @param   inValue Value to change
   @param   inChangeBy Change inValue by amount
   @param   inMIN Minimum posible value
   @param   inMAX Maximum posible value
   @return  Modyfied value
   @note    Resoult formula: y = x1 + x2 
*/
uint8_t RottaryButton::ChangeValueWithinRange(uint8_t inValue, uint8_t inChangeBy, uint8_t inMIN, uint8_t inMAX)
{
    // This work eaven value is minus
    inValue = inValue + inChangeBy;
    if(inValue > inMAX)
    {   inValue = inMAX;}
    else if((inValue < inMIN) || (inValue > 0x80)) // Value is in minus range 
    {   inValue = inMIN;}
    else{;}

    return inValue;
}
//================================================================================================================================

/*********************************************************************************************************************************
 * @name    LimitParameterChange
   @brief   Change input value by steep times multiplicator
   @param   inValue Value that will be changed
   @param   inRotAmount Change value multiplicator
   @param   inStepBy Base change steep
   @return  Modified value 
   @note    Resoult formula y = x1 + (x2 * x3)
*/
int16_t RottaryButton::ChangeValueByStepAmount(int16_t inValue, uint8_t inRotAmount, uint8_t inStepBy)
{
    int16_t temp;

    if(inRotAmount >= 0x80) // Value -
    {// When value is minus need to be changed to plus
        inRotAmount = 0x00 - inRotAmount;
        temp = inRotAmount * inStepBy;
        inValue = inValue - temp;
    }
    else if(inRotAmount <= 0x7F) // Value +
    {
        temp = inRotAmount * inStepBy;
        inValue = inValue + temp;
    }
    else{;}
    
    return inValue;
}
//================================================================================================================================

/*********************************************************************************************************************************
 * @name    ISR_Call11
   @brief   Call other functions for changing screens
   @param   inButton State of button
   @return  'true' When switch to edit parameter mode and 'false' in normal mode
   @note    This function is called in ISR TimerT2 interrupt 
*/
bool RottaryButton::TimerTx_ISR_Call11(uint8_t inButton)
{
    uint8_t temp = refLcdEncInterface.numberOfScreen;
    temp = ChangeValueWithinRange(temp, rotationValue, ENC_LCD_OVERVIEW_FIRST_SCREEN, ENC_LCD_OVERVIEW_ENTER_SETTINGS);
    refLcdEncInterface.numberOfScreen = temp;
    RotRes(); // After read amount of rotations data need to be reset

    if((refLcdEncInterface.numberOfScreen == ENC_LCD_OVERVIEW_ENTER_SETTINGS) && (inButton == MY_ENC_STATE_LONGPRSS))
    {   return true;}
    else
    {   return false;}                                                      
}
//================================================================================================================================

/*********************************************************************************************************************************
 * @name    ISR_Call22
   @brief   Call other functions for changing screens
   @param   inButton State of button
   @return  True when exit this rutine.
   @note    This function is called in ISR TimerT2 interrupt.
*/
bool RottaryButton::TimerTx_ISR_Call22(uint8_t inButton)
{
    static uint8_t taskmenage = 0x11;
    static uint8_t diplayInfo;
    static bool buttonRelised;// Prevent miltiple screen jump
    uint8_t temp;

    refLcdEncInterface.menuLevel = taskmenage;
    switch (taskmenage)
    {
    /*******************************************************************************
     *  Switch between screens with parameters to change. */
    case 0x11:// Changing screens from [1, 4]
        switch (inButton)
        {
        case MY_ENC_STATE_STABLEOFF:
            buttonRelised = true;
            break;

        case MY_ENC_STATE_FALLING:
            if(buttonRelised == true)
            {   taskmenage = 0x11; 
                buttonRelised = false;
                return true;}
            else{;}
            break;
        
        case MY_ENC_STATE_LONGPRSS:
            if(buttonRelised == true)
            {   taskmenage = 0x22; 
                buttonRelised = false;
                return false;}
            else{;}
            break;
        
        default:
            break;
        }

        temp = refLcdEncInterface.parameterScreen;
        temp = ChangeValueWithinRange(temp, GetTurnNumber(), ENC_LCD_SETTINGS_FIRST_SCREEN, ENC_LCD_SETTINGS_LAST_SCREEN);
        refLcdEncInterface.parameterScreen = temp;
        RotRes(); // After read amount of rotations data need to be reset
        break;
    //=============================================================================

    /******************************************************************************
     *  Switching between parameters on choisen screen */
    case 0x22:// Mark parameter to change value 
        //f(1) = 4, f(2) = 3, f(3) = 2, f(4) = 2,
        switch (inButton)
        {
        case MY_ENC_STATE_STABLEOFF:
            buttonRelised = true;
            break;

        case MY_ENC_STATE_FALLING:
            if(buttonRelised == true)
            {   taskmenage = 0x11; 
                buttonRelised = false;
                return false;}
            else{;}
            break;
        
        case MY_ENC_STATE_LONGPRSS:
            if(buttonRelised == true)
            {   taskmenage = 0x33; 
                buttonRelised = false;
                return false;}
            else{;}
            break;
        
        default:
            break;
        }

        // f(x) gdzie x c [1, 4]
        temp = PARAMETER_AMOUNT_ON_SCREEN(refLcdEncInterface.parameterScreen);
        refLcdEncInterface.parMarkNumber = ChangeValueWithinRange(refLcdEncInterface.parMarkNumber, GetTurnNumber(), 0x01, temp);
        RotRes(); // After read amount of rotations data need to be reset  
        break;
    //=============================================================================

    /******************************************************************************
     *  Increasing choisen parameter */
    case 0x33:// Change value of marked parameter
        switch (inButton)
        {
        case MY_ENC_STATE_STABLEOFF:
            buttonRelised = true;
            break;

        case MY_ENC_STATE_FALLING:
            if(buttonRelised == true)
            {   taskmenage = 0x22; 
                buttonRelised = false;
                return false;}
            else{;}
            break;
        
        case MY_ENC_STATE_LONGPRSS:
            if(buttonRelised == true)
            {   taskmenage = 0x44; 
                refLcdEncInterface.parameterScreen = ENC_LCD_SETTINGS_SAVE_DATA_TO_ROM;
                buttonRelised = false;
                return false;}
            else{;}
            break;
        
        default:
            break;
        }

        temp = refLcdEncInterface.parMarkNumber - 1;
        switch (refLcdEncInterface.parameterScreen)
        {// Chenging parameter of choisen screen number
        case 0x01:  // Screen number 1 with 5 parameters. "refLimitValues.dat1B[0 .. 4]"
            refLimitValues.dat1B[temp]  = ChangeValueByStepAmount(refLimitValues.dat1B[temp], GetTurnNumber(), PARAMETER_1B_STEP(temp));
            break;

        case 0x02:  // Screen number 2 with 4 parameters. "refLimitValues.dat2B[0 .. 3]"
            refLimitValues.dat2B[temp]  = ChangeValueByStepAmount(refLimitValues.dat2B[temp], GetTurnNumber(), PARAMETER_2B_STEP(temp));
            break;

        case 0x03:  // Screen number 3 with 2 parameters. "refLimitValues.CLT[0 .. 1]"
            refLimitValues.CLT[temp]    = ChangeValueByStepAmount(refLimitValues.CLT[temp],   GetTurnNumber(), PARAMETER_2B_STEP(temp));
            break;
        
        default:
            break;
        }
        RotRes(); // After read amount of rotations data need to be reset
        break;
    //=============================================================================

    /******************************************************************************
     * Screen with save data to ROM*/
    case 0x44:// Save data or not
        switch (inButton)
        {
        case MY_ENC_STATE_STABLEOFF:
            buttonRelised = true;
            break;

        case MY_ENC_STATE_FALLING:
            if(buttonRelised == true)
            {   taskmenage = 0x33; 
                buttonRelised = false;
                return false;}
            else{;}
            break;
        
        case MY_ENC_STATE_LONGPRSS:
            if(buttonRelised == true)
            {// Screen with save data and marked Yes value. 0x1 - yes; 0x2 - no.
                if((refLcdEncInterface.parameterScreen == ENC_LCD_SETTINGS_SAVE_DATA_TO_ROM) && (refLcdEncInterface.parMarkNumber == 0x00))
                {   taskmenage = 0x55;}
                else
                {   taskmenage = 0x11;}

                buttonRelised = false;
                return false;
            }
            else{;}
            break;
        
        default:
            break;
        }

        temp = GetTurnNumber();
        if((temp >= 0x01) && (temp <= 0x7F))
        {   refLcdEncInterface.parMarkNumber = 0x00;}
        else if((temp <= 0xFF) && (temp >= 0x80))
        {   refLcdEncInterface.parMarkNumber = 0x01;}
        else{;}
        RotRes();
        break;
    //=============================================================================

    /******************************************************************************
     *  Setting order to save data to ROM*/
    case 0x55:// Set order to save data
        refLcdEncInterface.updateDataInROM = true;
        refLcdEncInterface.parameterScreen = ENC_LCD_SETTINGS_DATA_SAVED;
        taskmenage = 0x66;
        diplayInfo = 0x00;
        break;

    /******************************************************************************
     *  Keep displayed data for some time*/
    case 0x66:// Keep info on LCD for some time
        diplayInfo++;
        if(diplayInfo <= 0xFE)   
        {   break;}
        else
        {
            if(refLcdEncInterface.updateDataInROM == false)
            {   taskmenage = 0x00;
                refLcdEncInterface.parameterScreen = ENC_LCD_SETTINGS_FIRST_SCREEN;
                return true;}
            else{;}
        }
        break;
    //=============================================================================
    
    default: taskmenage = 0x11;
    break;
    }

    return false;                
}
//================================================================================================================================




