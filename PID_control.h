#ifndef _PID_H_
#define _PID_H_

#include "stm32f1xx.h"

typedef struct {
	double dState;				// differential part (previous temperature value)
	double iState;				// integral part
	double iMax, iMin;		// maximum and minimum allowed values of iState
	
	double pGain;					// proportional coefficient
	double iGain;					// integral coefficient
	double dGain;					// differential coefficient
} PID_s;

typedef enum {
	HEATER_ON,
	HEATER_OFF
} heater_protection_status;

void UpdateHeater(void);
unsigned int UpdatePID(PID_s * pid, double error, double MeasuredTemperature);
heater_protection_status HardwareHeaterControl(void);
void InitPIDController(void);
#endif
