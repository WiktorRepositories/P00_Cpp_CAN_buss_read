#include "MyROM.h"
#ifndef EXIT_LIB

/*********************************************************************************************************
 * @name    EepromExtenson 
 * @brief   Class constructor
 * @param   inStartPointer ROM data starting pointer
 * @param   inDataLenght Structure enght in bytes
 * @param   inDataOffset Data offset for starting pointer in order to better usage EEPROM memory
 * @note    This class inherit EEPROMClass that is normaly set as EEPROM. 
 *          In this case this class is conected to custom class.
*/
EepromExtenson::EepromExtenson(uint16_t inStartPointer, uint16_t inDataLenght, uint8_t inDataOffset)
{
    startPointer    = inStartPointer + inDataOffset;
    dataLenght      = inDataLenght;
}
//========================================================================================================

/*********************************************************************************************************
 * @name    EepromExtenson     
 * @brief   Class deconstructor
 * @param   void
 * @note    void
 * @return  void
*/
EepromExtenson::~EepromExtenson(void)
{;}
//========================================================================================================

/*********************************************************************************************************
 * @name    ReadData
 * @brief   Read whole data structure from EEPROM memory
 * @param   pData Start pointer to destination data
 * @note    This method take some time depend on data lenght 
 * @return  TRUE or FALSE when data is read or error ocured
*/
uint8_t EepromExtenson::ReadDataStructure(uint8_t* pData)
{
    uint16_t i;
    for (i=0; i<dataLenght; i++)
    {
        *pData = read(startPointer + i);
        pData++;
    }
    return 0xFF;
}
//========================================================================================================

/*********************************************************************************************************
 * @name    Write data
 * @brief   Write whole data structure to EEPROM memory
 * @param   pData Start pointer to destination data
 * @note    Writes  when values are diffrent. Write takes 3,3 ms
 * @return  TRUE or FALSE when data is write or error ocured
*/
uint8_t EepromExtenson::UpdateDatastructure(uint8_t* pData)
{
    uint16_t i;
    for(i=0; i<dataLenght; i++)
    {
        update(startPointer + i, *pData);
        pData++;
    }
    return 0xFF;
}
//========================================================================================================

#endif