#include "MyCAN_v2.h"
/*********************************************************************************************************************************************
    @brief  Class constructor
    @param  refCAN1 Reference to decoded data package1 
    @param  refCAN2 Reference to decoded data package2 
    @param  refCAN3 Reference to decoded data package3 
    @param  refCAN4 Reference to decoded data package4 
    @param  refCAN5 Reference to decoded data package5 
    @param  refCAN6 Reference to decoded data package6 
    @param  refCAN7 Reference to decoded data package7 
*/
DecodeCANDataClass::DecodeCANDataClass(datShortCAN1_t& refCAN1, datShortCAN2_t& refCAN2, datShortCAN3_t& refCAN3, datShortCAN4_t& refCAN4, datShortCAN5_t& refCAN5, datShortCAN6_t& refCAN6, datShortCAN7_t& refCAN7, limitValues_t& refLimVal):
    refPackage1(refCAN1),
    refPackage2(refCAN2),
    refPackage3(refCAN3),
    refPackage4(refCAN4),
    refPackage5(refCAN5),
    refPackage6(refCAN6),
    refPackage7(refCAN7),
    reflimitValues(refLimVal) 
{}
//============================================================================================================================================

/*********************************************************************************************************************************************
   @brief   Class dekonctructor
   @param   pdata      Pointer to parameter type struture). 
*/
DecodeCANDataClass::~DecodeCANDataClass(void)
{;}
//============================================================================================================================================

/*********************************************************************************************************************************************
   @brief   Check paremeter value is it is in min max range
   @param   pdata Pointer to parameter type struture). 
   @return  void
   @note    Posible imputs: uint8_t
*/
void DecodeCANDataClass::CheckValues(valParShort_uint8_t* pdata, uint8_t inMIN, uint8_t inMAX)
{
    if(pdata->value == 0x00) 
    {   pdata->fault = CAN_DAT_PAR_ERROR_NODAT;}
    else if(pdata->value <= inMIN) 
    {   pdata->fault = CAN_DAT_PAR_ERROR_MIN;}
    else if(pdata->value >= inMAX) 
    {   pdata->fault = CAN_DAT_PAR_ERROR_MAX;}
    else 
    {   pdata->fault = CAN_DAT_PAR_ERROR_OK;}
}
//============================================================================================================================================

/*********************************************************************************************************************************************
   @brief   Check paremeter value is it is in min max range
   @param   pdata Pointer to parameter type struture). 
   @return  void
   @note    Posible imputs: int16_t
*/
void DecodeCANDataClass::CheckValues(valParShort_int16_t* pdata, int16_t inMIN, int16_t inMAX)
{
    if(pdata->value == 0x0000) 
    {   pdata->fault = CAN_DAT_PAR_ERROR_NODAT;}
    else if(pdata->value <= inMIN) 
    {   pdata->fault = CAN_DAT_PAR_ERROR_MIN;}
    else if(pdata->value >= inMAX) 
    {   pdata->fault = CAN_DAT_PAR_ERROR_MAX;}
    else 
    {   pdata->fault = CAN_DAT_PAR_ERROR_OK;}
}
//============================================================================================================================================

/*********************************************************************************************************************************************
   @brief   Check paremeter value is it is in min max range
   @param   pdata      Pointer to parameter type struture). 
   @return  void
   @note    Posible imputs: float
*/
void DecodeCANDataClass::CheckValues(valParShort_float_t* pdata, float inMIN, float inMAX)
{
    if(pdata->value <= inMIN) 
    {   pdata->fault = CAN_DAT_PAR_ERROR_MIN;}
    else if(pdata->value >= inMAX) 
    {   pdata->fault = CAN_DAT_PAR_ERROR_MAX;}
    else 
    {   pdata->fault = CAN_DAT_PAR_ERROR_OK;}
}
//============================================================================================================================================

/*********************************************************************************************************************************************
   @brief   Coppy data from structure to structure
   @param   pSrcPtr Base pointer to source structure
   @param   pDestPtr Base pointer to destination structure
   @param   dataLenght Lenght data in byte. Use "sizeof()"
   @return  void
   @note    Double check data lenght
*/
void DecodeCANDataClass::MoveData(uint8_t* pSrcPtr, uint8_t* pDestPtr, uint8_t dataLenght)
{
    uint8_t i;
    for(i=0; i<dataLenght; i++)
    {   *(pDestPtr + i) = *(pSrcPtr + i);}
}
//============================================================================================================================================

/*********************************************************************************************************************************************
   @brief   Decodes CAN data inside pBuffer to valid parameters
   @param   pBuffer[]   CAN data pBuffer
   @return  void
   @note    Custom for package 1
*/
void DecodeCANDataClass::DecodePackage1(uint8_t* pBuffer)
{
    refPackage1.RPM.value = static_cast<int16_t>((pBuffer[0] << 8) | (pBuffer[1] << 0));
    refPackage1.TPS.value = (pBuffer[2]);
    refPackage1.IAT.value = (pBuffer[3]);
    refPackage1.MAP.value = static_cast<int16_t>((pBuffer[4] << 8) | (pBuffer[5] << 0));
   refPackage1.VSPD.value = static_cast<int16_t>((pBuffer[6] << 8) | (pBuffer[7] << 0));

    CheckValues(&(refPackage1.RPM),     CAN_DAT_PAR_RANGE_RPM_MIN,      reflimitValues.dat2B[IDX_2B_RPM]);
    CheckValues(&(refPackage1.TPS),     CAN_DAT_PAR_RANGE_TPS_MIN,      CAN_DAT_PAR_RANGE_TPS_MAX);
    CheckValues(&(refPackage1.IAT),     CAN_DAT_PAR_RANGE_IAT_MIN,      CAN_DAT_PAR_RANGE_IAT_MAX);
    CheckValues(&(refPackage1.MAP),     CAN_DAT_PAR_RANGE_MAP_MIN,      CAN_DAT_PAR_RANGE_MAP_MAX);
    CheckValues(&(refPackage1.VSPD),    CAN_DAT_PAR_RANGE_VSPD_MIN,     reflimitValues.dat2B[IDX_2B_VSPD]);
}
//============================================================================================================================================

/*********************************************************************************************************************************************
   @brief   Decodes CAN data inside buffer to valid parameters
   @param   buffer[]   CAN data buffer
   @return  void
   @note    Custom for package 2
*/
void DecodeCANDataClass::DecodePackage2(uint8_t* pBuffer)
{
    refPackage2.AIN1.value = static_cast<int16_t>((pBuffer[0] << 8) | (pBuffer[1] << 0));
    refPackage2.AIN2.value = static_cast<int16_t>((pBuffer[2] << 8) | (pBuffer[3] << 0));
    refPackage2.AIN3.value = static_cast<int16_t>((pBuffer[4] << 8) | (pBuffer[5] << 0));
    refPackage2.AIN4.value = static_cast<int16_t>((pBuffer[6] << 8) | (pBuffer[7] << 0));

    CheckValues(&(refPackage2.AIN1), CAN_DAT_PAR_RANGE_AINx_MIN, CAN_DAT_PAR_RANGE_AINx_MAX);
    CheckValues(&(refPackage2.AIN2), CAN_DAT_PAR_RANGE_AINx_MIN, CAN_DAT_PAR_RANGE_AINx_MAX);
    CheckValues(&(refPackage2.AIN3), CAN_DAT_PAR_RANGE_AINx_MIN, CAN_DAT_PAR_RANGE_AINx_MAX);
    CheckValues(&(refPackage2.AIN4), CAN_DAT_PAR_RANGE_AINx_MIN, CAN_DAT_PAR_RANGE_AINx_MAX);
}
//============================================================================================================================================

/*********************************************************************************************************************************************
   @brief   Decodes CAN data inside pBuffer to valid parameters
   @param   pBuffer[]   CAN data pBuffer
   @return  void
   @note    Custom for package 3
*/
void DecodeCANDataClass::DecodePackage3(uint8_t* pBuffer)
{
    refPackage3.GPS_Speed.value   = static_cast<int16_t>((pBuffer[0] << 8) | (pBuffer[1] << 0));
    refPackage3.BARO.value        = static_cast<uint8_t>(pBuffer[2]);
    refPackage3.OILT.value        = static_cast<uint8_t>(pBuffer[3]);
    refPackage3.OILP.value        = static_cast<uint8_t>(pBuffer[4] / 10);
    refPackage3.FUELP.value       = static_cast<uint8_t>(pBuffer[5] / 10);
    refPackage3.CLT.value         = static_cast<int16_t>((pBuffer[6] << 8) | (pBuffer[7] << 0));

    CheckValues(&(refPackage3.GPS_Speed),   CAN_DAT_PAR_RANGE_GPSSPEED_MIN,     CAN_DAT_PAR_RANGE_GPSSPEED_MAX);
    CheckValues(&(refPackage3.BARO),        CAN_DAT_PAR_RANGE_BARO_MIN,         CAN_DAT_PAR_RANGE_BARO_MAX);
    CheckValues(&(refPackage3.OILT),        CAN_DAT_PAR_RANGE_OILT_MIN,         reflimitValues.dat1B[IDX_1B_OILT]);
    CheckValues(&(refPackage3.OILP),        CAN_DAT_PAR_RANGE_OILP_MIN,         reflimitValues.dat1B[IDX_1B_OILP]);
    CheckValues(&(refPackage3.FUELP),       CAN_DAT_PAR_RANGE_FUELP_MIN,        reflimitValues.dat1B[IDX_1B_FUELP]);
    CheckValues(&(refPackage3.CLT),         reflimitValues.CLT[0],              reflimitValues.CLT[1]);
}
//============================================================================================================================================

/*********************************************************************************************************************************************
   @brief   Decodes CAN data inside pBuffer to valid parameters
   @param   pBuffer[]   CAN data pBuffer
   @return  void
   @note    Custom for package 4
*/
void DecodeCANDataClass::DecodePackage4(uint8_t* pBuffer)
{
    refPackage4.Ign_Angle.value  = static_cast<uint8_t>(pBuffer[0]);
    refPackage4.Dwell.value      = static_cast<uint8_t>(pBuffer[1]);
    refPackage4.Lambda.value     = static_cast<uint8_t>(pBuffer[2]);
    refPackage4.AFR.value        = static_cast<uint8_t>(pBuffer[3]);
    refPackage4.EGT_1.value      = static_cast<int16_t>((pBuffer[4] << 8) | (pBuffer[5] << 0));
    refPackage4.EGT_2.value      = static_cast<int16_t>((pBuffer[6] << 8) | (pBuffer[1] << 0));

    CheckValues(&(refPackage4.Lambda),  CAN_DAT_PAR_RANGE_LAMBDA_MIN,   reflimitValues.dat1B[IDX_1B_LAMBDA]);
    CheckValues(&(refPackage4.AFR),     CAN_DAT_PAR_RANGE_AFR_MIN,      CAN_DAT_PAR_RANGE_AFR_MAX);
    CheckValues(&(refPackage4.EGT_1),   CAN_DAT_PAR_RANGE_EGTx_MIN,     reflimitValues.dat2B[IDX_2B_EGT1]);
    CheckValues(&(refPackage4.EGT_2),   CAN_DAT_PAR_RANGE_EGTx_MIN,     reflimitValues.dat2B[IDX_2B_EGT2]); 
}
//============================================================================================================================================

/*********************************************************************************************************************************************
   @brief   Decodes CAN data inside pBuffer to valid parameters
   @param   pBuffer[]   CAN data pBuffer
   @return  void
   @note    Custom for package 5
*/
void DecodeCANDataClass::DecodePackage5(uint8_t* pBuffer)
{
    refPackage5.Gear.value    = static_cast<uint8_t>(pBuffer[0]);
    refPackage5.ECU_Temp.value= static_cast<uint8_t>(pBuffer[1]);
    refPackage5.Battery.value = static_cast<int16_t>((pBuffer[2] << 8) | (pBuffer[3] << 0));

    CheckValues(&(refPackage5.Gear),        CAN_DAT_PAR_RANGE_GEAR_MIN, CAN_DAT_PAR_RANGE_GEAR_MAX);
    CheckValues(&(refPackage5.ECU_Temp),    CAN_DAT_PAR_RANGE_ECUT_MIN, CAN_DAT_PAR_RANGE_ECUT_MAX);
    CheckValues(&(refPackage5.Battery),     CAN_DAT_PAR_RANGE_BATT_MIN, CAN_DAT_PAR_RANGE_BATT_MAX);
}
//============================================================================================================================================

/*********************************************************************************************************************************************
   @brief   Decodes CAN data inside buffer to valid parameters
   @param   buffer[]   CAN data buffer
   @return  void
   @note    Custom for package 6
*/
void DecodeCANDataClass::DecodePackage6(uint8_t* pBuffer)
{
    refPackage6.DBW_Pos.fault     = static_cast<uint8_t>(pBuffer[0]);
    refPackage6.DBW_Target.value  = static_cast<uint8_t>(pBuffer[1]);
    refPackage6.TC_DRPM_RAW.value = static_cast<int16_t>((pBuffer[2] << 8) | (pBuffer[3] << 0));
    refPackage6.TC_DRPM.value     = static_cast<int16_t>((pBuffer[4] << 8) | (pBuffer[5] << 0));
    refPackage6.TC_TORQ_REDU.value= static_cast<uint8_t>(pBuffer[6]);
    refPackage6.PIT_TRQ_REDU.value= static_cast<uint8_t>(pBuffer[7]);

    CheckValues(&(refPackage6.DBW_Pos),         CAN_DAT_PAR_RANGE_DWBPOS_MIN,       CAN_DAT_PAR_RANGE_DWBPOS_MAX);
    CheckValues(&(refPackage6.DBW_Target),      CAN_DAT_PAR_RANGE_DWBTARG_MIN,      CAN_DAT_PAR_RANGE_DWBTARG_MAX);
    CheckValues(&(refPackage6.TC_DRPM_RAW),     CAN_DAT_PAR_RANGE_TCDRMRAW_MIN,     CAN_DAT_PAR_RANGE_TCDRMRAW_MAX);
    CheckValues(&(refPackage6.TC_DRPM),         CAN_DAT_PAR_RANGE_TCDRPM_MIN,       CAN_DAT_PAR_RANGE_TCDRPM_MAX);
    CheckValues(&(refPackage6.PIT_TRQ_REDU),    CAN_DAT_PAR_RANGE_PITTRQREDU_MIN,   CAN_DAT_PAR_RANGE_PITTRQREDU_MAX);
}
//============================================================================================================================================

/*********************************************************************************************************************************************
   @brief   Decodes CAN data inside buffer to valid parameters
   @param   pBuffer[]   CAN data pBuffer
   @return  void 
   @note    Custom for package 7
*/
void DecodeCANDataClass::DecodePackage7(uint8_t* pBuffer)
{
    refPackage7.AIN5.value = static_cast<int16_t>((pBuffer[0] << 8) | (pBuffer[1] << 0));
    refPackage7.AIN6.value = static_cast<int16_t>((pBuffer[2] << 8) | (pBuffer[3] << 0));

    CheckValues(&(refPackage7.AIN5), CAN_DAT_PAR_RANGE_AINx_MIN, CAN_DAT_PAR_RANGE_AINx_MAX);
    CheckValues(&(refPackage7.AIN6), CAN_DAT_PAR_RANGE_AINx_MIN, CAN_DAT_PAR_RANGE_AINx_MAX);
}
//============================================================================================================================================

/*********************************************************************************************************************************************
   @brief   Decodes CAN data inside pBuffer to valid parameters.
   @param   pDataFromCan Pointer of type (CAN_data_undecoded_t*).
   @return  void
   @note    When data comes it is decodec depend from ID.
*/
void DecodeCANDataClass::CallForReadCanData(CAN_data_undecoded_t* pCanData)
{           
    unsigned long tID;
    tID = pCanData->ID;
    switch (tID)
    {
    case MYCAN_CAN_ID_1: // Data1 decode and write to global data
        DecodePackage1(pCanData->arrData);
        break;

    case MYCAN_CAN_ID_2: // Data2 decode and write to global data
        DecodePackage2(pCanData->arrData);
        break;

    case MYCAN_CAN_ID_3: // Data3 decode and write to global data
        DecodePackage3(pCanData->arrData);
        break;

    case MYCAN_CAN_ID_4: // Data4 decode and write to global data
        DecodePackage4(pCanData->arrData);
        break;

    case MYCAN_CAN_ID_5: // Data5 decode and write to global data
        DecodePackage5(pCanData->arrData);
        break;

    case MYCAN_CAN_ID_6: // Data6 decode and write to global data
        DecodePackage6(pCanData->arrData);
        break;

    case MYCAN_CAN_ID_7: // Data7 decode and write to global data
        DecodePackage7(pCanData->arrData);
        break;
    
    default:
        break;
    }
}
//============================================================================================================================================

/*********************************************************************************************************************************************
   @brief   CAN communication procedure used pointer to MCP_CAN class.
   @param   pMcpCan  Pointer of type (MCP_CAN*) to initialized object.
   @param   pCanData Pointer of type (CAN_data_undecoded_t*) to recieved  data from CN buss.
   @return  void
   @note    This function decodes CAN data to global structure.
*/
void ReadCanDataBy_MCP_CAN(MCP_CAN* pMcpCan, CAN_data_undecoded_t* pCanData)
{ 
  byte  tLen;
  byte* pBuffer; 

  if(CAN_MSGAVAIL != pMcpCan->checkReceive() )
  { // Data is not recieved
    // Error code is assigned
    pCanData->ID = (unsigned long)(-1);
  }
  else
  { // Data is recieved
    tLen    = CAN_DATA_MAX_LEN;
    pBuffer = (byte*)(pCanData->arrData);

    pMcpCan->readMsgBuf(&tLen, pBuffer);
    pCanData->ID = pMcpCan->getCanId();
  }
}
//============================================================================================================================================
