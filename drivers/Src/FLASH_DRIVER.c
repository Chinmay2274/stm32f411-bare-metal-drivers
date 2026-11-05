/*
 * FLASH.c
 *
 *  Created on: Jun 12, 2026
 *      Author: chinmay
 */

#include "FLASH_DRIVER.h"
#include <stdint.h>
#include <string.h>

static Flash_Device_t *p;
// global pointers for calling the read_from_flash inside app_callback function
	// required due to limited scope of that call

	static uint8_t *pending_rx_buf;
	static uint8_t *pending_tx_buf;
	static uint8_t  pending_len;

typedef enum{
	APP_STATE_IDLE,
	APP_STATE_SENDING_CMD,
	APP_STATE_READING_DATA,
	APP_STATE_TRANSMITTING_DATA,
	APP_STATE_IGNORE,
}FlashState_t;

volatile FlashState_t flash_current_state = APP_STATE_IDLE;

uint32_t volatile clck_cycles=0;

uint8_t read_status_reg_oflash(Flash_Device_t *pHandle) {
    uint8_t cmd = 0x05;
    uint8_t status;
    uint8_t dummy;

    GPIO_WriteToOutputPin(pHandle->pCSPort, pHandle->CSPin, GPIO_PIN_RESET);

    SPI_SendData(pHandle->SPI_Main->pSPIx, &cmd, 1);



    // now clock out the actual status byte
    SPI_ReceiveData_Dummy(pHandle->SPI_Main->pSPIx, &status, 1);

    while (SpiGetFlagStatus(pHandle->SPI_Main->pSPIx, SPI_SR_BSY) == FLAG_SET);
   // while (pHandle->SPI_Main->pSPIx->SR & (1 << 7));
    GPIO_WriteToOutputPin(pHandle->pCSPort, pHandle->CSPin, GPIO_PIN_SET);

    return status;
}



void clear_write_protection(Flash_Device_t *pHandle){
	GPIO_WriteToOutputPin(pHandle->pCSPort,pHandle->CSPin,GPIO_PIN_RESET);
			static uint8_t txb;
			txb=0x06;
			SPI_SendData(pHandle->SPI_Main->pSPIx,&txb, 1);
			while (pHandle->SPI_Main->pSPIx->SR & (1 << 7));
			GPIO_WriteToOutputPin(pHandle->pCSPort,pHandle->CSPin,GPIO_PIN_SET);

			GPIO_WriteToOutputPin(pHandle->pCSPort,pHandle->CSPin,GPIO_PIN_RESET);
			uint8_t ar[2]={0x01,0x00};
			 SPI_SendData(pHandle->SPI_Main->pSPIx, ar, 2);
			 while (SpiGetFlagStatus(pHandle->SPI_Main->pSPIx, SPI_SR_BSY) == FLAG_SET);

			 GPIO_WriteToOutputPin(pHandle->pCSPort,pHandle->CSPin,GPIO_PIN_SET);

}





void read_flash_it(Flash_Device_t *pHandle,uint32_t target_addr,uint8_t *rx,uint8_t *tx,uint8_t Len){
	pHandle->SPI_Main->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
	pHandle->SPI_Main->Txstate = SPI_READY;
	pHandle->SPI_Main->Rxstate = SPI_READY;
	GPIO_WriteToOutputPin(pHandle->pCSPort,pHandle->CSPin,GPIO_PIN_RESET);

	static uint8_t txBuffer[4];

		uint8_t high = (target_addr >> 16)&0xFF;// extract the leftmost middle and rightmost byte
		uint8_t mid = (target_addr >> 8)&0xFF;
	    uint8_t low = (target_addr)&0xFF;

		txBuffer[0]=0x03;
		txBuffer[1]=high;
		txBuffer[2]=mid;
		txBuffer[3]=low;

		SPI_SendData_IT(pHandle->SPI_Main,txBuffer, 4);
		while(pHandle->SPI_Main->Txstate==SPI_BUSY_IN_TX);
		pHandle->SPI_Main->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);

		while (!(pHandle->SPI_Main->pSPIx->SR & (1 << 1)));// wait TXE set before proceeding
		while (pHandle->SPI_Main->pSPIx->SR & (1 << 7));

		uint8_t dummy_byte;
			dummy_byte=pHandle->SPI_Main->pSPIx->DR;
			dummy_byte=pHandle->SPI_Main->pSPIx->SR;
		// do NOT toggle cs here , cmd+addr and data read must stay in one cs low envelope

		flash_current_state=APP_STATE_READING_DATA;

		SPI_RecieveData_IT(pHandle->SPI_Main, rx, Len);// set the rxneie buffer
		pHandle->pSPI->DR=0XFF; //after setting up rx int send first dummy byte to enable rxne flag
		// send dummy bytes to generate clock signal so slave can send data back
		//SPI_SendData_IT(pHandle->SPI_Main, tx, Len);

		while(pHandle->SPI_Main->Rxstate==SPI_BUSY_IN_RX);// wait till all bytes received (like txstate wait)
		while (pHandle->SPI_Main->pSPIx->SR & (1 << 7));// wait till spi block not busy

	GPIO_WriteToOutputPin(pHandle->pCSPort,pHandle->CSPin,GPIO_PIN_SET);


}





//the hardware isr will be called in main.c  for flash

void SPI_Application_Event_CallBack(SPI_Handle_t *pHandle,uint8_t AppEv){



}






// implement cs logic low in asych function and high at end of reception in event call back



/*
 * **********************write portion************************
 */

void write_configure( Flash_Device_t *pHandle,uint32_t target_addr){
	GPIO_WriteToOutputPin(pHandle->pCSPort,pHandle->CSPin,GPIO_PIN_RESET);

	static uint8_t txb;
	txb=0x06;
	SPI_SendData_IT(pHandle->SPI_Main,&txb, 1);
	while(pHandle->SPI_Main->Txstate==SPI_BUSY_IN_TX);
	while (!(pHandle->SPI_Main->pSPIx->SR & (1 << 1)));  // wait TXE set (buffer empty again)(note always do this for int based send before toggligng cs )(imp debugging realisation)

	while (pHandle->SPI_Main->pSPIx->SR & (1 << 7));


	GPIO_WriteToOutputPin(pHandle->pCSPort,pHandle->CSPin,GPIO_PIN_SET);
	//wait while spi block is bsy (stuck in transmission of 0x02)
	// while(read_status_reg_oflash(pHandle)&(1<<0));

	GPIO_WriteToOutputPin(pHandle->pCSPort,pHandle->CSPin,GPIO_PIN_RESET);
	static uint8_t txBuffer[4];

		uint8_t high = (target_addr >> 16)&0xFF;// extract the leftmost middle and rightmost byte
		uint8_t mid = (target_addr >> 8)&0xFF;
	    uint8_t low = (target_addr)&0xFF;

		txBuffer[0]=0x20;
		txBuffer[1]=high;
		txBuffer[2]=mid;
		txBuffer[3]=low;

		SPI_SendData_IT(pHandle->SPI_Main,txBuffer, 4);
		//while (pHandle->SPI_Main->pSPIx->SR & (1 << 7));
		while(pHandle->SPI_Main->Txstate==SPI_BUSY_IN_TX);
		while (pHandle->SPI_Main->pSPIx->SR & (1 << 7));
		while (!(pHandle->SPI_Main->pSPIx->SR & (1 << 1)));
uint8_t dummy_byte;
			dummy_byte=pHandle->SPI_Main->pSPIx->DR;
			dummy_byte=pHandle->SPI_Main->pSPIx->SR;
		GPIO_WriteToOutputPin(pHandle->pCSPort,pHandle->CSPin,GPIO_PIN_SET);
		 while(read_status_reg_oflash(pHandle)&(1<<0));

		GPIO_WriteToOutputPin(pHandle->pCSPort,pHandle->CSPin,GPIO_PIN_RESET);

			static uint8_t txb1;
			txb1=0x06;
			SPI_SendData_IT(pHandle->SPI_Main,&txb1, 1);
			while (!(pHandle->SPI_Main->pSPIx->SR & (1 << 1)));
			while(pHandle->SPI_Main->Txstate==SPI_BUSY_IN_TX);
			while (pHandle->SPI_Main->pSPIx->SR & (1 << 7));
			dummy_byte=pHandle->SPI_Main->pSPIx->DR;
						dummy_byte=pHandle->SPI_Main->pSPIx->SR;
			GPIO_WriteToOutputPin(pHandle->pCSPort,pHandle->CSPin,GPIO_PIN_SET);

			 while(read_status_reg_oflash(pHandle)&(1<<0));




}

void write_to_segment(Flash_Device_t *pHandle,uint32_t target_addr,uint8_t *tx,uint8_t Len){

	flash_current_state=APP_STATE_IGNORE;
	write_configure( pHandle, target_addr);


	//while (pHandle->SPI_Main->pSPIx->SR & (1 << 7));
	GPIO_WriteToOutputPin(pHandle->pCSPort,pHandle->CSPin,GPIO_PIN_RESET);


		static uint8_t txBuffer1[4];

					uint8_t high1 = (target_addr >> 16)&0xFF;// extract the leftmost middle and rightmost byte
					uint8_t mid1 = (target_addr >> 8)&0xFF;
				    uint8_t low1 = (target_addr)&0xFF;

					txBuffer1[0]=0x02;
					txBuffer1[1]=high1;
					txBuffer1[2]=mid1;
					txBuffer1[3]=low1;
					SPI_SendData_IT(pHandle->SPI_Main,txBuffer1, 4);

					while (pHandle->SPI_Main->Txstate == SPI_BUSY_IN_TX);   // ← ADD: all 4 bytes handed off
					while (!(pHandle->SPI_Main->pSPIx->SR & (1 << 1)));     // TXE
					while (pHandle->SPI_Main->pSPIx->SR & (1 << 7));        // ← ADD: BSY clear, header fully on wire
					(void)pHandle->SPI_Main->pSPIx->DR;                     // ← ADD: drain RX
					(void)pHandle->SPI_Main->pSPIx->SR;
					flash_current_state=APP_STATE_TRANSMITTING_DATA;
//while(pHandle->SPI_Main->Txstate==SPI_BUSY_IN_TX);

	SPI_SendData_IT(pHandle->SPI_Main,tx, Len);


	GPIO_WriteToOutputPin(pHandle->pCSPort,pHandle->CSPin,GPIO_PIN_SET);

	//while (!(pHandle->pSPIx->SR & (1 << 1)));


}
void read_jedec_id(Flash_Device_t *pHandle, uint8_t *id_out /* 3 bytes */) {


	// chip_wake_up(pHandle);
	    uint8_t cmd = 0x9F;
	    GPIO_WriteToOutputPin(pHandle->pCSPort, pHandle->CSPin, GPIO_PIN_RESET);
	    SPI_SendData(pHandle->SPI_Main->pSPIx, &cmd, 1);

	    SPI_ReceiveData_Dummy(pHandle->SPI_Main->pSPIx, id_out, 3);   // blocking receive
	    //while (SpiGetFlagStatus(pHandle->SPI_Main->pSPIx, SPI_SR_BSY) == FLAG_SET);
	    while (pHandle->SPI_Main->pSPIx->SR & (1 << 7));
	    GPIO_WriteToOutputPin(pHandle->pCSPort, pHandle->CSPin, GPIO_PIN_SET);
	}



void write_data(Flash_Device_t *pHandle,uint32_t target_addr,uint8_t *tx,uint8_t Len){
	GPIO_WriteToOutputPin(pHandle->pCSPort,pHandle->CSPin,GPIO_PIN_RESET);
		static uint8_t txb;
		txb=0x06;
		SPI_SendData(pHandle->SPI_Main->pSPIx,&txb, 1);
		while (pHandle->SPI_Main->pSPIx->SR & (1 << 7));
		GPIO_WriteToOutputPin(pHandle->pCSPort,pHandle->CSPin,GPIO_PIN_SET);


		// while(read_status_reg_oflash(pHandle)&(1<<0));
		GPIO_WriteToOutputPin(pHandle->pCSPort,pHandle->CSPin,GPIO_PIN_RESET);
		static uint8_t txBuffer[4];

			uint8_t high = (target_addr >> 16)&0xFF;// extract the leftmost middle and rightmost byte
			uint8_t mid = (target_addr >> 8)&0xFF;
		    uint8_t low = (target_addr)&0xFF;

			txBuffer[0]=0x20;
			txBuffer[1]=high;
			txBuffer[2]=mid;
			txBuffer[3]=low;

			SPI_SendData(pHandle->SPI_Main->pSPIx,txBuffer, 4);

			while (pHandle->SPI_Main->pSPIx->SR & (1 << 7));

			GPIO_WriteToOutputPin(pHandle->pCSPort,pHandle->CSPin,GPIO_PIN_SET);
			while(read_status_reg_oflash(pHandle)&(1<<0));

			GPIO_WriteToOutputPin(pHandle->pCSPort,pHandle->CSPin,GPIO_PIN_RESET);

				static uint8_t txb1;
				txb1=0x06;
				SPI_SendData(pHandle->SPI_Main->pSPIx,&txb1, 1);
				while (pHandle->SPI_Main->pSPIx->SR & (1 << 7));

				GPIO_WriteToOutputPin(pHandle->pCSPort,pHandle->CSPin,GPIO_PIN_SET);
				// while(read_status_reg_oflash(pHandle)&(1<<0));

				GPIO_WriteToOutputPin(pHandle->pCSPort,pHandle->CSPin,GPIO_PIN_RESET);


						static uint8_t txBuffer1[4];

									uint8_t high1 = (target_addr >> 16)&0xFF;// extract the leftmost middle and rightmost byte
									uint8_t mid1 = (target_addr >> 8)&0xFF;
								    uint8_t low1 = (target_addr)&0xFF;

									txBuffer1[0]=0x02;
									txBuffer1[1]=high1;
									txBuffer1[2]=mid1;
									txBuffer1[3]=low1;
SPI_SendData(pHandle->SPI_Main->pSPIx,txBuffer1, 4);



SPI_SendData(pHandle->SPI_Main->pSPIx,tx, Len);
while (pHandle->SPI_Main->pSPIx->SR & (1 << 7));

GPIO_WriteToOutputPin(pHandle->pCSPort,pHandle->CSPin,GPIO_PIN_SET);
uint8_t flash_sr=0;
flash_sr=read_status_reg_oflash(pHandle);
while(read_status_reg_oflash(pHandle)&(1<<0));

}

void read_data(Flash_Device_t *pHandle, uint32_t target_addr, uint8_t *rx, uint8_t *tx, uint8_t len,uint8_t cmd){

	GPIO_WriteToOutputPin(pHandle->pCSPort,pHandle->CSPin,GPIO_PIN_RESET);

	static uint8_t txBuffer[4];

		uint8_t high = (target_addr >> 16)&0xFF;// extract the leftmost middle and rightmost byte
		uint8_t mid = (target_addr >> 8)&0xFF;
	    uint8_t low = (target_addr)&0xFF;

		txBuffer[0]=cmd;
		txBuffer[1]=high;
		txBuffer[2]=mid;
		txBuffer[3]=low;

		SPI_SendData(pHandle->SPI_Main->pSPIx,txBuffer, 4);



		 SPI_ReceiveData_Dummy(pHandle->SPI_Main->pSPIx, rx, len);
		 while (pHandle->SPI_Main->pSPIx->SR & (1 << 7));
		//while(read_status_reg_oflash(pHandle)&(1<<0));
		 GPIO_WriteToOutputPin(pHandle->pCSPort,pHandle->CSPin,GPIO_PIN_SET);

		 //clck_cycles = *(DWT_CYCCNT_BASEADDR);


}


void chip_wake_up(Flash_Device_t *pHandle){
	uint8_t wake = 0xAB;
		GPIO_WriteToOutputPin(pHandle->pCSPort, pHandle->CSPin, GPIO_PIN_RESET);
		SPI_SendData(pHandle->SPI_Main->pSPIx, &wake, 1);
		while (pHandle->SPI_Main->pSPIx->SR & (1 << 7));
		GPIO_WriteToOutputPin(pHandle->pCSPort, pHandle->CSPin, GPIO_PIN_SET);
		delay_1_second_approx();
}





