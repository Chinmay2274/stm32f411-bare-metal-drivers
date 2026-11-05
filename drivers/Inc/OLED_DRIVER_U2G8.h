/*
 * OLED_DRIVER_U2G8.h
 *
 *  Created on: Aug 7, 2026
 *      Author: chinmay
 */

#ifndef INC_OLED_DRIVER_U2G8_H_
#define INC_OLED_DRIVER_U2G8_H_

#include "STM32F411E.h"

typedef struct {
	u8g2_uint_t x;
	u8g2_uint_t y;
} game_state_t;





uint8_t u8x8_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
void oled_render_game( game_state_t *s,u8g2_t u8g2);


#endif /* INC_OLED_DRIVER_U2G8_H_ */
