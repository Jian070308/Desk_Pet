#ifndef INC_KEY_H_
#define INC_KEY_H_

#include "main.h"
#include "stm32f1xx.h"
#include "font.h"
#include "oled.h"

int KEY_Scan();
void Play();
void Feed();
void Wash();
void Limit(uint8_t *value);


#endif /* INC_KEY_H_ */
