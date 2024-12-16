#include <stdint.h>
#include <Arduino.h>

#ifndef _CUSTOM_STRUCTURES_H
#define _CUSTOM_STRUCTURES_H

/****************************************************************************************************
 * @brief   Definitions of MIN and MAX values for each parameter
*/
#define CAN_DAT_PAR_ERROR_OK        0x00
#define CAN_DAT_PAR_ERROR_NODAT     0x11
#define CAN_DAT_PAR_ERROR_MAX       0x22   
#define CAN_DAT_PAR_ERROR_MIN       0x33
//============================================================================

/*****************************************************************************
 * @name    Structures for LCD screen interface with rottary button.
*/
typedef struct 
{
    uint8_t numberOfScreen;
    uint8_t limitValuesOfparametr;
    uint8_t numberOfParametr;

    uint8_t parameterScreen;
    uint8_t parMarkNumber;
    uint8_t menuLevel;

    bool updateDataInROM;
}LCD_ENC_interface_t;
//============================================================================

/*****************************************************************************
 * @name    Structures for CAN undecoded data from can buss.
*/
#define CAN_DATA_MAX_LEN 0x8
typedef struct 
{
    unsigned long   ID;
    byte            len;
    uint8_t arrData[CAN_DATA_MAX_LEN];
}CAN_data_undecoded_t;
//============================================================================

/*****************************************************************************
 * @name    Basic structures for CAN decoded data.
*/
typedef struct 
{// Basic memeber of type uint8_t
    uint8_t max;
    uint8_t value;
    uint8_t min;
    uint8_t fault;
}valParLong_uint8_t;

typedef struct 
{// Basic memeber of type int16_t
    int16_t max;
    int16_t value;
    int16_t min;
    uint8_t fault;
}valParLong_int16_t;

typedef struct 
{// Basic memeber of type float
    float max;
    float value;
    float min;
    uint8_t fault;
}valParLong_float_t;
//============================================================================

/*****************************************************************************
 * @name    Specyfic Structores for containing data
*/
typedef struct
{// Structures collection for package1
    valParLong_int16_t   RPM;  // 2Byte RPM     (0 .. 16000rpm)
    valParLong_uint8_t   TPS;  // 1B TPS        (0 .. 100%)
    valParLong_uint8_t   IAT;  // 1B IAT        (-40 .. 127 C)
    valParLong_int16_t   MAP;  // 2B MAP        (0 .. 600 kPa)
    valParLong_int16_t   VSPD; // 2Byte VSPD    (0 .. 400 km/h)
}datFullCAN1_t;

typedef struct
{// Structures collection for package2
    valParLong_int16_t  AIN1;  // 2B AIN #1 (0 .. 5 V)
    valParLong_int16_t  AIN2;  // 2B AIN #2 (0 .. 5 V)
    valParLong_int16_t  AIN3;  // 2B AIN #3 (0 .. 5 V)
    valParLong_int16_t  AIN4;  // 2B AIN #4 (0 .. 5 V)
}datFullCAN2_t;

typedef struct
{// Structures collection for package3
    valParLong_int16_t  GPS_Speed;// 2Byte VSPD (0 .. 400 km/h)
    valParLong_uint8_t  BARO;   // 1B BARO  (50 .. 130 kPa)
    valParLong_uint8_t  OILT;   // 1B OILT  (0 .. 160 C)
    valParLong_uint8_t  OILP;   // 1B OILP  (0 .. 12 Bar)
    valParLong_uint8_t  FUELP;  // 1B FUELP (0 .. 7 Bar)
    valParLong_int16_t  CLT;    // 2B CLT   (-40 .. 250 C)
}datFullCAN3_t;

typedef struct 
{// Structures collection for package4
    valParLong_uint8_t  Ign_Angle;// 1B Ign Angle   (-60 .. 60 Deg) 0.5deg/bit
    valParLong_uint8_t  Dwell;  // 1B DWELL         (0 .. 10 ms)
    valParLong_uint8_t  Lambda; // 1B LAMBDA        (0 .. 2 Lambda)
    valParLong_uint8_t  AFR;    // 1B LAMBDA Correction (75 .. 12t %)
    valParLong_int16_t  EGT_1;  // 2B EGT1          (0 .. 1100 C)
    valParLong_int16_t  EGT_2;  // 2B EGT1          (0 .. 1100 C)
}datFullCAN4_t;

typedef struct 
{// Structures collection for package5
    valParLong_uint8_t  Gear;      // 1B Gear       ( 0 .. 7 )
    valParLong_uint8_t  ECU_Temp;  // 1B ECU TEMP   (-40 .. 120 C)
    valParLong_int16_t  Battery;   // 2B BATTery    (0 .. 20 V)
}datFullCAN5_t;

typedef struct 
{// Structures collection for package6
    valParLong_uint8_t  DBW_Pos;    // 1B DBW Pos                   (0 .. 100 % )
    valParLong_uint8_t  DBW_Target; // 1B DBW TRGT                  (0 .. 100 % )
    valParLong_int16_t  TC_DRPM_RAW;// 2B TC DRPM RAW               (0 .. 1000)
    valParLong_int16_t  TC_DRPM;    // 2B TC DRPM                   (0 .. 400)
    valParLong_uint8_t  TC_TORQ_REDU;// 1B TC Torque Reduction      (0 .. 100 %)
    valParLong_uint8_t  PIT_TRQ_REDU;// 1B PIT Limit torque reduction (0 ..100 %)
}datFullCAN6_t;

typedef struct 
{// Structures collection for package7
    valParLong_int16_t  AIN5;       // 2B AIN #5 (0 .. 5 V )
    valParLong_int16_t  AIN6;       // 2B AIN #6 (0 .. 5 V )
    valParLong_uint8_t  OUT_FLAG1;  // 1B OUT FLAGS1 (see EMU Manual)
    valParLong_uint8_t  OUT_FLAG2;  // 1B OUT FLAGS2
    valParLong_uint8_t  OUT_FLAG3;  // 1B OUT FLAGS3
    valParLong_uint8_t  OUT_FLAG4;  // 1B OUT FLAGS4
}datFullCAN7_t;
//============================================================================

/*****************************************************************************
 * @name    Lesss memory consuming CAN data packages
*/
typedef struct 
{// Basic memeber of type uint8_t
    uint8_t value;
    uint8_t fault;
}valParShort_uint8_t;

typedef struct 
{// Basic memeber of type int16_t
    int16_t value;
    uint8_t fault;
}valParShort_int16_t;

typedef struct 
{// Basic memeber of type float
    float value;
    uint8_t fault;
}valParShort_float_t;
//============================================================================

/*****************************************************************************
 * @name    Specyfic Structores for containing data
*/
typedef struct 
{
    valParShort_int16_t   RPM;  // 2Byte RPM (0-16000rpm)
    valParShort_uint8_t   TPS;  // 1B TPS (0-100%)
    valParShort_uint8_t   IAT;  // 1B IAT (-40-127 C)
    valParShort_int16_t   MAP;  // 2B MAP (0-600 kPa)
    valParShort_int16_t   VSPD; // 2Byte VSPD (0-400 km/h)
}datShortCAN1_t;

typedef struct 
{
    valParShort_int16_t  AIN1;  // 2B AIN #1 (0-5 V)
    valParShort_int16_t  AIN2;  // 2B AIN #2 (0-5 V)
    valParShort_int16_t  AIN3;  // 2B AIN #3 (0-5 V)
    valParShort_int16_t  AIN4;  // 2B AIN #4 (0-5 V)
}datShortCAN2_t;

typedef struct
{// Structures collection for package3
    valParShort_int16_t  GPS_Speed;// 2Byte VSPD (0-400 km/h)
    valParShort_uint8_t  BARO;   // 1B BARO (50-130 kPa)
    valParShort_uint8_t  OILT;   // 1B OILT (0-160 C)
    valParShort_uint8_t  OILP;   // 1B OILP (0-12 Bar)
    valParShort_uint8_t  FUELP;  // 1B FUELP (0-7 Bar)
    valParShort_int16_t  CLT;    // 2B CLT (-40 - 250 C)
}datShortCAN3_t;

typedef struct 
{// Structures collection for package4
    valParShort_uint8_t  Ign_Angle;// 1B Ign Angle (-60-60 Deg) 0.5deg/bit
    valParShort_uint8_t  Dwell;  // 1B DWELL (0-10 ms)
    valParShort_uint8_t  Lambda; // 1B LAMBDA (0-2 Lambda)
    valParShort_uint8_t  AFR;    // 1B LAMBDA Correction (75-12t %)
    valParShort_int16_t  EGT_1;  // 2B EGT1 (0-1100 C)
    valParShort_int16_t  EGT_2;  // 2B EGT1 (0-1100 C)
}datShortCAN4_t;

typedef struct 
{// Structures collection for package5
    valParShort_uint8_t  Gear;      // 1B Gear ( 0 - 7 )
    valParShort_uint8_t  ECU_Temp;  // 1B ECU TEMP (-40 - 120 C)
    valParShort_int16_t  Battery;   // 2B Battery (0-20 V)
}datShortCAN5_t;

typedef struct 
{// Structures collection for package6
    valParShort_uint8_t  DBW_Pos;    // 1B DBW Pos ( 0 - 100 % )
    valParShort_uint8_t  DBW_Target; // 1B DBW TRGT ( 0 - 100 % )
    valParShort_int16_t  TC_DRPM_RAW;// 2B TC DRPM RAW (0-1000)
    valParShort_int16_t  TC_DRPM;    // 2B TC DRPM (0-400)
    valParShort_uint8_t  TC_TORQ_REDU;// 1B TC Torque Reduction (0-100 %)
    valParShort_uint8_t  PIT_TRQ_REDU;// 1B PIT Limit torque reduction (0-100 %)
}datShortCAN6_t;

typedef struct 
{// Structures collection for package7
    valParShort_int16_t  AIN5;       // 2B AIN #5 (0 - 5 V)
    valParShort_int16_t  AIN6;       // 2B AIN #6 (0 - 5 V)
    //valParShort_uint8_t  OUT_FLAG1;  // 1B OUT FLAGS1 (see EMU Manual)
    //valParShort_uint8_t  OUT_FLAG2;  // 1B OUT FLAGS2
    //valParShort_uint8_t  OUT_FLAG3;  // 1B OUT FLAGS3
    //valParShort_uint8_t  OUT_FLAG4;  // 1B OUT FLAGS4
}datShortCAN7_t;
//============================================================================

/*****************************************************************************
 * @name    Limit values structure
 * @note    These structures need to saved in EEPROM
*/
#define IDX_1B_OILP      0
#define IDX_1B_OILT      1
#define IDX_1B_FUELP     2
#define IDX_1B_LAMBDA    3
#define IDX_1B_AFR       4

#define IDX_2B_VSPD      0
#define IDX_2B_RPM       1
#define IDX_2B_EGT1      2
#define IDX_2B_EGT2      3

typedef struct 
{// This is importatnt limit values that will be saved in EEPROM memory
    uint8_t dataInitialized; // If initialized TRUE(0xFF)

    //uint8_t OILP;     // Only max value
    //uint8_t OILT;     // Only max value
    //uint8_t FUELP;    // Only max value
    //uint8_t LAMBDA;   // Only max value
    //uint8_t AFR;      // Only max value

    //int16_t VSPD;     // Only max value
    //int16_t RPM;      // Only max value
    //int16_t EGT1;     // Only max value
    //int16_t EGT2;     // Only max value

    // For better data menagment
    uint8_t dat1B[5];       
    int16_t dat2B[4];
    int16_t CLT[2];     // Max and min
}limitValues_t;
//============================================================================
#endif // _CUSTOM_STRUCTURES_H