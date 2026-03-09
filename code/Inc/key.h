#ifndef __key_h__
#define __key_h__

#include "main.h"

typedef struct {
	uint8_t state;
    uint8_t last_state;
    
    uint32_t timer;
    uint8_t flag;
} Key;

extern Key key;

void KEY_Scan(void);
uint8_t KEY_GetFlag(void);

#endif
