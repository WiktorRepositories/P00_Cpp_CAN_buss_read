#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT_Macros.h>
#include <Adafruit_SPITFT.h>
#include <gfxfont.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include "mcp_can.h"
#include <SPI.h>
#include <Wire.h>

//CAN const
const PROGMEM byte spiCSPin = 10;
const PROGMEM byte ledPin = 2;
MCP_CAN CAN(spiCSPin);

// Encoder Wheel definition / branch 2 

#define encoderOutA 3 // CP1 (CLK) GPIO3 Rotary Enocoder
#define encoderOutB 4 // CP2 (DT) GPIO4  Rotary Enocoder
int counterScreenID = 0;
int counterParameterID = 0;
int presentStateOutA;
int presentStateOutB;
int previousStateOutA;
int previousStateOutB;

//CAN vars
unsigned char bufferCAN1[8];

// 2Byte RPM (0-16000rpm)
int i_CAN_RPM;
byte RPM_Fault;

// 1B TPS (0-100%)
byte i_CAN_TPS;
byte TPS_Fault;

// 1B IAT (-40-127 C)
byte i_CAN_IAT;
byte IAT_Fault;

// 2B MAP (0-600 kPa)
int i_CAN_MAP;
byte MAP_Fault;

//2Byte VSPD (0-400 km/h)
int i_CAN_VSPD; //
byte VSPD_Fault;

unsigned char bufferCAN2[8]; // Analog Input
// 2B AIN #1 (0-5 V)
int i_CAN_AIN_1;
byte AIN_1_Fault;

// 2B AIN #2 (0-5 V)
int i_CAN_AIN_2;
// 2B AIN #3 (0-5 V)
int i_CAN_AIN_3;
// 2B AIN #4 (0-5 V)
int i_CAN_AIN_4;

unsigned char bufferCAN3[8];

// 2Byte VSPD (0-400 km/h)
int i_CAN_GPS_Speed;
byte GPS_Speed_Fault;

// 1B BARO (50-130 kPa)
byte i_CAN_BARO;
byte BARO_Fault;

// 1B OILT (0-160 C)
byte i_CAN_OILT;
byte OILT_Fault;
byte OILT_Max;
byte OILT_Min;

// 1B OILP (0-12 Bar)
float i_CAN_OILP;
byte OILP_Fault;
float OILP_Max;
float OILP_Min;

// 1B FUELP (0-7 Bar)
float i_CAN_FUELP;
byte FUELP_Fault;
float FUELP_Max;
float FUELP_Min;

// 2B CLT (-40-250 C)
int i_CAN_CLT;
byte CLT_Fault;
int CLT_Max;
int CLT_Min;

unsigned char bufferCAN4[8];

// 1B Ign Angle (-60-60 Deg) 0.5deg/bit
byte i_CAN_Ign_Angle;

// 1B DWELL (0-10 ms)
byte i_CAN_Dwell;

// 1B LAMBDA (0-2 Lambda)
float i_CAN_Lambda;
byte LAMBDA_Fault;

// 1B LAMBDA Correction (75-12t %)
float i_CAN_AFR;
byte AFR_Fault;

// 2B EGT1 (0-1100 C)
int i_CAN_EGT_1;
byte EGT_1_Fault;

// 2B EGT2 (0-1100 C)
int i_CAN_EGT_2;
byte EGT_2_Fault;

unsigned char bufferCAN5[8];
// 1B Gear ( 0 - 7 )
byte i_CAN_Gear;
// 1B ECU TEMP (-40 - 120 C)
byte i_CAN_ECU_Temp;
// 2B BATTery (0-20 V)
float i_CAN_Batt;
byte Batt_Fault;

// 2B Err Flag () bits
/*Bit 0-coolant temp sens failed /  1-IAT sems Faild /
  2-MAP sens. failed /    3-WBand sens Failed/
  4-EGT1 Failed /     5-EGT2 Failed /6-EGT toohigh
  7 - Knock Detected /    8- FFuel sens Failed /  9- DBW failure
  10 - Fuel press relative error
*/
bool i_CAN_ErrFlag[16];
// 1B Flags1 ()
// 1B Etanol Conten (0-100 %)

unsigned char bufferCAN6[8]; //Reserve
// 1B DBW Pos ( 0 - 100 % )
byte i_CAN_DBW_Pos;
// 1B DBW TRGT ( 0 - 100 % )
byte i_CAN_DBW_Target;
// 2B TC DRPM RAW (0-1000)
// 2B TC DRPM (0-400)
// 1B TC Torque Reduction (0-100 %)
// 1B PIT Limit torque reduction (0-100 %)

unsigned char bufferCAN7[8]; //Reserve
// 2B AIN #5 ( 0 - 5 V )
// 2B AIN #6 ( 0 - 5 V )
// 1B OUT FLAGS1 (see EMU Manual)
// 1B OUT FLAGS2
// 1B OUT FLAGS3
// 1B OUT FLAGS4

//++++++++++++++++++++++  OLED variables ++++++++++++++++++++++++++++++++++++++++++++++++++++

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 7     // Reset pin # (or -1 if sharing Arduino reset pin)
int screen_id;
int parameter_id; // for Setting
byte editMode;    // editMode is for parameterScreen, when 1- change parameter and draw lines around parameter, when 2- edit min value, when 3 edit max value

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//  +++++++++++++ Digital input CP5  +++++++++++++
byte D5 = 7;                 //  D5 - GPIO7 (Display Reset)
byte CP5 = 6;                //  CP5 (Reserve)- GPIO6
byte CP6 = 5;                //  CP6 - (SW) GPIO5
unsigned long CP6_Press = 0; // the last time the CP5 was press

void readCANData(void);      // READ CAN DATA
void calculateCANData(void); // CALCULATE CAN DATA
void screenSelect(void);     // SCREEN ID OR PARAMETER ID

void displayVSPD(byte Fault, int screen_ID, int valueVSPD);
void displayOILP(byte Fault, int screen_ID, float valueOILP);
void displayMAP(byte Fault, int screen_ID, int valueMAP);
void displayCLT(byte Fault, int screen_ID, int valueCLT);
void displayRPM(byte Fault, int screen_ID, int valueRPM);
void displayOILT(byte Fault, int screen_ID, byte valueOILT);
void displayFUELP(byte Fault, int screen_ID, float valueFUELP);
void displayMAP(byte Fault, int screen_ID, int valueMAP);
void displayIAT(byte Fault, int screen_ID, byte valueIAT);
void displayBATT(byte Fault, int screen_ID, float valueBATT);
void displayIAT(byte Fault, int screen_ID, byte valueIAT);
void displayMAP(byte Fault, int screen_ID, int valueMAP);
void displayLAMBDA(byte Fault, int screen_ID, float valueLAMBDA);
void displayEGT1(byte Fault, int screen_ID, int valueEGT1);
void displayEGT2(byte Fault, int screen_ID, int valueEGT2);
void displayTPS(byte Fault, int screen_ID, byte valueTPS);
void displayAFR(byte Fault, int screen_ID, float valueAFR);
void editLimits(byte editMode, int Parameter);
void displaySettings(byte editMode, int Parameter);
void parameterSelect(byte editMode);
void changeEditMode(int screen_ID);
void drawLinesScreen1(void);
void drawLinesScreen2(void);
void drawLinesScreen3(void);
void drawLinesScreen4(void);
void drawLinesScreen5(void);
void drawLinesScreen6(void);

//++++++++++++++++++++++++++++++++++++++++ S E T U P +++++++++++++++++++++++++++++++++++++++
void setup()
{

  //Ditial Input
  pinMode(CP5, INPUT); // CP5 (Reserve)- GPIO6
  pinMode(CP6, INPUT); // CP6 - (SW switch) GPIO5
  pinMode(D5, INPUT);  //  D5 - GPIO7 (Display Reset)
  pinMode(encoderOutA, INPUT);
  pinMode(encoderOutB, INPUT);

  Serial.begin(9600);
  previousStateOutA = digitalRead(encoderOutA); // Get current state of the encoderOutA
  previousStateOutB = digitalRead(encoderOutB); // Get current state of the encoderOutA

  //++++++++++++++++++++++ CAN setup  +++++++++++++++++++++++
  //
  //        while (CAN_OK != CAN.begin(CAN_250KBPS))
  //        {
  //            Serial.println("CAN BUS Init Failed");
  //            delay(100);
  //        }
  //        Serial.println("CAN BUS  Init OK!");
  //

  // ++++++++++++++++ OLED ++++++++++++++++++++++++++++++++
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // c or d
    Serial.println(F("SSD1306 allocation failed"));
    // for(;;); // Don't proceed, loop forever
  }
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
}

void loop()
{
  delay(500);
  display.clearDisplay();
  //readCANData();      // READ CAN DATA
  //calculateCANData(); // CALCULATE CAN DATA
  //screenSelect();     // SCREEN ID OR PARAMETER ID
  switch (screen_id)
  {
  case 1:               // SCREEN 1 (Main )
    drawLinesScreen1(); // draw a lines to split screen
    displayVSPD(VSPD_Fault, screen_id, i_CAN_VSPD);
    displayRPM(RPM_Fault, screen_id, i_CAN_RPM);
    break;

  case 2:               // SCREEN 2
    drawLinesScreen2(); // draw a lines to split screen
    displayOILP(OILP_Fault, screen_id, i_CAN_OILP);
    displayOILT(OILT_Fault, screen_id, i_CAN_OILT);
    displayCLT(CLT_Fault, screen_id, i_CAN_CLT);
    break;

  case 3:               // SCREEN 3
    drawLinesScreen3(); // draw a lines to split screen
    displayRPM(RPM_Fault, screen_id, i_CAN_RPM);
    displayOILP(OILP_Fault, screen_id, i_CAN_OILP);
    displayFUELP(FUELP_Fault, screen_id, i_CAN_FUELP);
    break;

  case 4:               // SCREEN 4
    drawLinesScreen4(); // draw a lines to split screen
    displayMAP(MAP_Fault, screen_id, i_CAN_MAP);
    displayIAT(IAT_Fault, screen_id, i_CAN_IAT);
    displayBATT(Batt_Fault, screen_id, i_CAN_Batt);
    displayTPS(TPS_Fault, screen_id, i_CAN_TPS);
    displayEGT1(EGT_1_Fault, screen_id, i_CAN_EGT_1);
    displayEGT2(EGT_2_Fault, screen_id, i_CAN_EGT_2);
    displayLAMBDA(LAMBDA_Fault, screen_id, i_CAN_Lambda);
    displayAFR(AFR_Fault, screen_id, i_CAN_AFR);
    break;

  case 5:               // SCREEN 5
    drawLinesScreen5(); // draw a lines to split screen
    break;

  case 6: // SCREEN 6 Settings
    changeEditMode(screen_id);
    parameterSelect(editMode);
    drawLinesScreen6(); // draw a lines to split screen
    displaySettings(editMode, parameter_id);
    editLimits(editMode, parameter_id);
    break;

  default: // DEAFULT :)
    break;
  }

  display.display();
  delay(1);
}

// ++++++++++++  Buffer 1 CAN Data Display +++++++++++++++

void displayRPM(byte Fault, int screen_ID, int valueRPM)
{ //RPM
  byte size1;
  byte size2;
  byte x1;
  byte y1;
  byte x2;
  byte y2;

  if (screen_ID == 1)
  {
    size1 = 1;
    size2 = 3;
    x1 = 4;
    y1 = 54;
    x2 = 42;
    y2 = 40;
  }

  if (screen_ID == 3)
  {
    size1 = 1;
    size2 = 3;
    x1 = 4;
    y1 = 4;
    x2 = 45;
    y2 = 3;
  }

  display.setTextSize(size1);
  display.setCursor(x1, y1);
  display.println(F("RPM"));
  display.setTextSize(size2);
  display.setCursor(x2, y2);
  display.print(valueRPM);
}

void displayIAT(byte Fault, int screen_ID, byte valueIAT)
{ // IAT
  byte size1;
  byte size2;
  byte x1;
  byte y1;
  byte x2;
  byte y2;
  byte valueIAT2;
  byte temp_127 = 127;

  //sign handling

  if ((screen_ID == 4) & (valueIAT > temp_127))
  { //oly on screen 4
    size1 = 1;
    size2 = 1;
    x1 = 4;
    y1 = 21;
    x2 = 38;
    y2 = 21;

    valueIAT2 = abs(valueIAT - 256);

    display.setTextSize(size1);
    display.setCursor(x1, y1);
    display.println(F("IAT"));
    display.setTextSize(size2);
    display.setCursor(x2, y2);
    display.println("-");
    display.setCursor(x2 + 6, y2);
    display.println(valueIAT2);
  }

  if ((screen_ID == 4) & (valueIAT <= temp_127))
  { //oly on screen 4
    size1 = 1;
    size2 = 1;
    x1 = 4;
    y1 = 21;
    x2 = 38;
    y2 = 21;

    valueIAT2 = valueIAT;

    display.setTextSize(size1);
    display.setCursor(x1, y1);
    display.println(F("IAT"));
    display.setTextSize(size2);
    display.setCursor(x2, y2);
    display.println(valueIAT2);
  }
}

void displayTPS(byte Fault, int screen_ID, byte valueTPS)
{ // TPS
  byte size1;
  byte size2;
  byte x1;
  byte y1;
  byte x2;
  byte y2;
  int screen_width = 89;
  float tempTPS;
  int tempTPS2;
  float maxTPS = 100;

  if (screen_ID == 99)
  {                                       // Not Used
    tempTPS = float((valueTPS / maxTPS)); // temp solution
    tempTPS2 = int(tempTPS * screen_width) + 25;
    x1 = 4;
    y1 = 0;
    display.setTextSize(size1);
    display.setCursor(x1, y1);
    display.println(F("TPS"));
    display.drawLine(25, 0, SCREEN_WIDTH - 13, 0, WHITE); //
    display.drawLine(25, 1, tempTPS2, 1, WHITE);
    display.drawLine(25, 2, tempTPS2, 2, WHITE);
    display.drawLine(25, 3, tempTPS2, 3, WHITE);
    display.drawLine(25, 4, tempTPS2, 4, WHITE);
    display.drawLine(25, 5, SCREEN_WIDTH - 13, 5, WHITE); //
    display.setCursor(x1 + 115, y1);
    display.println(F("%"));
    display.drawLine(115, 0, 115, 5, WHITE);
  }

  if (screen_ID == 4)
  {
    size1 = 1;
    size2 = 1;
    x1 = 4;
    y1 = 53;
    x2 = 38;
    y2 = 53;

    display.setTextSize(size1);
    display.setCursor(x1, y1);
    display.println(F("TPS"));
    display.setTextSize(size2);
    display.setCursor(x2, y2);
    display.println(valueTPS);
  }
}

void displayMAP(byte Fault, int screen_ID, int valueMAP)
{ // MAP
  byte size1;
  byte size2;
  byte x1;
  byte y1;
  byte x2;
  byte y2;

  if (screen_ID == 4)
  {
    size1 = 1;
    size2 = 1;
    x1 = 4;
    y1 = 5;
    x2 = 38;
    y2 = 5;
  }

  display.setTextSize(size1);
  display.setCursor(x1, y1);
  display.println(F("MAP"));
  display.setTextSize(size2);
  display.setCursor(x2, y2);
  display.println(valueMAP);
}

// ++++++++++++  Buffer CAN 2  +++++++++++++++

void displayAIN1(byte Fault, int screen_ID, int valueAIN1)
{
  byte size1;
  byte size2;
  byte x1;
  byte y1;
  byte x2;
  byte y2;

  if (screen_ID == 99)
  { //not used yet
    size1 = 1;
    size2 = 1;
    x1 = 4;
    y1 = 5;
    x2 = 50;
    y2 = 5;
  }

  display.setTextSize(size1);
  display.setCursor(x1, y1);
  display.println(F("MAP:"));
  display.setTextSize(size2);
  display.setCursor(x2, y2);
  display.println(valueAIN1);
}

// ++++++++++++  Buffer CAN 3  +++++++++++++++

void displayVSPD(byte Fault, int screen_ID, int valueVSPD)
{ // VSPD
  byte size1;
  byte size2;
  byte x1;
  byte y1;
  byte x2;
  byte y2;

  if (screen_ID == 1)
  {
    size1 = 1;
    size2 = 4;
    x1 = 4;
    y1 = 2;
    x2 = 42;
    y2 = 3;
  }

  if (screen_ID == 5)
  {
    size1 = 1;
    size2 = 3;
    x1 = 4;
    y1 = 5;
    x2 = 50;
    y2 = 5;
  }

  display.setTextSize(size1);
  display.setCursor(x1, y1);
  display.println(F("SPEED"));
  display.setTextSize(size2);
  display.setCursor(x2, y2);
  display.println(valueVSPD);
}

void displayOILT(byte Fault, int screen_ID, byte valueOILT)
{ //OIL T
  byte size1;
  byte size2;
  byte x1;
  byte y1;
  byte x2;
  byte y2;

  if (screen_ID == 2)
  {
    size1 = 1;
    size2 = 2;
    x1 = 4;
    y1 = 25;
    x2 = 50;
    y2 = 25;
  }

  if (screen_ID == 5)
  { //to do
    size1 = 1;
    size2 = 1;
    x1 = 4;
    y1 = 5;
    x2 = 50;
    y2 = 5;
  }

  display.setTextSize(size1);
  display.setCursor(x1, y1);
  display.println(F("OIL"));
  display.setCursor(x1, y1 + 8);
  display.println(F("TEMP"));
  display.setTextSize(size2);
  display.setCursor(x2, y2);
  display.println(valueOILT);
}

void displayOILP(byte Fault, int screen_ID, float valueOILP)
{ //OILP
  byte size1;
  byte size2;
  byte x1;
  byte y1;
  byte x2;
  byte y2;

  if (screen_ID == 2)
  {
    size1 = 1;
    size2 = 2;
    x1 = 4;
    y1 = 4;
    x2 = 50;
    y2 = 4;

    display.setTextSize(size1);
    display.setCursor(x1, y1);
    display.println(F("OIL"));
    display.setCursor(x1, y1 + 8);
    display.println(F("PRES"));
    display.setTextSize(size2);
    display.setCursor(x2, y2);
    display.println(valueOILP, 1);
  }

  if (screen_ID == 3)
  {
    size1 = 1;
    size2 = 2;
    x1 = 4;
    y1 = 31;
    x2 = 6;
    y2 = 44;

    display.setTextSize(size1);
    display.setCursor(x1, y1);
    display.println(F("OIL PRES"));
    display.setTextSize(size2);
    display.setCursor(x2, y2);
    display.println(valueOILP, 1);
    display.setTextSize(size1);
    display.setCursor(x2 + 48, y2 + 7);
    display.println(F("b"));
  }
}

void displayFUELP(byte Fault, int screen_ID, float valueFUELP)
{ // FUELP
  byte size1;
  byte size2;
  byte x1;
  byte y1;
  byte x2;
  byte y2;

  if (screen_ID == 3)
  {
    size1 = 1;
    size2 = 2;
    x1 = 69;
    y1 = 31;
    x2 = 70;
    y2 = 44;
    display.setTextSize(size1);
    display.setCursor(x1, y1);
    display.println(F("FUEL PRES"));
    display.setCursor(x2 + 47, y2 + 7);
    display.println(F("b"));
    display.setTextSize(size2);
    display.setCursor(x2, y2);
    display.println(valueFUELP, 1);
  }
  if (screen_ID == 99)
  { //to do
    size1 = 1;
    size2 = 1;
    x1 = 4;
    y1 = 5;
    x2 = 50;
    y2 = 5;
  }
}

void displayCLT(byte Fault, int screen_ID, int valueCLT)
{
  byte size1;
  byte size2;
  byte x1;
  byte y1;
  byte x2;
  byte y2;
  int valueCLT2;
  int temp_32767 = 32767;

  if ((screen_ID == 2) & (valueCLT > temp_32767))
  {
    size1 = 1;
    size2 = 2;
    x1 = 4;
    y1 = 46;
    x2 = 50;
    y2 = 46;

    valueCLT2 = abs(valueCLT - 65535);

    display.setTextSize(size1);
    display.setCursor(x1, y1);
    display.println(F("WATER"));
    display.setCursor(x1, y1 + 8);
    display.println(F("TEMP"));
    display.setTextSize(size2);
    display.setCursor(x2, y2);
    display.println("-");
    display.setCursor(x2 + 6, y2);
    display.println(valueCLT2);
  }
  if ((screen_ID == 2) & (valueCLT <= temp_32767))
  {
    size1 = 1;
    size2 = 2;
    x1 = 4;
    y1 = 46;
    x2 = 50;
    y2 = 46;

    valueCLT2 = valueCLT;

    display.setTextSize(size1);
    display.setCursor(x1, y1);
    display.println(F("WATER"));
    display.setCursor(x1, y1 + 8);
    display.println(F("TEMP"));
    display.setTextSize(size2);
    display.setCursor(x2, y2);
    display.println(valueCLT2);
  }
}

// ++++++++++++  Buffer CAN 4  +++++++++++++++

void displayLAMBDA(byte Fault, int screen_ID, float valueLAMBDA)
{ //LAMBDA
  byte size1;
  byte size2;
  byte x1;
  byte y1;
  byte x2;
  byte y2;

  if (screen_ID == 4)
  {
    size1 = 1;
    size2 = 1;
    x1 = 69;
    y1 = 37;
    x2 = 98;
    y2 = 37;
  }
  display.setTextSize(size1);
  display.setCursor(x1, y1);
  display.println(F("LAMB"));
  display.setTextSize(size2);
  display.setCursor(x2, y2);
  display.println(valueLAMBDA, 2);
}

void displayAFR(byte Fault, int screen_ID, float valueAFR)
{ //AFR
  byte size1;
  byte size2;
  byte x1;
  byte y1;
  byte x2;
  byte y2;

  if (screen_ID == 4)
  {
    size1 = 1;
    size2 = 1;
    x1 = 69;
    y1 = 53;
    x2 = 98;
    y2 = 53;
  }
  display.setTextSize(size1);
  display.setCursor(x1, y1);
  display.println(F("AFR"));
  display.setTextSize(size2);
  display.setCursor(x2, y2);
  display.println(valueAFR, 1);
}

void displayEGT1(byte Fault, int screen_ID, int valueEGT1)
{ //EGT1
  byte size1;
  byte size2;
  byte x1;
  byte y1;
  byte x2;
  byte y2;

  if (screen_ID == 4)
  {
    size1 = 1;
    size2 = 1;
    x1 = 69;
    y1 = 5;
    x2 = 98;
    y2 = 5;
  }

  display.setTextSize(size1);
  display.setCursor(x1, y1);
  display.println(F("EGT1"));
  display.setTextSize(size2);
  display.setCursor(x2, y2);
  display.println(valueEGT1);
}

void displayEGT2(byte Fault, int screen_ID, int valueEGT2)
{ //EGT2

  byte size1;
  byte size2;
  byte x1;
  byte y1;
  byte x2;
  byte y2;

  if (screen_ID == 4)
  {
    size1 = 1;
    size2 = 1;
    x1 = 69;
    y1 = 21;
    x2 = 98;
    y2 = 21;
  }

  display.setTextSize(size1);
  display.setCursor(x1, y1);
  display.println(F("EGT2"));
  display.setTextSize(size2);
  display.setCursor(x2, y2);
  display.println(valueEGT2);
}

// ++++++++++++  Buffer CAN 2  +++++++++++++++

void displayGEAR(byte Fault, int screen_ID, byte valueGEAR)
{ // GEAR
}
void displayBATT(byte Fault, int screen_ID, float valueBATT)
{ // BATT
  byte size1;
  byte size2;
  byte x1;
  byte y1;
  byte x2;
  byte y2;

  //int to Float

  if (screen_ID == 4)
  {
    size1 = 1;
    size2 = 1;
    x1 = 4;
    y1 = 37;
    x2 = 36;
    y2 = 37;
  }
  display.setTextSize(size1);
  display.setCursor(x1, y1);
  display.println(F("BATT"));
  display.setTextSize(size2);
  display.setCursor(x2, y2);
  display.println(valueBATT, 1);
}

void displayError(byte Fault, int screen_ID, int valueERROR)
{ //ERRORS
}

void calculateCANData()
{ //CAN DATA CALCULATION

  //Buffer CAN 1
  i_CAN_RPM = (bufferCAN1[0] * 256) + bufferCAN1[1];
  i_CAN_TPS = bufferCAN1[2];
  i_CAN_IAT = bufferCAN1[3];
  i_CAN_MAP = (bufferCAN1[5] * 256) + bufferCAN1[4];
  i_CAN_VSPD = (bufferCAN1[6] * 256) + bufferCAN1[7];

  //Buffer CAN 2
  i_CAN_AIN_1 = (bufferCAN2[0] * 256) + bufferCAN2[1];
  i_CAN_AIN_2 = (bufferCAN2[2] * 256) + bufferCAN2[3];
  i_CAN_AIN_3 = (bufferCAN2[4] * 256) + bufferCAN2[5];
  i_CAN_AIN_4 = (bufferCAN2[6] * 256) + bufferCAN2[7];

  //Buffer CAN 3
  i_CAN_GPS_Speed = (bufferCAN3[0] * 256) + bufferCAN3[1];
  i_CAN_BARO = bufferCAN3[2];
  i_CAN_OILT = bufferCAN3[3];
  i_CAN_OILP = (bufferCAN3[4] / 10);  // *10 in EMU
  i_CAN_FUELP = (bufferCAN3[5] / 10); // *10 in EMU
  i_CAN_CLT = (bufferCAN3[6] * 256) + bufferCAN3[7];

  //buffer CAN 4
  //Byte 0 Reserve
  //Byte 1 Reserve
  float i_CAN_AFR_Temp = bufferCAN4[3];
  i_CAN_AFR = i_CAN_AFR_Temp / 10;
  float i_CAN_LAMBDA_Temp = bufferCAN4[2];
  i_CAN_Lambda = (i_CAN_LAMBDA_Temp / 10);
  i_CAN_EGT_1 = (bufferCAN4[4] * 256) + bufferCAN4[5];
  i_CAN_EGT_2 = (bufferCAN4[6] * 256) + bufferCAN4[7];

  //buffer CAN 5
  i_CAN_Gear = bufferCAN5[0];
  i_CAN_ECU_Temp = bufferCAN5[1];
  float i_CAN_Batt_int = bufferCAN5[2];
  i_CAN_Batt = (i_CAN_Batt_int / 10);
}

void readCANData(void)
{ //READ DATA FROM CAN Network
  unsigned char len = 0;
  unsigned long canId;
  unsigned char TEMP_BUFF[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  const int CAN_Id_1 = 600;
  const int CAN_Id_2 = 601;
  const int CAN_Id_3 = 602;
  const int CAN_Id_4 = 603;
  const int CAN_Id_5 = 604;
  const int CAN_Id_6 = 605;
  const int CAN_Id_7 = 606;

  if (CAN_MSGAVAIL == CAN.checkReceive())
  {

    CAN.readMsgBuf(&len, TEMP_BUFF);
    canId = CAN.getCanId();
    switch (canId)
    {
    case CAN_Id_1:
      memcpy(bufferCAN1, TEMP_BUFF, 8);
      break;

    case CAN_Id_2:
      memcpy(bufferCAN2, TEMP_BUFF, 8);
      break;

    case CAN_Id_3:
      memcpy(bufferCAN3, TEMP_BUFF, 8);
      break;

    case CAN_Id_4:
      memcpy(bufferCAN4, TEMP_BUFF, 8);
      break;

    case CAN_Id_5:
      memcpy(bufferCAN5, TEMP_BUFF, 8);
      break;

    case CAN_Id_6:
      memcpy(bufferCAN6, TEMP_BUFF, 8);
      break;

    case CAN_Id_7:
      memcpy(bufferCAN7, TEMP_BUFF, 8);
      break;

    default:
      Serial.println(F("No CAN Data"));
      break;
    }
  }
}

// *****Parameter Setting Screen****

void changeEditMode(int screen_ID)
{
  int buttonState = digitalRead(CP6);
  unsigned long actualMS = millis();

  if ((actualMS > CP6_Press) & (buttonState == 0) & (screen_id == 6))
  {
    CP6_Press = millis() + 500;
    editMode = editMode + 1;
    if (editMode > 3)
    {
      editMode = 0;
    }
  }
}

void displaySettings(byte editMode, int Parameter)
{
  //editMode =0 - possible to move back
  //editMode =1 - mark parameter
  //editMode =2 - mark min parameter
  //editMode =3 - mark max parameter

  display.setTextSize(1);
  display.setCursor(10, 1);
  display.println(F("limit settings:"));
  display.setCursor(120, 1);
  display.println(Parameter);
  display.setCursor(6, 38);
  display.println(F("min"));
  display.setCursor(66, 38);
  display.println(F("max"));
  //mark parameter with lines

  if (editMode == 1)
  {
    display.drawLine(2, 12, 127, 12, WHITE);
    display.drawLine(2, 12, 2, 33, WHITE);
    display.drawLine(2, 33, 127, 33, WHITE);
    display.drawLine(127, 12, 127, 33, WHITE);
  }
  if (editMode == 2)
  {
    display.drawLine(2, 35, 60, 35, WHITE);
    display.drawLine(2, 35, 2, 63, WHITE);
    display.drawLine(2, 63, 60, 63, WHITE);
    display.drawLine(60, 35, 60, 63, WHITE);
  }
  if (editMode == 3)
  {
    display.drawLine(62, 35, 127, 35, WHITE);
    display.drawLine(62, 35, 62, 63, WHITE);
    display.drawLine(62, 63, 127, 63, WHITE);
    display.drawLine(127, 35, 127, 63, WHITE);
  }

  // parameter_id =
  switch (Parameter)
  {
  case 1: // OILP;
    display.setTextSize(1);
    display.setCursor(6, 16); //min Value
    display.println("OILP");
    display.setTextSize(2);
    display.setCursor(60, 16); //min Value
    display.println(i_CAN_OILP, 1);

    display.setTextSize(1);
    display.setCursor(6, 50); //min Value
    display.println(OILP_Min, 1);
    display.setCursor(66, 50); //max Value
    display.println(OILP_Max, 1);
    break;

  case 2: // OILT;
    display.setTextSize(1);
    display.setCursor(6, 16); //min Value
    display.println("OILT");
    display.setTextSize(2);
    display.setCursor(60, 16); //min Value
    display.println(i_CAN_OILT);

    display.setTextSize(1);
    display.setCursor(6, 50); //min Value
    display.println(OILT_Min, 1);
    display.setCursor(66, 50); //max Value
    display.println(OILT_Max, 1);
    break;

  case 3: // CLT;
    display.setTextSize(1);
    display.setCursor(6, 16); //min Value
    display.println("CLT");
    display.setTextSize(2);
    display.setCursor(60, 16); //min Value
    display.println(i_CAN_CLT);

    display.setTextSize(1);
    display.setCursor(6, 50); //min Value
    display.println(CLT_Min, 1);
    display.setCursor(66, 50); //max Value
    display.println(CLT_Max, 1);
    break;

  case 4: // FUELP;
    display.setTextSize(1);
    display.setCursor(6, 16); //min Value
    display.println("FUELP");
    display.setTextSize(2);
    display.setCursor(60, 16); //min Value
    display.println(i_CAN_FUELP);

    display.setTextSize(1);
    display.setCursor(6, 50); //min Value
    display.println(FUELP_Min, 1);
    display.setCursor(66, 50); //max Value
    display.println(FUELP_Max, 1);
    break;

  case 5: //
    display.setTextSize(1);
    display.setCursor(6, 16); //min Value
    display.println("piec");
    display.setTextSize(2);
    display.setCursor(60, 16); //min Value
    display.println(i_CAN_CLT);

    display.setTextSize(1);
    display.setCursor(6, 50); //min Value
    display.println(CLT_Min, 1);
    display.setCursor(66, 50); //max Value
    display.println(CLT_Max, 1);
    break;

  case 6: //
    display.setTextSize(1);
    display.setCursor(6, 16); //min Value
    display.println("szesc");
    display.setTextSize(2);
    display.setCursor(60, 16); //min Value
    display.println(i_CAN_CLT);

    display.setTextSize(1);
    display.setCursor(6, 50); //min Value
    display.println(CLT_Min, 1);
    display.setCursor(66, 50); //max Value
    display.println(CLT_Max, 1);
    break;

  case 7: //
    display.setTextSize(1);
    display.setCursor(6, 16); //min Value
    display.println("siedem");
    display.setTextSize(2);
    display.setCursor(60, 16); //min Value
    display.println(i_CAN_CLT);

    display.setTextSize(1);
    display.setCursor(6, 50); //min Value
    display.println(CLT_Min, 1);
    display.setCursor(66, 50); //max Value
    display.println(CLT_Max, 1);
    break;

  case 8: //
    display.setTextSize(1);
    display.setCursor(6, 16); //min Value
    display.println("osiem");
    display.setTextSize(2);
    display.setCursor(60, 16); //min Value
    display.println(i_CAN_CLT);

    display.setTextSize(1);
    display.setCursor(6, 50); //min Value
    display.println(CLT_Min, 1);
    display.setCursor(66, 50); //max Value
    display.println(CLT_Max, 1);
    break;
  default:
    break;
  }
}

void screenSelect()
{ //SCREEN SELECT

  //Screen Select
  if (editMode == 0)
  {
    presentStateOutA = digitalRead(encoderOutA);
    presentStateOutB = digitalRead(encoderOutB);
    if ((presentStateOutA != presentStateOutB))
    {
      if (presentStateOutA != previousStateOutA)
      {
        counterScreenID++;
      }
      if (presentStateOutB != previousStateOutB)
      {
        counterScreenID--;
      }
    }
    previousStateOutA = presentStateOutA;
    previousStateOutB = presentStateOutB;
  }

  int sID = counterScreenID;
  if (editMode == 0)
  {
    if ((sID >= 0) & (sID < 1))
    {
      screen_id = 1;
    }
    if ((sID >= 1) & (sID < 2))
    {
      screen_id = 2;
    }
    if ((sID >= 2) & (sID < 3))
    {
      screen_id = 3;
    }
    if ((sID >= 3) & (sID < 4))
    {
      screen_id = 4;
    }
    if ((sID >= 4) & (sID < 5))
    {
      screen_id = 5;
    }
    if ((sID >= 5) & (sID < 6))
    {
      screen_id = 6;
    }
  }

  // Parameter select !!!

  if (editMode == 1)
  {
    presentStateOutA = digitalRead(encoderOutA);
    presentStateOutB = digitalRead(encoderOutB);
    if ((presentStateOutA != presentStateOutB))
    {
      if (presentStateOutA != previousStateOutA)
      {
        counterParameterID++;
      }
      if (presentStateOutB != previousStateOutB)
      {
        counterParameterID--;
      }
    }
    previousStateOutA = presentStateOutA;
    previousStateOutB = presentStateOutB;
  }

  delay(10);

  if (counterScreenID > 6)
  {
    counterScreenID = 6;
  } //Max = 18/3 =6
  if (counterScreenID < 0)
  {
    counterScreenID = 0;
  } // Min = 0
}

// parameterSelect

void parameterSelect(byte editMode)
{
  int sensorA1Value = counterParameterID;
  if (editMode == 1)
  {
    if ((sensorA1Value >= 0) & (sensorA1Value < 1))
    {
      parameter_id = 1;
    }
    if ((sensorA1Value >= 1) & (sensorA1Value < 2))
    {
      parameter_id = 2;
    }
    if ((sensorA1Value >= 2) & (sensorA1Value < 3))
    {
      parameter_id = 3;
    }
    if ((sensorA1Value >= 3) & (sensorA1Value < 4))
    {
      parameter_id = 4;
    }
    if ((sensorA1Value >= 4) & (sensorA1Value < 5))
    {
      parameter_id = 5;
    }
    if ((sensorA1Value >= 5) & (sensorA1Value < 6))
    {
      parameter_id = 6;
    }
    if ((sensorA1Value >= 6) & (sensorA1Value < 7))
    {
      parameter_id = 7;
    }
    if ((sensorA1Value >= 7) & (sensorA1Value < 8))
    {
      parameter_id = 8;
    }
  }

  if (counterParameterID > 8)
  {
    counterParameterID = 8;
  } //Max =24/3 = 8
  if (counterParameterID < 0)
  {
    counterParameterID = 0;
  } // Min = 0
}

// Function to edit Limits
void editLimits(byte editMode, int Parameter)
{

  // FUELP 4
  if (Parameter == 4)
  {
    //Min Value
    if (editMode == 2)
    {
      // FUELP_Min
      //TO DO!!!
    }
    //Max Value
    if (editMode == 3)
    {
    }
  }
}

//++++++++++++++++++++++++++++++++++ DRAW LINES +++++++++++++++++++++++++

void drawLinesScreen1(void)
{ //for Screen 1

  //x
  //display.drawLine(0,0,SCREEN_WIDTH,0,WHITE); // TOP Line
  // display.drawLine(0,SCREEN_HEIGHT-1,SCREEN_WIDTH,SCREEN_HEIGHT-1,WHITE);  // Bottom Line
  //display.drawLine(0,26,SCREEN_WIDTH,26,WHITE); // TOP Line
  // display.drawLine(0,34,SCREEN_WIDTH,36,WHITE);  // Bottom Line

  //y
  //display.drawLine(0,0,0,SCREEN_HEIGHT,WHITE);
  //display.drawLine(SCREEN_WIDTH-1,0,SCREEN_WIDTH-1,SCREEN_HEIGHT,WHITE);
  ;
}

void drawLinesScreen2(void)
{ //for Screen 2
  //x
  display.drawLine(0, 0, SCREEN_WIDTH, 0, WHITE);                                                                                         // TOP Line
  display.drawLine(0, (SCREEN_HEIGHT - 2 * (SCREEN_HEIGHT / 3) - 1), SCREEN_WIDTH, (SCREEN_HEIGHT - 2 * (SCREEN_HEIGHT / 3) - 1), WHITE); // 2nd  Line
  display.drawLine(0, (SCREEN_HEIGHT - (SCREEN_HEIGHT / 3)), SCREEN_WIDTH, (SCREEN_HEIGHT - (SCREEN_HEIGHT / 3)), WHITE);                 // 3rd Line
  display.drawLine(0, SCREEN_HEIGHT - 1, SCREEN_WIDTH, SCREEN_HEIGHT - 1, WHITE);                                                         // Bottom Line
  //y
  display.drawLine(0, 0, 0, SCREEN_HEIGHT, WHITE);
  display.drawLine(SCREEN_WIDTH - 1, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT, WHITE);
}

void drawLinesScreen3()
{ //for Screen 3
  //x
  display.drawLine(0, 0, SCREEN_WIDTH, 0, WHITE);                                 // TOP Line
  display.drawLine(0, 27, SCREEN_WIDTH, 27, WHITE);                               // 2nd  Line
  display.drawLine(0, SCREEN_HEIGHT - 1, SCREEN_WIDTH, SCREEN_HEIGHT - 1, WHITE); // Bottom Line
  //y
  display.drawLine(0, 0, 0, SCREEN_HEIGHT, WHITE);
  display.drawLine(64, 27, 64, 64, WHITE);
  display.drawLine(SCREEN_WIDTH - 1, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT, WHITE);
}

void drawLinesScreen4()
{ //for Screen 4
  //x
  display.drawLine(0, 0, SCREEN_WIDTH, 0, WHITE);                                 // TOP Line
  display.drawLine(0, 16, SCREEN_WIDTH, 16, WHITE);                               // 2nd  Line
  display.drawLine(0, 32, SCREEN_WIDTH, 32, WHITE);                               // 3nd  Line
  display.drawLine(0, 48, SCREEN_WIDTH, 48, WHITE);                               // 4rd  Line
  display.drawLine(0, SCREEN_HEIGHT - 1, SCREEN_WIDTH, SCREEN_HEIGHT - 1, WHITE); // Bottom Line
  //y
  display.drawLine(0, 0, 0, SCREEN_HEIGHT, WHITE);
  // display.drawLine(SCREEN_WIDTH/2-32,0,SCREEN_WIDTH/2-32,SCREEN_HEIGHT,WHITE);
  display.drawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, WHITE);
  display.drawLine(SCREEN_WIDTH - 1, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT, WHITE);
}

void drawLinesScreen5()
{ //for Screen 5
  //x
  display.drawLine(0, 0, SCREEN_WIDTH, 0, WHITE);                                                                                         // TOP Line
  display.drawLine(0, (SCREEN_HEIGHT - 2 * (SCREEN_HEIGHT / 3) - 1), SCREEN_WIDTH, (SCREEN_HEIGHT - 2 * (SCREEN_HEIGHT / 3) - 1), WHITE); // 2nd  Line
  display.drawLine(0, (SCREEN_HEIGHT - (SCREEN_HEIGHT / 3)), SCREEN_WIDTH, (SCREEN_HEIGHT - (SCREEN_HEIGHT / 3)), WHITE);                 // 3rd Line
  display.drawLine(0, SCREEN_HEIGHT - 1, SCREEN_WIDTH, SCREEN_HEIGHT - 1, WHITE);                                                         // Bottom Line
  //y
  display.drawLine(0, 0, 0, SCREEN_HEIGHT, WHITE);
  display.drawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, WHITE);
  display.drawLine(SCREEN_WIDTH - 1, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT, WHITE);
}
void drawLinesScreen6()
{ //for Screen 6
  //x
  //display.drawLine(0,0,SCREEN_WIDTH,0,WHITE); // TOP Line
  //display.drawLine(0,11,SCREEN_WIDTH,11,WHITE); // 2nd  Line
  //display.drawLine(0,30,SCREEN_WIDTH,30,WHITE); // 2nd  Line
  //display.drawLine(0,40,SCREEN_WIDTH,40,WHITE); // 2nd  Line
  //display.drawLine(0,SCREEN_HEIGHT-1,SCREEN_WIDTH,SCREEN_HEIGHT-1,WHITE);  // Bottom Line
  //y
  //display.drawLine(0,0,0,SCREEN_HEIGHT,WHITE);
  //display.drawLine(SCREEN_WIDTH/2,11,SCREEN_WIDTH/2,SCREEN_HEIGHT,WHITE);
  //display.drawLine(SCREEN_WIDTH-1,0,SCREEN_WIDTH-1,SCREEN_HEIGHT,WHITE);
}
