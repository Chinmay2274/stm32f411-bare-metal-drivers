/*
 * run.c
 *
 *  Created on: Aug 7, 2026
 *      Author: chinmay
 */


#include "delay.h"
#include "u8g2.h"
#include "OLED_DRIVER_U2G8.h"
#include <string.h>
SPI_Handle_t SPI2Handle;

void mosi_spi2(){
	GPIO_Handle_t GPIOConfig;
		 memset(&GPIOConfig,0,sizeof(GPIOConfig));

	GPIOConfig.pGPIOx=GPIOB;
	GPIOConfig.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_15;
	GPIOConfig.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_ALTFN;
	GPIOConfig.GPIO_PinConfig.GPIO_PinALTFunMode=5;

	GPIO_Init(&GPIOConfig);
}

void sclck_spi2(){

	 GPIO_Handle_t GPIOConfig;

	 memset(&GPIOConfig,0,sizeof(GPIOConfig));
	//pb13 af5 sclck spi2
	GPIOConfig.pGPIOx=GPIOB;
	GPIOConfig.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_13;
	GPIOConfig.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_ALTFN;
	GPIOConfig.GPIO_PinConfig.GPIO_PinALTFunMode=5;

	GPIO_Init(&GPIOConfig);
}

void miso_spi2(){
	 GPIO_Handle_t GPIOConfig;

		 memset(&GPIOConfig,0,sizeof(GPIOConfig));
		//pb14 af5 miso spi2
		GPIOConfig.pGPIOx=GPIOB;
		GPIOConfig.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_14;
		GPIOConfig.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_ALTFN;
		GPIOConfig.GPIO_PinConfig.GPIO_PinALTFunMode=5;

		GPIO_Init(&GPIOConfig);
}

void spi2_config(){
    // 2. Configure the ACTUAL SPI2Handle, not FLASHMaster
    SPI2Handle.pSPIx = SPI2;
    SPI2Handle.SPI_Config.SPI_BusConfig = SPI_BUSCONFIG_FD;
    SPI2Handle.SPI_Config.SPI_DFF = SPI_DFF_8BIT;
    SPI2Handle.SPI_Config.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
    SPI2Handle.SPI_Config.SPI_Sclk_Speed = SPI_SCLK_SPEED_DIV2 ;
    SPI2Handle.SPI_Config.SPI_SSM = SPI_SSM_SW_EN;

    SPI_PCLK_CONTROL(SPI2, ENABLE);

    // Initialize the hardware using the actual handle
    SPI_Init(&SPI2Handle);
}
    void oled_cs_gpio(void)
    {
        GPIO_Handle_t GPIOConfig;
        memset(&GPIOConfig, 0, sizeof(GPIOConfig));

        GPIOConfig.pGPIOx = GPIOD;
        GPIOConfig.GPIO_PinConfig.GPIO_PinNumber   = GPIO_PIN_NO_1;
        GPIOConfig.GPIO_PinConfig.GPIO_PinMode     = GPIO_MODE_OUT;
        GPIOConfig.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NOPu_NOPd;
        GPIOConfig.GPIO_PinConfig.GPIO_PinSpeed    = GPIO_SPEED_FAST;
        GPIOConfig.GPIO_PinConfig.GPIO_PinOPType   = GPIO_OP_TYPE_PP;
        GPIO_Init(&GPIOConfig);

        GPIO_WriteToOutputPin(GPIOD, GPIO_PIN_NO_1, GPIO_PIN_SET);   /* CS idle high (deselected) */
    }

    void oled_dc_gpio(void)
    {
        GPIO_Handle_t GPIOConfig;
        memset(&GPIOConfig, 0, sizeof(GPIOConfig));

        GPIOConfig.pGPIOx = GPIOD;
        GPIOConfig.GPIO_PinConfig.GPIO_PinNumber   = GPIO_PIN_NO_2;
        GPIOConfig.GPIO_PinConfig.GPIO_PinMode     = GPIO_MODE_OUT;
        GPIOConfig.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NOPu_NOPd;
        GPIOConfig.GPIO_PinConfig.GPIO_PinSpeed    = GPIO_SPEED_FAST;
        GPIOConfig.GPIO_PinConfig.GPIO_PinOPType   = GPIO_OP_TYPE_PP;
        GPIO_Init(&GPIOConfig);

        GPIO_WriteToOutputPin(GPIOD, GPIO_PIN_NO_2, GPIO_PIN_RESET); /* DC low = command mode */
    }

    void oled_reset_gpio(void)
    {
        GPIO_Handle_t GPIOConfig;
        memset(&GPIOConfig, 0, sizeof(GPIOConfig));

        GPIOConfig.pGPIOx = GPIOD;
        GPIOConfig.GPIO_PinConfig.GPIO_PinNumber   = GPIO_PIN_NO_3;
        GPIOConfig.GPIO_PinConfig.GPIO_PinMode     = GPIO_MODE_OUT;
        GPIOConfig.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NOPu_NOPd;
        GPIOConfig.GPIO_PinConfig.GPIO_PinSpeed    = GPIO_SPEED_FAST;
        GPIOConfig.GPIO_PinConfig.GPIO_PinOPType   = GPIO_OP_TYPE_PP;
        GPIO_Init(&GPIOConfig);

        GPIO_WriteToOutputPin(GPIOD, GPIO_PIN_NO_3, GPIO_PIN_SET);   /* RST high = not in reset */
    }
    u8g2_t u8g2;
   game_state_t s;
int main(void){

	mosi_spi2();
	sclck_spi2();
    miso_spi2();
    spi2_config();
    oled_cs_gpio();
    oled_dc_gpio();
    oled_reset_gpio();
    dwt_init();

    ssi_control(SPI2,ENABLE);
    spe_control(SPI2, ENABLE); // must enable the spe bit after spi config must keep it diabled when CONFIG THE SPI


    u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, U8G2_R0,
                                        u8x8_spi, u8x8_gpio_and_delay);
     u8g2_InitDisplay(&u8g2);
     u8g2_SetPowerSave(&u8g2, 0);     /* wake the panel */


     s.x=123;
     s.y=59;
     oled_render_game(&s,u8g2);
     while (1) ;








}
