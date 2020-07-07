#include <stdint.h>
#include "DS18x20.h"

OneWireStatus DS18x20_ReadScr(OneWireDriver *driver, OneWireDevice *dev, uint8_t scr[SIZE_OF_SCRATCHPAD]) {
  if (OneWire_MatchRom(driver, dev) == ONE_WIRE_NO_DEVICES) return ONE_WIRE_NO_DEVICES;
  
  driver->WriteByte(driver, READ_SCRATCHPAD);
  
  uint8_t crc = 0;
  for (uint8_t j = 0; j < SIZE_OF_SCRATCHPAD; j++) {
    scr[j] = driver->ReadByte(driver);
    crc = NextCrc_8bit(crc, scr[j]);
  }
  if (crc != 0) return ONE_WIRE_CRC_ERROR;
  
  return ONE_WIRE_OK;
}

static int32_t GetTemp(uint8_t scr[SIZE_OF_SCRATCHPAD], uint8_t digit) {
  int32_t temp = scr[0] | (scr[1] << 8);
  
  if ((temp & 0x2000) != 0) {
		temp &= 0x7FF;
		temp = ((~temp) & 0x7FF) + 1;
	  temp = -(temp * 10000 >> digit);
  } else {
    temp = (temp * 10000 >> digit);
  }
  
  return temp;
}

OneWireStatus DS18x20_GetTemperature(OneWireDriver *driver, OneWireDevice *dev, int32_t *temp) {
  uint8_t scr[SIZE_OF_SCRATCHPAD];
  OneWireStatus status = ONE_WIRE_NO_DEVICES;
  
  status = DS18x20_ReadScr(driver, dev, scr);
  if (status != ONE_WIRE_OK) return status;
  
  if (dev->rom[0] == DS18B20) {
    uint8_t digit = ((scr[4] & 0x60) >> 5) + 1;
    *temp = GetTemp(scr, digit);
    status = ONE_WIRE_OK;
  } else if (dev->rom[0] == DS18S20) {
    *temp = GetTemp(scr, 1);
    status = ONE_WIRE_OK;
  }
  
  return status;
}

OneWireStatus DS18x20_ConvTemp(OneWireDriver *driver) {
  if (!driver->SendPresence(driver)) return ONE_WIRE_NO_DEVICES;
  
  driver->WriteByte(driver, SKIP_ROM);
  driver->WriteByte(driver, CONVERT_T);
  
  return ONE_WIRE_OK;
}
