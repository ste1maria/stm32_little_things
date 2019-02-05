#include "PID_control.h"

unsigned int Heater_Power;
PID_s _pid; 		

#define HeaterEnable(en)	 			(HAL_GPIO_WritePin(HEATER_EN_GPIO_Port, HEATER_EN_Pin, (en)))
#define LOCK_HEATER			 			GPIO_PIN_RESET													
#define UNLOCK_HEATER						GPIO_PIN_SET		

/* PID gains */
#define _dGain		0	          	// derivative gain
#define _pGain	 	1.3						// chosen because when only proportional control, this coefficient is the best
#define _iGain		0.0006			  // almost perfect integral gain, which provides very small overshoot
#define _iMax			30000.0     	// maximum integral STATE  (sum of errors)
#define _iMin			-30000.0			// minimum integral STATE		(sum of errors)

#define Minimum_Power 	0
#define Maximum_Power 	100

void UpdateHeater(void){
		uint16_t ControlTemperature = GetMeasuredTemperature();			
		uint16_t TemperatureDifference = GetDesiredTemperature()-ControlTemperature;
		
		if (HardwareHeaterControl()==HEATER_ON){	
			Heater_Power = UpdatePID(&_pid, TemperatureDifference, (double)ControlTemperature);	// calculate heater power
				
		}
		else {
			Heater_Power = 0;
		}
	
	TIM1->CCR1 = Heater_Power; 								// apply heater pwoer as timN chN PWM duty cycle
}


unsigned int UpdatePID(PID_s * pid, double error, double MeasuredTemperature){	
	double POWER_HH;
	
  double pTerm, dTerm, iTerm;

	pTerm = pid->pGain * error;    // calculate the proportional term
	pid->iState += error;          // calculate the integral state with appropriate limiting
 
	if (pid->iState > pid->iMax) 
			pid->iState = pid->iMax;     
	else if (pid->iState < pid->iMin) 
			pid->iState = pid->iMin;
	
	iTerm = pid->iGain * pid->iState;    			// calculate the integral term
	
	dTerm = pid->dGain * (MeasuredTemperature - pid->dState);		//calculate derivative term
	
	pid->dState = MeasuredTemperature;					// dState is actually the last "position" (or temperature), so we write it now to use in the next cycle.
	
	POWER_HH = (pTerm + iTerm - dTerm)*10;			//multipying by 10 because timer reload value is 1000 ms, but we need a scale of 1-100% (BECAUSE 1% is actually 10 ms)
	
	if (POWER_HH >= Maximum_Power){							//if calculated power is greater than maximum then set maximum power
		POWER_HH = Maximum_Power;
	}
	if (POWER_HH < Minimum_Power){							//if calculated power is negative then set zero power
		POWER_HH = Minimum_Power;
	}
	
  return ((unsigned int)(POWER_HH));					// return the needed power in terms of TIM1 channel 3 duty cycle 
}

heater_protection_status HardwareHeaterControl(void){
	if (AnyAlarm ||			// if any failure occured
			|| !HeatingStarted){														// OR we just started the device and haven't already set the temperature
			HeaterEnable(LOCK_HEATER);							 				// Heater is Locked so even if TIM1 CH1 line somehow triggers, heater won't be affected
			StopHeating();
			return HEATER_OFF;
	}
	else {
			HeaterEnable(UNLOCK_HEATER);	
			return HEATER_ON;
	}
}

void InitPIDController(void){
	_pid.iGain = _iGain;
	_pid.iMax = _iMax;
	_pid.iMin = _iMin;
	
	_pid.pGain = _pGain;
	_pid.dGain = _dGain;
}
