#ifndef __pulsein_h__
#define __pulsein_h__

#include "main.h"
#include "tim.h"

typedef struct {
	uint8_t pulse;
    uint16_t freq;
} Pul;

extern Pul pul;

void PUL_Scan(void);

#endif
