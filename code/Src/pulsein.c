#include "pulsein.h"

Pul pul = {0};

void PUL_Scan(void){
    
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
    if(htim == &htim2) {
        pul.freq = 1000000 / htim->Instance->CCR1;
        htim2.Instance->CNT = 0;
    }
}