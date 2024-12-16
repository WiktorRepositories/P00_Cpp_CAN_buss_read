#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "mcp_can.h"
#include <EEPROM.h>

#include "MyLCD_v3.h"
#include "MyCAN_v2.h"
#include "MyENC_v2.h"
#include "ISRConfig.h"

/*************************************************************************************************************************************
*   Macros and definitions
**************************************************************************************************************************************/
// Inputs and outputs 
#define ENC_CLK       0x02
#define ENC_DT        0x03
#define ENC_SW        0x05

#define CAN_SPEED     0x00
#define CAN_CLK_SET   0x00

#define MAIN_PROGRAM_READ_FROM_CAN  0x11
#define MAIN_PROGRAM_WRITE_TO_ROM   0x22

#define MAIN_CAN_CLASS_SPI_CS_PIN (byte)10

//====================================================================================================================================

/*************************************************************************************************************************************
*  Global structures declaration
**************************************************************************************************************************************/
// Global undecoded CAN data
CAN_data_undecoded_t dataFromCan; 

// Global CAN decoded data. Both DisplayCanOnLcd and DecodeCANDataClass are using these structures.
datShortCAN1_t canDataPack1;
datShortCAN2_t canDataPack2;
datShortCAN3_t canDataPack3;
datShortCAN4_t canDataPack4;
datShortCAN5_t canDataPack5;
datShortCAN6_t canDataPack6;
datShortCAN7_t canDataPack7;

// Comunications interface between classes: DisplayCanOnLcd and RottaryButton
LCD_ENC_interface_t   lcdEncInterface;
limitValues_t         limitValues;

// Some other variables
uint8_t mainProgramJobSelect, timerCounter, button;
//====================================================================================================================================

/*************************************************************************************************************************************
*   Class object constructors
**************************************************************************************************************************************/
Adafruit_SSD1306      AdafruitLCDDisplay(LCD_SCREEN_WIDTH, LCD_SCREEN_HEIGHT, &Wire, LCD_DISP_RES);
MCP_CAN               McpCan(MAIN_CAN_CLASS_SPI_CS_PIN);

DisplayCanOnLcd       DisplayCanData(AdafruitLCDDisplay, lcdEncInterface, canDataPack1, canDataPack2, canDataPack3, canDataPack4, canDataPack5, canDataPack6, canDataPack7);
DecodeCANDataClass    CanDecode(canDataPack1, canDataPack2, canDataPack3, canDataPack4, canDataPack5, canDataPack6, canDataPack7, limitValues);
RottaryButton         RottButt(lcdEncInterface, limitValues);
//====================================================================================================================================


/*************************************************************************************************************************************
* @brief  Init data for CAN buss structure
* @note   Init in function in order to save space
**************************************************************************************************************************************/
void GlobalCanDataInitialization(void)
{ // LCD init values
  lcdEncInterface.numberOfScreen = 0x01;
  lcdEncInterface.parameterScreen = 0x01;
  lcdEncInterface.parMarkNumber = 0x01;
  lcdEncInterface.updateDataInROM = false;

  // Main display program initialization.
  mainProgramJobSelect = MAIN_PROGRAM_WRITE_TO_ROM;
  dataFromCan.ID = MYCAN_CAN_ID_1;
  timerCounter = 0;

  // Maximum value structure initialization.
  uint8_t*  p = (uint8_t*)&(limitValues);
  for(uint8_t i= 0; i<sizeof(limitValues_t); i++)
  { *(p + i) = 0x01;}
}
void SimulationCanTransmit(CAN_data_undecoded_t* pDat)
{
  static uint8_t test;

  if(pDat->ID <= MYCAN_CAN_ID_7)
  {   pDat->ID++;}
  else
  {   pDat->ID = MYCAN_CAN_ID_1;}
  
  if(test < 0x20)
  {   test++;}
  else
  {   test = 0x00;}

  pDat->arrData[0] = test;
  pDat->arrData[1] = test;
  pDat->arrData[2] = test;
  pDat->arrData[3] = test;
  pDat->arrData[4] = test;
  pDat->arrData[5] = test;
  pDat->arrData[6] = test;
  pDat->arrData[7] = test;
}
void UpdateEEPROM(uint16_t inStartPtr, uint8_t* pBase, uint8_t inSize)
{ // If data is diffrent then save inside EEPROM cell
  // EEPROM has only 100 000 write cycle 
  if((inStartPtr + inSize) >= 1023) return;

  // Write cycle per cell takes aproximate time 3ms.
  // Update method read and if data is diffrent then write data.
  for(uint8_t i = 0; i<inSize; i++)
  { EEPROM.update((inStartPtr + i), *(pBase + i));}
}
void ReadFromEEPROM(uint16_t inStartPtr, uint8_t* pBase, uint8_t inSize)
{ // This method is use only on initializing the program.
  if((inStartPtr + inSize) >= 1023) return;

  for(uint8_t i = 0; i<inSize; i++)
  { *(pBase + i) = EEPROM.read(inStartPtr + i);} 
}
void CallBack_PIN2_PIN3(void);
/*************************************************************************************************************************************
* @brief  Main program initialization for init functions call
* @note   Object initialization here is not posible (class constructors)
**************************************************************************************************************************************/
void setup()
{ // Init variables 
  // Ditial Input init
  pinMode(LCD_CP6, INPUT);       // CP5 (Reserve)- GPIO6
  pinMode(LCD_CP5, INPUT);       // CP6 - (SW switch) GPIO5
  pinMode(LCD_DISP_RES, INPUT);  //  D5 - GPIO7 (Display Reset)

  pinMode(ENC_CLK, INPUT); // Enocder CLK input
  pinMode(ENC_DT, INPUT);  // Encoder DT input
  pinMode(ENC_SW, INPUT); // Enocder button input

  Serial.begin(9600);
  McpCan.begin(CAN_SPEED, CAN_CLK_SET);
  // Initialize memory for LCD display.
  // Also if stack is left with too less memory then application will be corupted.
  // Check all memory comsumption from display malloc(). In this current displat 128 x 64 is about ~1200 Bytes.
  // WARNING!!! If uC will be left with not enought memory display is not posible!!!
  if(false == AdafruitLCDDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Memory allocation failed. LCD is not initialized
    Serial.println(F("SSD1306 allocation failed"));
    // Don't proceed, loop forever. Program is not able to run
    while(true){;} 
  }
  else // Memory initialization OK. In this app "(uint8_t*)(malloc(128 * (64 + 7)) = ~1136 Byte".
  {   
    AdafruitLCDDisplay.setTextColor(SSD1306_WHITE);
    AdafruitLCDDisplay.clearDisplay();
  }

  GlobalCanDataInitialization();
  ReadFromEEPROM(0x00, (uint8_t*)&(limitValues), sizeof(limitValues_t));

#ifdef IN_BUTTON_INTERRUPT 
  attachInterrupt(digitalPinToInterrupt(2) ,CallBack_PIN2_PIN3, CHANGE); 
  attachInterrupt(digitalPinToInterrupt(3), CallBack_PIN2_PIN3, CHANGE); 
#endif

  SetupTimerT2(0x00, MYCFG_TIMER_MODE_NORMAL, MYCFG_TIMER_PRESCALER_1024x);
}

/*************************************************************************************************************************************
*   @brief   Main program infinite loop
**************************************************************************************************************************************/
void loop()
{// Diplay data on LCD
delay(50);

  /* Main program choice */
  if(mainProgramJobSelect == MAIN_PROGRAM_READ_FROM_CAN)
  {/* Display decoded data on LCD 
    * This sequence takes 7 cycles + wait time 10 cycles. In order to avoid display corruption */
    if(DisplayCanData.CallDisplayScreens() == LCD_SEQ_CLEAR_DISPLAY) 
    { // If display finished then load data from CAN
      /* Read and decode data from CAN buss */
      ReadCanDataBy_MCP_CAN(&McpCan, &dataFromCan);
      CanDecode.CallForReadCanData(&dataFromCan);
    }
    else{;}
  }
  else if(mainProgramJobSelect == MAIN_PROGRAM_WRITE_TO_ROM)      
  {  /* Display settings screens. */ 
    DisplayCanData.CallDisplaySettings(&limitValues);
    if(lcdEncInterface.updateDataInROM)
    { // Update data and send to ROM memory 
      lcdEncInterface.updateDataInROM = false;
      UpdateEEPROM(0x00, (uint8_t*)&(limitValues), sizeof(limitValues_t));
    }
    else{;}
  }
  else{;}
} 
//====================================================================================================================================

/*************************************************************************************************************************************
 * External input interruptcallback.
 * When state change interrupt is called */
void CallBack_PIN2_PIN3(void)
{
  #ifdef IN_BUTTON_INTERRUPT 
  cli();

  uint8_t tRotDir = RottButt.DirectionDetection(bitRead(PIND,2), bitRead(PIND,3));
  if(tRotDir == 0x01)
  {   RottButt.RotInc();}
  else if (tRotDir == 0xFF)
  {   RottButt.RotDec();}
  else{;}

  sei();
  #endif
}

/*************************************************************************************************************************************
 * Timer T2 interrupt overflow callback.
 * Cyclical call for time depent action like botton press and rottation. */
ISR(TIMER2_OVF_vect)
{
  timerCounter++;
  if(timerCounter >= 0x6)
  {  
    timerCounter = 0x00; // Reset interrupt number 
  }
  else if(timerCounter == 0x1) // Button state read rutine
  {
    button = !(bitRead(PIND,ENC_SW)); // Negation because of low active state
    button = RottButt.ButtonDebounce(button, 0x01);

    uint8_t tRotDir = RottButt.DirectionDetection(bitRead(PIND,2), bitRead(PIND,3));
    if(tRotDir == 0x01)       { RottButt.RotInc();}
    else if (tRotDir == 0xFF) { RottButt.RotDec();}
    else{;}
  }
  else if(timerCounter == 0x2) // Display information change rutine
  {// Program jump distrubutor
    if(mainProgramJobSelect == MAIN_PROGRAM_READ_FROM_CAN)        
    { /* Display CAN data change screen rutine. */
      if(RottButt.TimerTx_ISR_Call11(button) == true)   { mainProgramJobSelect = MAIN_PROGRAM_WRITE_TO_ROM;}
      else{;}
    }
    else if(mainProgramJobSelect == MAIN_PROGRAM_WRITE_TO_ROM)    
    { /* Settings maximum values change rutine. */
      if(RottButt.TimerTx_ISR_Call22(button) == true)   { mainProgramJobSelect = MAIN_PROGRAM_READ_FROM_CAN;}
      else{;}
    }
    else{;}
  }
  else{;}
}
//====================================================================================================================================