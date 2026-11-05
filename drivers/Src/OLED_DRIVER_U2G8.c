/*
 * OLED_DRIVER_U2G8.c
 *
 *  Created on: Aug 7, 2026
 *      Author: chinmay
 */


#include <stdint.h>
#include <string.h>

//#include "OLED_DRIVER.h"
#include "STM32F411E_spi_drivers.h"
#include "u8g2.h"
#include "delay.h"
#include "OLED_DRIVER_U2G8.h"

void oled_render_game( game_state_t *s,u8g2_t u8g2) {
    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawFrame(&u8g2, 0, 0, 128, 64);      /* walls */
    u8g2_DrawDisc(&u8g2, s->x, s->y, 3, U8G2_DRAW_ALL);
    u8g2_SendBuffer(&u8g2);
}

uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg,
                             uint8_t arg_int, void *arg_ptr)
{
    switch(msg)
    {
    case U8X8_MSG_DELAY_MILLI:
        dwt_delay(arg_int*1000);
        break;
    case U8X8_MSG_GPIO_CS:
       GPIO_WriteToOutputPin(GPIOD , 1, arg_int); //cs for oled portd pin 1
        break;
    case U8X8_MSG_GPIO_DC:
    	GPIO_WriteToOutputPin(GPIOD,2, arg_int);// DC FOR OLED PORTD PIN2
        break;
    case U8X8_MSG_GPIO_RESET:
    	GPIO_WriteToOutputPin(GPIOD,3, arg_int);// RESET PIN PORTD PIN3
        break;
    }
    return 1;
}


uint8_t u8x8_spi(u8x8_t *u8x8, uint8_t msg,
                 uint8_t arg_int, void *arg_ptr)
{
    switch(msg)
    {
    case U8X8_MSG_BYTE_SET_DC:
    	GPIO_WriteToOutputPin(GPIOD,2, arg_int);
        break;
    case U8X8_MSG_BYTE_SEND:
        SPI_SendData(SPI2, (uint8_t*)arg_ptr, arg_int);
        break;
    case U8X8_MSG_BYTE_START_TRANSFER:
    	 GPIO_WriteToOutputPin(GPIOD , 1, GPIO_PIN_RESET);
        break;
    case U8X8_MSG_BYTE_END_TRANSFER:
    	 GPIO_WriteToOutputPin(GPIOD , 1, GPIO_PIN_SET);
        break;
    }
    return 1;
}
