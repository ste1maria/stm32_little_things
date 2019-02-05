#include "flash.h"

void internal_flash_init(void){
	FLASH->KEYR = FLASH_KEY1;                               //Unlock FLASH access
  FLASH->KEYR = FLASH_KEY2; 
}

void Internal_Flash_Erase(unsigned int pageAddress) {
	while (FLASH->SR & FLASH_SR_BSY);
	if (FLASH->SR & FLASH_SR_EOP) {
		FLASH->SR = FLASH_SR_EOP;
	}

	FLASH->CR |= FLASH_CR_PER;
	FLASH->AR = pageAddress;
	FLASH->CR |= FLASH_CR_STRT;
	while (!(FLASH->SR & FLASH_SR_EOP));
	FLASH->SR = FLASH_SR_EOP;
	FLASH->CR &= ~FLASH_CR_PER;
}

void Internal_Flash_Write(unsigned char * data, unsigned int address, unsigned int count) {
	unsigned int i;

	while (FLASH->SR & FLASH_SR_BSY);
	if (FLASH->SR & FLASH_SR_EOP) {
		FLASH->SR = FLASH_SR_EOP;
	}

	FLASH->CR |= FLASH_CR_PG;
	
	for (i = 0; i < count; i += 2) {
		*(volatile unsigned short*)(address + i) = (((unsigned short)data[i + 1]) << 8) + data[i];
		while (!(FLASH->SR & FLASH_SR_EOP));
		FLASH->SR = FLASH_SR_EOP;
	}
	

	FLASH->CR &= ~(FLASH_CR_PG);
}


