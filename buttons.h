#ifndef _BUTTONS_H_
#define _BUTTONS_H_
#include "stm32f1xx.h"

void ButtonTimerReset(void);
uint16_t GetButtonTimerCount(void);

typedef enum{
 NO_BTTN			 = 0,
 BUTTON_START  = 1,
 BUTTON_UP		 = 2,
 BUTTON_DOWN	 = 3, 
 BUTTON_MUTE	 = 4, 
 BUTTON_MODE 	 = 5
} key_codes;


void ProcessKeyFSM(void);
void ScanKeyboard(void);
void ExecuteButton(key_codes keycode);

#endif
