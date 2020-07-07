#include <stdbool.h>
#include <stdint.h>
#include "1wire.h"

/** 
  * @brief Searchs the next device on the bus and reads its rom
  *
  * @note 
  *
  * @param driver: One wire driver implementation
  * @param dev: Pointer on the empty OneWireDevice
  * @param command: Sending byte before, for example: SEARCH_ROM
  */
OneWireStatus OneWire_NextDevice(OneWireDriver *driver, OneWireDevice *dev, uint8_t command) {
  uint8_t bit, ibit;
  
  // Checks whether the device is on the bus. 
  if (driver->SendPresence(driver)) {
    uint8_t numBit = 0;
    driver->WriteByte(driver, command);
    
    uint8_t curCol = 0;
    uint8_t crc = 0;
    for (uint8_t j = 0; j < SIZE_OF_ROM; j++) {
      for (uint8_t k = 0; k < 8; k++) {
        bit = driver->ReadBit(driver);
        ibit = driver->ReadBit(driver);
        if (bit & ibit) {
          //No devices
          return ONE_WIRE_NO_DEVICES;
        } else if (!(bit | ibit)) {
          // Collision
          if (numBit < driver->_lastCol) {
            bit = (driver->_prevDev->rom[j] >> k) & 1;
            // Remeber index of collision
            curCol = numBit;
          } else if (driver->_lastCol == numBit) {
            // Choose the other branch
            bit = 1;
          } else {
            // Choose the first branch
            curCol = numBit;
          }
        }
        // Writes chosen bit
        driver->WriteBit(driver, bit);
        // And remember it
        dev->rom[j] |= bit << k;
        numBit++;
      } 
      // Calculate crc sum
      crc = NextCrc_8bit(crc, dev->rom[j]);
    }
    
    if (crc != 0) return ONE_WIRE_CRC_ERROR;
    
    driver->_lastCol = curCol;
    driver->_prevDev = dev;
    
  } else {
    return ONE_WIRE_NO_DEVICES;
  }
  
  return ONE_WIRE_OK;
}

OneWireStatus OneWire_MatchRom(OneWireDriver *driver, OneWireDevice *dev) {
  if (!driver->SendPresence(driver)) return ONE_WIRE_NO_DEVICES;
  
  driver->WriteByte(driver, MATCH_ROM);
  for (uint8_t i = 0; i < SIZE_OF_ROM; i++) {
    driver->WriteByte(driver, dev->rom[i]);
  }
  
  return ONE_WIRE_OK;
}
