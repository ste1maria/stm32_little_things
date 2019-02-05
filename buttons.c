#include "buttons.h"

#define DebounceTime			100		// delay for debouncing:  100 ms
#define FirstDelay				500		// delay for first reapeat if button is being hold
#define AutoRepeat				250		// delay for auto repeat if button is being hold

char key_state = 0;
key_codes key_code, key_code_prev;


volatile uint16_t ButtonTimerCount = 0;			// ticks every 1 ms - to count timeouts for button debouncing


/*____________________SCAN KEYBOARD FUNCTION__________________________________*/
void ScanKeyboard(void){
	
	// scanning itself. BUTTONS ARE ACTIVE LOW!!!!!!!!
	if (HAL_GPIO_ReadPin(GPIOA, BUTTON_UP_Pin)== GPIO_PIN_RESET) {
		key_code = BUTTON_UP;
	}
	else if (HAL_GPIO_ReadPin(GPIOA, BUTTON_DOWN_Pin)==GPIO_PIN_RESET) {
		key_code = BUTTON_DOWN;
	}
	else if (HAL_GPIO_ReadPin(GPIOB, BUTTON_MODE_Pin)==GPIO_PIN_RESET) {
		key_code = BUTTON_MODE;
	}
	else if (HAL_GPIO_ReadPin(GPIOB, BUTTON_MUTE_Pin)==GPIO_PIN_RESET) {
		key_code = BUTTON_MUTE;
	}
	else if (HAL_GPIO_ReadPin(GPIOB, BUTTON_START_Pin)==GPIO_PIN_RESET) {
		key_code = BUTTON_START;
	}
	else {
		key_code = NO_BTTN;
	}
	
	//debouncing and autorepeat processing
	ProcessKeyFSM();
	
	//executing the action assigned to the button that was pressed
	if (ButtonPressed){
		ExecuteButton(key_code);
		ButtonPressed = 0;
	}
} /*____________________________END OF SCAN KEYS FUNCTION______________________________________*/


/* Provides button pressing processing: 
* eliminates bounce;
* provides service for auto repeat (when you hold button for a long time)
* for references refer to GOOGLE SEARCH - > http://kit-e.ru/assets/files/pdf/2007_08_170.pdf 
*/
void ProcessKeyFSM(void){

	switch(key_state){
		case 0:
			if (key_code>NO_BTTN){
				key_code_prev = key_code;
				ButtonTimerReset();
				key_state=1;
			}
		break;
		
		case 1:
			if (GetButtonTimerCount()>DebounceTime){
				key_state=2;
			}
		break;
		
		case 2:
			if (key_code==key_code_prev){
				ButtonTimerReset();
				ButtonPressed = 1;
				key_state=3;
			}
			else
				key_state=0;
		break;
			
		case 3:
			if (key_code==key_code_prev){
				if (GetButtonTimerCount()>FirstDelay){
					ButtonTimerReset();
					ButtonPressed = 1;
					key_state=4;
				}
			}
			else
				key_state=0;
		break;
			
		case 4:
			if(key_code==key_code_prev){
				if (GetButtonTimerCount()>AutoRepeat){
					ButtonTimerReset();
					ButtonPressed = 1;
				}
			}
			else
				key_state = 0;
		break;
	}
}

void ExecuteButton(key_codes keycode){
	
	switch(keycode){
		case BUTTON_UP:
				
		break;
		
		case BUTTON_START:
			
						
				
		break;
		
		case BUTTON_DOWN:
				
		break;
		
		case BUTTON_MUTE:
				
		break;
		
		
		case BUTTON_MODE:
			
		break;
		
		default:
		break;
	}
	
}


// ticks every 1 msec
void HAL_SYSTICK_Callback(void){
	
	ButtonTimerCount++;
		
}

void ButtonTimerReset(void){
	ButtonTimerCount=0;
}

uint16_t GetButtonTimerCount(void){
	return ButtonTimerCount;
}
