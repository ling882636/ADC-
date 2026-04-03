#ifndef __FILTER_H
#define __FILTER_H

#include <stdint.h>

uint16_t Filter_Average(uint16_t *buf, uint8_t len);
uint16_t Filter_Median(uint16_t *buf, uint8_t len);
uint16_t Filter_MovingAverage(uint16_t new_value);
uint16_t low_pass_filter(uint16_t input);

void Filter_SetAlpha(float alpha);
float Filter_GetAlpha(void);

#endif
