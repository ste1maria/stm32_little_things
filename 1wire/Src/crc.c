#include <stdint.h>

uint8_t NextCrc_8bit(uint8_t crc, uint8_t b) {
  for (uint8_t p = 8; p; p--) {
    if ((crc ^ b) & 1) crc = (crc >> 1) ^ 0x8C; 
    else crc >>= 1;
    b >>= 1;
  }
  return crc;
}

// CRC16 CCITT  version
uint16_t NextCrc_16bit(uint16_t crc, uint8_t data)
{
	uint16_t t;
	data ^= crc & 0xff;
	data ^= data << 4;
	t = (((uint16_t)data << 8) | ((crc>>8) & 0xff));
	t ^= (unsigned char)(data >> 4);
	t ^= ((uint16_t)data << 3);
	return t;
}
