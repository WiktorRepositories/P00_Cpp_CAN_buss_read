#ifndef COMMON_SETTINGS
#define COMMON_SETTINGS

/***********************************************************************************
 * Settings for rottary button and display */
#define ENC_LCD_OVERVIEW_FIRST_SCREEN       0x01U
#define ENC_LCD_OVERVIEW_SECOND_SCREEN      0x02U
#define ENC_LCD_OVERVIEW_THIRD_SCREEN       0x03U
#define ENC_LCD_OVERVIEW_FOURTH_SCREEN      0x04U
#define ENC_LCD_OVERVIEW_LAST_SCREEN        ENC_LCD_OVERVIEW_FOURTH_SCREEN
#define ENC_LCD_OVERVIEW_ENTER_SETTINGS     (ENC_LCD_OVERVIEW_LAST_SCREEN + 0x01U)
//==================================================================================

/***********************************************************************************
 * Changing maximum values for certain parameters */
#define ENC_LCD_SETTINGS_FIRST_SCREEN       0x01U
#define ENC_LCD_SETTINGS_LAST_SCREEN        0x03U
#define ENC_LCD_SETTINGS_SAVE_DATA_TO_ROM   (ENC_LCD_SETTINGS_LAST_SCREEN + 0x01U)
#define ENC_LCD_SETTINGS_DATA_SAVED         (ENC_LCD_SETTINGS_SAVE_DATA_TO_ROM + 0x01U)
//===================================================================================

/************************************************************************************
* Macro function for check amunt of parameter depend on screen numer */
// Return values
#define MAX_PARAMS_ON_SCR_N01   5   // f(1) = OILP + OILT + FUEL + LAMBDA + AFR = 5
#define MAX_PARAMS_ON_SCR_N02   4   // f(2) = VSPD + RPM + EGT1 + EGT2 = 4
#define MAX_PARAMS_ON_SCR_N03   2   // f(3) = CLT MIN + CLT MAX = 2
#define MAX_PARAMS_ON_SCR_N04   2   // f(4) = SAVE DATA YES + SAVE DATA NO = 2
#define MAX_PARAMS_ON_SCR_DEF   1   // default
// Macro funtion
#define PARAMETER_AMOUNT_ON_SCREEN(x) ((x == 1) ? MAX_PARAMS_ON_SCR_N01 : (x == 2) ? MAX_PARAMS_ON_SCR_N02 : (x == 3) ? MAX_PARAMS_ON_SCR_N03 : (x == 4) ? MAX_PARAMS_ON_SCR_N04 : MAX_PARAMS_ON_SCR_DEF)
//=======================================================================================================================================================================

/************************************************************************************
*   Macro for change 1 byte parameter by step */
// Return values
#define PAR_1B_N01_STEP 1      // f(0) = OILP step by 1
#define PAR_1B_N02_STEP 5      // f(1) = OILT step by 5
#define PAR_1B_N03_STEP 1      // f(2) = FUELP step by 1
#define PAR_1B_N04_STEP 10     // f(3) = LAMBDA step by 10
#define PAR_1B_N05_STEP 5      // f(4) = AFR step by 5
#define PAR_1B_DEF_STEP 1      // default
// Macro funtion
#define PARAMETER_1B_STEP(x) ((x == 0) ? PAR_1B_N01_STEP : (x == 1) ? PAR_1B_N02_STEP : (x == 2) ? PAR_1B_N03_STEP : (x == 3) ? PAR_1B_N04_STEP : (x == 4) ? PAR_1B_N05_STEP : PAR_1B_DEF_STEP)
//=======================================================================================================================================================================

/************************************************************************************
*   Macro for change 2 byte parameter by step */
// Return values
#define PAR_2B_N01_STEP 20     // f(0) = VSPD step by 20
#define PAR_2B_N02_STEP 100    // f(1) = RPM step by 100
#define PAR_2B_N03_STEP 50     // f(2) = EGT1 step by 50
#define PAR_2B_N04_STEP 50     // f(3) = EGT2 step by 50
#define PAR_2B_DEF_STEP 50     // default  
// Macro funtion                            
#define PARAMETER_2B_STEP(x) ((x == 0) ? PAR_2B_N01_STEP : (x == 1) ? PAR_2B_N02_STEP : (x == 2) ? PAR_2B_N03_STEP : (x == 3) ? PAR_2B_N04_STEP : PAR_2B_DEF_STEP)
//=======================================================================================================================================================================

#endif //COMMON_SETTINGS