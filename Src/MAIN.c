

/*
 * FLASH.c
 *
 *  Created on: Jun 14, 2026
 *      Author: chinmay
 */


#include <stdint.h>
#include <string.h>
#include "FLASH_DRIVER.h"
#include "STM32F411E_uart_drivers.h"

	Flash_Device_t FLASHMaster;
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

void cs_gpio(){
	 GPIO_Handle_t GPIOConfig;

		 memset(&GPIOConfig,0,sizeof(GPIOConfig));

			FLASHMaster.CSPin=GPIO_PIN_NO_10;
			FLASHMaster.pCSPort=GPIOB;

		GPIOConfig.pGPIOx=FLASHMaster.pCSPort;
		GPIOConfig.GPIO_PinConfig.GPIO_PinNumber=FLASHMaster.CSPin;
		GPIOConfig.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_OUT;
		GPIOConfig.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NOPu_NOPd ;
		GPIOConfig.GPIO_PinConfig.GPIO_PinSpeed= GPIO_SPEED_FAST;
		GPIOConfig.GPIO_PinConfig.GPIO_PinOPType=GPIO_OP_TYPE_PP;
		GPIO_Init(&GPIOConfig);

		GPIO_WriteToOutputPin(FLASHMaster.pCSPort, FLASHMaster.CSPin, GPIO_PIN_SET);


}




/*
 *
 * SPI CONFIG END
 */


/*
 *
 *                   UART CONFIG
 *
 *
 */

USART_Handle_t USART2Handle;

void tx_usart2(){
	GPIO_Handle_t GPIOConfig;
	memset(&GPIOConfig,0,sizeof(GPIOConfig));

	//pa2 af7 tx usart2
	GPIOConfig.pGPIOx=GPIOA;
	GPIOConfig.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_2;
	GPIOConfig.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_ALTFN;
	GPIOConfig.GPIO_PinConfig.GPIO_PinALTFunMode=7;
	GPIOConfig.GPIO_PinConfig.GPIO_PinOPType=GPIO_OP_TYPE_PP;
	//GPIOConfig.GPIO_PinConfig.GPIO_PinPuPdControl=GPIO_PIN_PU;
	GPIOConfig.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_FAST;

	GPIO_Init(&GPIOConfig);
}

void rx_usart2(){
	GPIO_Handle_t GPIOConfig;
	memset(&GPIOConfig,0,sizeof(GPIOConfig));

	//pa3 af7 rx usart2
	GPIOConfig.pGPIOx=GPIOA;
	GPIOConfig.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_3;
	GPIOConfig.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_ALTFN;
	GPIOConfig.GPIO_PinConfig.GPIO_PinALTFunMode=7;
	GPIOConfig.GPIO_PinConfig.GPIO_PinOPType=GPIO_OP_TYPE_PP;
	//GPIOConfig.GPIO_PinConfig.GPIO_PinPuPdControl=GPIO_PIN_PU;
	GPIOConfig.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_FAST;

	GPIO_Init(&GPIOConfig);
}

void usart2_config(){
	USART2Handle.pUSARTx = USART2;
	USART2Handle.USART_Config.USART_Mode = USART_MODE_TXRX;
	USART2Handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
	USART2Handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
	USART2Handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
	USART2Handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
	// baud fixed at 9600 inside USART_Init (BRR hardcoded 1667, 16 MHz HSI assumed)
	// OVER8 left at reset value 0 -> oversampling by 16
	// UE set inside USART_Init (config first, enable last)

	USART_Init(&USART2Handle);
}





/*
 * UART CONFIG END
 */







uint8_t cmd=0x03;
uint8_t len =10;
uint8_t     rx [10];
uint8_t tx[10];


uint32_t target_addr = 0x000010U;



uint8_t id[3]={0,0,0};

volatile uint8_t tx_done = 0;
volatile uint8_t rx_done = 0;
volatile int status;

char msg[] = "DATA NOT CORRUPTED\r\n";
char msg2[]= "DATA CORRUPTED\r\n";


void USART2_IRQHandler(void){
	USART_TXE_INTHandler(&USART2Handle);
	USART_RXNE_INTHandler(&USART2Handle);
}

// overrides the weak callback in the driver
void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle, uint8_t AppEv){
	if(AppEv == USART_EVENT_TX_CMPLT){
		tx_done = 1;
	}
	if(AppEv==USART_EVENT_RX_CMPLT){
        rx_done=1;
	}
}
uint32_t write_cycles=0;
uint32_t read_cycles=0;

int main(void){





	mosi_spi2();
	sclck_spi2();
    miso_spi2();
    cs_gpio();
	spi2_config();

    FLASHMaster.SPI_Main=&SPI2Handle;


	tx_usart2();
	rx_usart2();
	usart2_config();

	USART_IRQPriorityConfig(IRQ_NO_USART2, 0);
	USART_IRQInterruptConfig(IRQ_NO_USART2, ENABLE);
	// MUST ENABLE SSI WHEN SSM=1 BEFORE ENABLING SPI(spe bit)

	ssi_control(SPI2,ENABLE);
	spe_control(SPI2, ENABLE); // must enable the spe bit after spi config must keep it diabled when CONFIG THE SPI
    read_jedec_id(&FLASHMaster,id);

    // Step 1: Enable the trace block (Set bit 24 in DEMCR)
    		    (*(DEMCR_BASEADDR)) |= (1 << 24);
    		    // SET CLCK COUNTER TO 0
    		(*(DWT_CYCCNT_BASEADDR)) = 0x00000000U ;
    		// Step 3: Start the cycle counter ticking (Set bit 0 in DWT_CTRL)
    				    (*(DWT_CTRL_BASEADDR)) |= 1;

//all spi config done^
	USART_ReceiveDataIT(&USART2Handle, tx, len);
		while(!rx_done);
//clear_write_protection(&FLASHMaster);

	 write_data(&FLASHMaster, target_addr,tx ,len);
	 write_cycles = *(DWT_CYCCNT_BASEADDR);

	 (*(DWT_CYCCNT_BASEADDR)) = 0x00000000U ;



	 read_data(&FLASHMaster, target_addr, rx, tx, len, cmd);
	 read_cycles=*(DWT_CYCCNT_BASEADDR);
	// memset(rx, 1, sizeof(rx));

	 status = memcmp(rx,tx,len);
	if(status==0){
	 USART_SendDataIT(&USART2Handle, (uint8_t*)msg,strlen(msg));
	 	while(!tx_done);
	}
	else{
		USART_SendDataIT(&USART2Handle, (uint8_t*)msg2,strlen(msg2));
		 	while(!tx_done);
	}



	 while(1);
}


