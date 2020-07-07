#ifndef __DS18X20_H
#define __DS18X20_H

#include <stdint.h>
#include "1wire.h"

#define CONVERT_T               0x44
#define WRITE_SCRATCHPAD        0x4E
#define COPY_SCRATCHPAD         0x48
#define READ_SCRATCHPAD         0xBE
#define RECALL_E                0xB8
#define READ_POWER_SUPPLY       0xB4

#define SIZE_OF_SCRATCHPAD 9

/*typedef enum {
  DS18B20 = 0x28,
  DS18S20 = 0x10
} SensType;*/

#define DS18B20 0x28
#define DS18S20 0x10

#define DS18x20_IsConvComplete(driver) ((driver)->ReadBit((driver)))

OneWireStatus DS18x20_GetTemperature(OneWireDriver *driver, OneWireDevice *dev, int32_t *temp);
//OneWireStatus DS18x20_GetTemperatureW(OneWireDriver *driver, SensType t, int32_t *temp);
OneWireStatus DS18x20_ConvTemp(OneWireDriver *driver);
OneWireStatus DS18x20_ReadScr(OneWireDriver *driver, OneWireDevice *dev, uint8_t scr[SIZE_OF_SCRATCHPAD]);

#endif
