#define EXIT_LIB

#ifndef EXIT_LIB
#include <EEPROM.h>
#include <stdint.h>
#ifndef _MY_ROM_H
#define _MY_ROM_H

class EepromExtenson: private EEPROMClass
{
private:
    // Change data offset when EEPROM reaches 100 000 write cycles 
    uint16_t startPointer;   // EEPROM pointer start write data
    uint16_t dataLenght;     // EEPROM data lenght in bytes
   
public:
    EepromExtenson(uint16_t inStartPointer, uint16_t inDataLenght, uint8_t inDataOffset);
    ~EepromExtenson(void);

    uint8_t ReadDataStructure(uint8_t* pData);
    uint8_t UpdateDatastructure(uint8_t* pData);
};

#endif // _MY_ROM_H
#endif