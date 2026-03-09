#ifndef __myadc_h__
#define __myadc_h__

#include "main.h"
#include "adc.h"

typedef struct {
	uint16_t vol1;
    uint16_t vol2;
} Vol;

extern Vol vol;

void ADC_Scan(void);

#endif
