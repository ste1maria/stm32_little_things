#ifndef __flash_H__
#define __flash_H__

#include "stm32f1xx.h"

void internal_flash_init(void);
void Internal_Flash_Erase(unsigned int pageAddress);
void Internal_Flash_Write(unsigned char* data, unsigned int address, unsigned int count) ;

#endif
