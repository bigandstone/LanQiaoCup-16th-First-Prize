#ifndef __led_h__
#define __led_h__

#include "main.h"

typedef struct {
	uint8_t state;
} Led;

extern Led led;

void LED_Updata(void);
void LED_Reset(uint8_t n );
void LED_Set(uint8_t n );

#endif
