#include <stdint.h>
#include "mcp_can.h"
#include "CustomStructures.h"
#include "ParametersValue.h"

#ifndef _MY_CAN_H
#define _MY_CAN_H

/****************************************************************************************************
 * @brief   Definitions of CAN id data packages
*/
#define MYCAN_CAN_ID_1          600UL
#define MYCAN_CAN_ID_2          601UL
#define MYCAN_CAN_ID_3          602UL
#define MYCAN_CAN_ID_4          603UL
#define MYCAN_CAN_ID_5          604UL
#define MYCAN_CAN_ID_6          605UL
#define MYCAN_CAN_ID_7          606UL

#define MYCAN_CAN_BUFFER_SIZE   0x08
//===================================================================================================

/****************************************************************************************************
 * @name        DecodeCANDataClass
 * @brief       This class prepare data recieced by CAN buss to actual structures.
 * @note        This class is using pointers to global structures that contain decoded packages.
*/
class DecodeCANDataClass
{
private:
/*  Referance data for global structures */
    datShortCAN1_t& refPackage1;
    datShortCAN2_t& refPackage2;
    datShortCAN3_t& refPackage3;
    datShortCAN4_t& refPackage4;
    datShortCAN5_t& refPackage5;
    datShortCAN6_t& refPackage6;
    datShortCAN7_t& refPackage7;
    limitValues_t&  reflimitValues;

/*  Private methods */
    void CheckValues(valParShort_uint8_t* pdata, uint8_t inMAX, uint8_t inMIN);
    void CheckValues(valParShort_int16_t* pdata, int16_t inMAX, int16_t inMIN);
    void CheckValues(valParShort_float_t* pdata, float inMAX, float inMIN);
    void MoveData(uint8_t* pSrcPtr, uint8_t* pDestPtr, uint8_t dataLenght);

/*  Decode metods for each packages */
    void DecodePackage1(uint8_t* pBuffer);
    void DecodePackage2(uint8_t* pBuffer);
    void DecodePackage3(uint8_t* pBuffer);
    void DecodePackage4(uint8_t* pBuffer);
    void DecodePackage5(uint8_t* pBuffer);
    void DecodePackage6(uint8_t* pBuffer);
    void DecodePackage7(uint8_t* pBuffer);
//=============================================================

public:
/*  Class constructor and deconstructor */
    DecodeCANDataClass(datShortCAN1_t& refCAN1, datShortCAN2_t& refCAN2, datShortCAN3_t& refCAN3, datShortCAN4_t& refCAN4, datShortCAN5_t& refCAN5, datShortCAN6_t& refCAN6, datShortCAN7_t& refCAN7, limitValues_t& refLimVal);
    ~DecodeCANDataClass(void);
    
/*  Public call */
    void CallForReadCanData(CAN_data_undecoded_t* pCanData);
};

//=======================================================================================================
// Other custom functions definitions
//=======================================================================================================

/********************************************************************************************************
   @brief   CAN communication procedure used pointer to MCP_CAN class.
   @param   pMcpCan  Pointer of type (MCP_CAN*) to initialized object.
   @param   pCanData Pointer of type (CAN_data_undecoded_t*) to recieved  data from CN buss.
   @return  void
   @note    This function decodes CAN data to global structure.
*/
void ReadCanDataBy_MCP_CAN(MCP_CAN* pMcpCan, CAN_data_undecoded_t* pCanData); 
//=======================================================================================================

#endif //_MY_DEF_H