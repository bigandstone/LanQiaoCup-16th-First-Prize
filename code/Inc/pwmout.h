#ifndef __pwmout_h__
#define __pwmout_h__

#include "main.h"
#include "tim.h"

typedef struct {
	uint16_t freq;
	uint8_t pulse;
}Pwm;

extern Pwm pwm;

void PWM_Updata(void);

#endif
