#ifndef __1WIRE_H
#define __1WIRE_H

#include <stdbool.h>
#include <stdint.h>

#define SIZE_OF_ROM 8

#define SEARCH_ROM              0xF0
#define ALARM_SEARCH            0xEC
#define MATCH_ROM								0x55
#define SKIP_ROM                0xCC
#define READ_ROM								0x33

#define OneWire_AllDevices(bus) ((bus)->_lastCol == 0)

typedef enum {
  ONE_WIRE_OK = 0,
  ONE_WIRE_CRC_ERROR,
  ONE_WIRE_NO_DEVICES
} OneWireStatus;

typedef struct {
  uint8_t rom[SIZE_OF_ROM];
} OneWireDevice;

/** One wire driver abstract class
  * SendPresence sends reset signal and check presence of devices
  * ptr is pointer on local variables struct of driver implementation
  * _prevDev and _lastCol is used by the function NextDevice
 */
typedef struct OneWireDriver {
  bool (*SendPresence)(struct OneWireDriver*);
  void (*WriteBit)(struct OneWireDriver*, bool bit);
  bool (*ReadBit)(struct OneWireDriver*);
  void (*WriteByte)(struct OneWireDriver*, uint8_t byte);
  uint8_t (*ReadByte)(struct OneWireDriver*);
  
  OneWireDevice *_prevDev;
  uint8_t _lastCol;
  
  void *ptr;
} OneWireDriver;

OneWireStatus OneWire_NextDevice(OneWireDriver *driver, OneWireDevice *dev, uint8_t command);
OneWireStatus OneWire_MatchRom(OneWireDriver *driver, OneWireDevice *dev);

extern uint8_t NextCrc_8bit(uint8_t crc, uint8_t byte);

#endif
