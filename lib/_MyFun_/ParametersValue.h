#ifndef _CAN_DEFINITIONS_H
#define _CAN_DEFINITIONS_H

#define MODEL_X

/****************************************************************************************************
 * @brief   Definitions of MIN and MAX values for each parameter for MODEL_X
*/
#ifdef MODEL_X 

#define CAN_DAT_PAR_RANGE_RPM_MIN   20
#define CAN_DAT_PAR_RANGE_RPM_MAX   2000

#define CAN_DAT_PAR_RANGE_TPS_MIN   0
#define CAN_DAT_PAR_RANGE_TPS_MAX   100

#define CAN_DAT_PAR_RANGE_IAT_MIN   -40
#define CAN_DAT_PAR_RANGE_IAT_MAX   127

#define CAN_DAT_PAR_RANGE_MAP_MIN   0
#define CAN_DAT_PAR_RANGE_MAP_MAX   600

#define CAN_DAT_PAR_RANGE_VSPD_MIN  0
#define CAN_DAT_PAR_RANGE_VSPD_MAX  400

//---------------------------------------------

#define CAN_DAT_PAR_RANGE_AINx_MIN   0
#define CAN_DAT_PAR_RANGE_AINx_MAX   5

//---------------------------------------------

#define CAN_DAT_PAR_RANGE_GPSSPEED_MIN   0
#define CAN_DAT_PAR_RANGE_GPSSPEED_MAX   400

#define CAN_DAT_PAR_RANGE_BARO_MIN   50
#define CAN_DAT_PAR_RANGE_BARO_MAX   130

#define CAN_DAT_PAR_RANGE_OILT_MIN   0
#define CAN_DAT_PAR_RANGE_OILT_MAX   160

#define CAN_DAT_PAR_RANGE_OILP_MIN   0
#define CAN_DAT_PAR_RANGE_OILP_MAX   12

#define CAN_DAT_PAR_RANGE_FUELP_MIN   0
#define CAN_DAT_PAR_RANGE_FUELP_MAX   7

#define CAN_DAT_PAR_RANGE_CLT_MIN   -40
#define CAN_DAT_PAR_RANGE_CLT_MAX   250

//---------------------------------------------

#define CAN_DAT_PAR_RANGE_IGNANGLE_MIN   (-60)
#define CAN_DAT_PAR_RANGE_IGNANGLE_MAX   60

#define CAN_DAT_PAR_RANGE_DWELL_MIN   0
#define CAN_DAT_PAR_RANGE_DWELL_MAX   10

#define CAN_DAT_PAR_RANGE_LAMBDA_MIN   0
#define CAN_DAT_PAR_RANGE_LAMBDA_MAX   2

#define CAN_DAT_PAR_RANGE_AFR_MIN   75
#define CAN_DAT_PAR_RANGE_AFR_MAX   120

#define CAN_DAT_PAR_RANGE_EGTx_MIN   0
#define CAN_DAT_PAR_RANGE_EGTx_MAX   1100

//---------------------------------------------

#define CAN_DAT_PAR_RANGE_GEAR_MIN   0
#define CAN_DAT_PAR_RANGE_GEAR_MAX   7

#define CAN_DAT_PAR_RANGE_ECUT_MIN   (-40)
#define CAN_DAT_PAR_RANGE_ECUT_MAX   120

#define CAN_DAT_PAR_RANGE_BATT_MIN   0
#define CAN_DAT_PAR_RANGE_BATT_MAX   20

//---------------------------------------------

#define CAN_DAT_PAR_RANGE_DWBPOS_MIN   0
#define CAN_DAT_PAR_RANGE_DWBPOS_MAX   100

#define CAN_DAT_PAR_RANGE_DWBTARG_MIN   0
#define CAN_DAT_PAR_RANGE_DWBTARG_MAX   100

#define CAN_DAT_PAR_RANGE_TCDRMRAW_MIN   0
#define CAN_DAT_PAR_RANGE_TCDRMRAW_MAX   1000

#define CAN_DAT_PAR_RANGE_TCDRPM_MIN    0
#define CAN_DAT_PAR_RANGE_TCDRPM_MAX    400

#define CAN_DAT_PAR_RANGE_TCTRQREDU_MIN   0
#define CAN_DAT_PAR_RANGE_TCTRQREDU_MAX   100

#define CAN_DAT_PAR_RANGE_PITTRQREDU_MIN   0
#define CAN_DAT_PAR_RANGE_PITTRQREDU_MAX   100

#endif
//===================================================================================================

/****************************************************************************************************
 * @brief   Definitions of MIN and MAX values for each parameter for MODEL_Y
*/
#ifdef MODEL_Y

#define CAN_DAT_PAR_RANGE_RPM_MIN   0
#define CAN_DAT_PAR_RANGE_RPM_MAX   16000

#define CAN_DAT_PAR_RANGE_TPS_MIN   0
#define CAN_DAT_PAR_RANGE_TPS_MAX   100

#define CAN_DAT_PAR_RANGE_IAT_MIN   -40
#define CAN_DAT_PAR_RANGE_IAT_MAX   127

#define CAN_DAT_PAR_RANGE_MAP_MIN   0
#define CAN_DAT_PAR_RANGE_MAP_MAX   600

#define CAN_DAT_PAR_RANGE_VSPD_MIN  0
#define CAN_DAT_PAR_RANGE_VSPD_MAX  400

//---------------------------------------

#define CAN_DAT_PAR_RANGE_AINx_MIN   0
#define CAN_DAT_PAR_RANGE_AINx_MAX   5

//---------------------------------------

#define CAN_DAT_PAR_RANGE_GPSSPEED_MIN   0
#define CAN_DAT_PAR_RANGE_GPSSPEED_MAX   400

#define CAN_DAT_PAR_RANGE_BARO_MIN   50
#define CAN_DAT_PAR_RANGE_BARO_MAX   130

#define CAN_DAT_PAR_RANGE_OILT_MIN   0
#define CAN_DAT_PAR_RANGE_OILT_MAX   160

#define CAN_DAT_PAR_RANGE_OILP_MIN   0
#define CAN_DAT_PAR_RANGE_OILP_MAX   12

#define CAN_DAT_PAR_RANGE_FUELP_MIN   0
#define CAN_DAT_PAR_RANGE_FUELP_MAX   7

#define CAN_DAT_PAR_RANGE_CLT_MIN   -40
#define CAN_DAT_PAR_RANGE_CLT_MAX   250

//---------------------------------------

#define CAN_DAT_PAR_RANGE_IGNANGLE_MIN   (-60)
#define CAN_DAT_PAR_RANGE_IGNANGLE_MAX   60

#define CAN_DAT_PAR_RANGE_DWELL_MIN   0
#define CAN_DAT_PAR_RANGE_DWELL_MAX   10

#define CAN_DAT_PAR_RANGE_LAMBDA_MIN   0
#define CAN_DAT_PAR_RANGE_LAMBDA_MAX   2

#define CAN_DAT_PAR_RANGE_AFR_MIN   75
#define CAN_DAT_PAR_RANGE_AFR_MAX   120

#define CAN_DAT_PAR_RANGE_EGTx_MIN   0
#define CAN_DAT_PAR_RANGE_EGTx_MAX   1100

//---------------------------------------

#define CAN_DAT_PAR_RANGE_GEAR_MIN   0
#define CAN_DAT_PAR_RANGE_GEAR_MAX   7

#define CAN_DAT_PAR_RANGE_ECUT_MIN   (-40)
#define CAN_DAT_PAR_RANGE_ECUT_MAX   120

#define CAN_DAT_PAR_RANGE_BATT_MIN   0
#define CAN_DAT_PAR_RANGE_BATT_MAX   20

//---------------------------------------

#define CAN_DAT_PAR_RANGE_DWBPOS_MIN   0
#define CAN_DAT_PAR_RANGE_DWBPOS_MAX   100

#define CAN_DAT_PAR_RANGE_DWBTARG_MIN   0
#define CAN_DAT_PAR_RANGE_DWBTARG_MAX   100

#define CAN_DAT_PAR_RANGE_TCDRMRAW_MIN   0
#define CAN_DAT_PAR_RANGE_TCDRMRAW_MAX   1000

#define CAN_DAT_PAR_RANGE_TCDRPM_MIN   0
#define CAN_DAT_PAR_RANGE_TCDRPM_MAX   400

#define CAN_DAT_PAR_RANGE_TCTRQREDU_MIN   0
#define CAN_DAT_PAR_RANGE_TCTRQREDU_MAX   100

#define CAN_DAT_PAR_RANGE_PITTRQREDU_MIN   0
#define CAN_DAT_PAR_RANGE_PITTRQREDU_MAX   100

#endif
//===================================================================================================

/****************************************************************************************************
 * @brief   Definitions of MIN and MAX values for each parameter for MODEL_Z
*/
#ifdef MODEL_Z

#define CAN_DAT_PAR_RANGE_RPM_MIN   0
#define CAN_DAT_PAR_RANGE_RPM_MAX   16000

#define CAN_DAT_PAR_RANGE_TPS_MIN   0
#define CAN_DAT_PAR_RANGE_TPS_MAX   100

#define CAN_DAT_PAR_RANGE_IAT_MIN   -40
#define CAN_DAT_PAR_RANGE_IAT_MAX   127

#define CAN_DAT_PAR_RANGE_MAP_MIN   0
#define CAN_DAT_PAR_RANGE_MAP_MAX   600

#define CAN_DAT_PAR_RANGE_VSPD_MIN  0
#define CAN_DAT_PAR_RANGE_VSPD_MAX  400

//---------------------------------------

#define CAN_DAT_PAR_RANGE_AINx_MIN   0
#define CAN_DAT_PAR_RANGE_AINx_MAX   5

//---------------------------------------

#define CAN_DAT_PAR_RANGE_GPSSPEED_MIN   0
#define CAN_DAT_PAR_RANGE_GPSSPEED_MAX   400

#define CAN_DAT_PAR_RANGE_BARO_MIN   50
#define CAN_DAT_PAR_RANGE_BARO_MAX   130

#define CAN_DAT_PAR_RANGE_OILT_MIN   0
#define CAN_DAT_PAR_RANGE_OILT_MAX   160

#define CAN_DAT_PAR_RANGE_OILP_MIN   0
#define CAN_DAT_PAR_RANGE_OILP_MAX   12

#define CAN_DAT_PAR_RANGE_FUELP_MIN   0
#define CAN_DAT_PAR_RANGE_FUELP_MAX   7

#define CAN_DAT_PAR_RANGE_CLT_MIN   -40
#define CAN_DAT_PAR_RANGE_CLT_MAX   250

//---------------------------------------

#define CAN_DAT_PAR_RANGE_IGNANGLE_MIN   (-60)
#define CAN_DAT_PAR_RANGE_IGNANGLE_MAX   60

#define CAN_DAT_PAR_RANGE_DWELL_MIN   0
#define CAN_DAT_PAR_RANGE_DWELL_MAX   10

#define CAN_DAT_PAR_RANGE_LAMBDA_MIN   0
#define CAN_DAT_PAR_RANGE_LAMBDA_MAX   2

#define CAN_DAT_PAR_RANGE_AFR_MIN   75
#define CAN_DAT_PAR_RANGE_AFR_MAX   120

#define CAN_DAT_PAR_RANGE_EGTx_MIN   0
#define CAN_DAT_PAR_RANGE_EGTx_MAX   1100

//---------------------------------------

#define CAN_DAT_PAR_RANGE_GEAR_MIN   0
#define CAN_DAT_PAR_RANGE_GEAR_MAX   7

#define CAN_DAT_PAR_RANGE_ECUT_MIN   (-40)
#define CAN_DAT_PAR_RANGE_ECUT_MAX   120

#define CAN_DAT_PAR_RANGE_BATT_MIN   0
#define CAN_DAT_PAR_RANGE_BATT_MAX   20

//---------------------------------------

#define CAN_DAT_PAR_RANGE_DWBPOS_MIN   0
#define CAN_DAT_PAR_RANGE_DWBPOS_MAX   100

#define CAN_DAT_PAR_RANGE_DWBTARG_MIN   0
#define CAN_DAT_PAR_RANGE_DWBTARG_MAX   100

#define CAN_DAT_PAR_RANGE_TCDRMRAW_MIN   0
#define CAN_DAT_PAR_RANGE_TCDRMRAW_MAX   1000

#define CAN_DAT_PAR_RANGE_TCDRPM_MIN   0
#define CAN_DAT_PAR_RANGE_TCDRPM_MAX   400

#define CAN_DAT_PAR_RANGE_TCTRQREDU_MIN   0
#define CAN_DAT_PAR_RANGE_TCTRQREDU_MAX   100

#define CAN_DAT_PAR_RANGE_PITTRQREDU_MIN   0
#define CAN_DAT_PAR_RANGE_PITTRQREDU_MAX   100

#endif
//===================================================================================================

#endif //_CAN_DEFINITIONS_h