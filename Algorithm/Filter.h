#ifndef __FILTER_H
#define __FILTER_H


uint16_t Filter_Average(uint16_t *buf, uint8_t len);
uint16_t Filter_Median(uint16_t *buf, uint8_t len);
uint16_t Filter_MovingAverage(uint16_t new_value);

#endif
