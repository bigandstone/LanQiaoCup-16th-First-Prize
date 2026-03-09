#include "pwmout.h"

Pwm pwm = {0};

void PWM_Updata(void){
    htim3.Instance->CCR2 = pwm.pulse;
    htim3.Instance->PSC = 80000000 / 100 / pwm.freq ;
}