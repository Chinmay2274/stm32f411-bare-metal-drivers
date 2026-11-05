/*
 * main.c
 *
 *  Created on: May 31, 2026
 *      Author: chinmay
 */
#include <stdint.h>
#include <string.h>
#include "STM32F411E_spi_drivers.h"

void mosi_spi2(){
	GPIO_Handle_t GPIOConfig;
		 memset(&GPIOConfig,0,sizeof(GPIOConfig));

	GPIOConfig.pGPIOx=GPIOB;
	GPIOConfig.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_15;
	GPIOConfig.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_ALTFN;
	GPIOConfig.GPIO_PinConfig.GPIO_PinALTFunMode=5;
	//GPIO_PCLCK_CONTROL(GPIOB, ENABLE);
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

void spi2_config(){
	SPI_Handle_t SPIMaster;

	memset(&SPIMaster,0,sizeof(SPIMaster));

	SPIMaster.pSPIx=SPI2;
	SPIMaster.SPI_Config.SPI_BusConfig=SPI_BUSCONFIG_FD;
	SPIMaster.SPI_Config.SPI_DFF=SPI_DFF_8BIT;
	SPIMaster.SPI_Config.SPI_DeviceMode=SPI_DEVICE_MODE_MASTER;
	SPIMaster.SPI_Config.SPI_Sclk_Speed=SPI_SCLK_SPEED_DIV2 ;
	SPIMaster.SPI_Config.SPI_SSM=SPI_SSM_SW_EN;
	SPI_PCLK_CONTROL(SPI2, ENABLE);

	SPI_Init(&SPIMaster);



}
int main (void){






mosi_spi2();
sclck_spi2();

spi2_config();

// MUST ENABLE SSI WHEN SSM=1 BEFORE ENABLING SPI(spe bit)
ssi_control(SPI2,ENABLE);
spe_control(SPI2, ENABLE); // must enable the spe bit after spi config must keep it diabled when CONFIG THE SPI


char str[]="Hello World";// 11 bytes  excluding null terminator

	SPI_SendData(SPI2, (uint8_t*)str, 11);

// calling the spi send data

while(SpiGetFlagStatus(SPI2,SPI_SR_BSY)==FLAG_SET);	// WAIT while SPI IS BUSY


spe_control(SPI2, DISABLE);//DISABLE SPI WHEN SPI IS NO LONGER BUSY
ssi_control(SPI2,DISABLE);





}











