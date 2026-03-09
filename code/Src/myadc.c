#include "myadc.h"

Vol vol = { 0 };

void ADC_Scan(void){
    HAL_ADC_Start(&hadc1);
    HAL_ADC_Start(&hadc2);
    if(HAL_OK == HAL_ADC_PollForConversion(&hadc1,100)){
        vol.vol1 = HAL_ADC_GetValue(&hadc1);
    }
    if(HAL_OK == HAL_ADC_PollForConversion(&hadc2,100)){
        vol.vol2 = HAL_ADC_GetValue(&hadc2);
    }
}
