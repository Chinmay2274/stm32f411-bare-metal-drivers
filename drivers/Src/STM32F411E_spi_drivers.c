/*
 * STM32F411E_spi_drivers.c
 *
 *  Created on: May 30, 2026
 *      Author: chinmay
 */
#include "STM32F411E_spi_drivers.h"
#include <stdio.h>
volatile uint32_t rxne_count = 0;

void spe_control(SPI_RegDef_t *pSPIx, uint8_t ENorDI) {
	if (ENorDI == ENABLE) {
		pSPIx->CR1 |= (1 << SPI_CRI_SPE);
	} else {
		pSPIx->CR1 &= ~(1 << SPI_CRI_SPE);
	}
}

void ssi_control(SPI_RegDef_t *pSPIx, uint8_t ENorDI) {
	if (ENorDI == ENABLE) {
		pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	} else {
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
	}
}

uint8_t SpiGetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName) {
	if (pSPIx->SR & (1 << FlagName)) { // A MACRO FlagName is sent as an argument containsthe psotion of it in sr register
		return FLAG_SET;
	}
	return FLAG_RESET;
}

void SPI_PCLK_CONTROL(SPI_RegDef_t *pSPIx, uint8_t ENorDI) {
	if (ENorDI == ENABLE) {
		if (pSPIx == SPI1) {
			SPI1_PCLK_EN();
		} else if (pSPIx == SPI2) {
			SPI2_PCLK_EN();
		} else if (pSPIx == SPI3) {
			SPI3_PCLK_EN();
		} else if (pSPIx == SPI4) {
			SPI4_PCLK_EN();
		} else if (pSPIx == SPI5) {
			SPI5_PCLK_EN();
		}

	}

	else if (ENorDI == DISABLE) {
		if (pSPIx == SPI1) {
			SPI1_PCLK_DI();
		} else if (pSPIx == SPI2) {
			SPI2_PCLK_DI();
		} else if (pSPIx == SPI3) {
			SPI3_PCLK_DI();
		} else if (pSPIx == SPI4) {
			SPI4_PCLK_DI();
		} else if (pSPIx == SPI5) {
			SPI5_PCLK_DI();
		}
	}

}

void SPI_Init(SPI_Handle_t *pSPIHandle) {
	SPI_PCLK_CONTROL(pSPIHandle->pSPIx, ENABLE);
	uint32_t temp = 0;

	//master or slave
	temp |= (pSPIHandle->SPI_Config.SPI_DeviceMode << SPI_CR1_MSTR);

	//Bus confiG
	if (pSPIHandle->SPI_Config.SPI_BusConfig == SPI_BUSCONFIG_FD) {
		// enable full duplex
		//CLEAR BIDI REG
		temp &= ~(1 << SPI_CR1_BIDIMODE);
	} else if (pSPIHandle->SPI_Config.SPI_BusConfig == SPI_BUSCONFIG_HD) {
		//enable half duplex
		//ENABLE BIDI REG
		temp |= (1 << SPI_CR1_BIDIMODE);

	} else if (pSPIHandle->SPI_Config.SPI_BusConfig == SPI_BUSCONFIG_SIMPLEX_RX) {
		// enable simplex receive only
		//clear bidi reg
		temp &= ~(1 << SPI_CR1_BIDIMODE);
// enbale rxonly reg
		temp |= (1 << SPI_CR1_RXONLY);
	}

	// clock speed config
	temp |= (pSPIHandle->SPI_Config.SPI_Sclk_Speed << SPI_CR1_BR);

	//DFF CONFIG
	temp |= (pSPIHandle->SPI_Config.SPI_DFF << SPI_CR1_DFF);
	// SSM CONFIG
	temp |= (pSPIHandle->SPI_Config.SPI_SSM << SPI_CR1_SSM);
	// cpol comfig
	temp |= (pSPIHandle->SPI_Config.SPI_CPOL << SPI_CR1_CPOL);
	//cpha cnfig
	temp |= (pSPIHandle->SPI_Config.SPI_CPHA << SPI_CR1_CPHA);

	pSPIHandle->pSPIx->CR1 = temp;
}

void SPI_Deit(SPI_RegDef_t *pSPIx) {
	if (pSPIx == SPI1) {
		SPI1_REG_RESET();
	}
	if (pSPIx == SPI2) {
		SPI2_REG_RESET();
	}
	if (pSPIx == SPI3) {
		SPI3_REG_RESET();
	}
	if (pSPIx == SPI4) {
		SPI4_REG_RESET();
	}
	if (pSPIx == SPI5) {
		SPI5_REG_RESET();
	}
}

void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len) {
	while (Len > 0) {
		while (((SpiGetFlagStatus(pSPIx, SPI_SR_TXE)) == FLAG_RESET));
					//keep looping till tx buffer empty ie txe flag set

		if (pSPIx->CR1 & (1 << SPI_CR1_DFF)) {//if dff 1 that is 16 bit data transfer that is 2 bytes at a time
			pSPIx->DR = *((uint16_t*) pTxBuffer); // typecast the pointer as 16bit
			Len--; //decrement len twice as 2 bytes have been passed
			Len--;
			pTxBuffer += 2; // increment the address by 2

		} else { //8 bit data transfer
			pSPIx->DR = *pTxBuffer;
			Len--;
			pTxBuffer++;

		}
		while (((SpiGetFlagStatus(pSPIx, SPI_SR_RXNE)) == FLAG_RESET));
		//when tx buffer empty then do dr read so as to b
							     uint8_t dummy;
							     dummy = pSPIx->DR;   // discard the byte that came back due to tx
							     (void)dummy;

	}
}

void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len){
	while(Len>0){
		while (((SpiGetFlagStatus(pSPIx, SPI_SR_RXNE)) == FLAG_RESET));

		if(pSPIx->CR1 & (1 << SPI_CR1_DFF)){
			*((uint16_t*)pRxBuffer)= pSPIx->DR;
			Len--;
			Len--;
			pRxBuffer+=2;
		} else{
			*pRxBuffer= pSPIx->DR;
			Len--;
			pRxBuffer++;
		}

	}
}
void SPI_ReceiveData_Dummy(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len){
    while(Len > 0){
        // Wait until TX buffer is empty so we can push a dummy byte
        while (((SpiGetFlagStatus(pSPIx, SPI_SR_TXE)) == FLAG_RESET));

        if (pSPIx->CR1 & (1 << SPI_CR1_DFF)) {
            // 16-bit dummy write to generate clock
            pSPIx->DR = 0xFFFF;
        } else {
            // 8-bit dummy write to generate clock
            pSPIx->DR = 0xFF;
        }

        // Now wait for the corresponding RXNE (data clocked in from slave)
        while (((SpiGetFlagStatus(pSPIx, SPI_SR_RXNE)) == FLAG_RESET));

        if(pSPIx->CR1 & (1 << SPI_CR1_DFF)){
            *((uint16_t*)pRxBuffer) = pSPIx->DR;
            Len--;
            Len--;
            pRxBuffer += 2;
        } else {
            *pRxBuffer = pSPIx->DR;
            Len--;
            pRxBuffer++;
        }
    }
}

/*
 * ENABLES OR DISBLES THE SPI IRQ
 */
void SPI_IRQConfig(uint8_t IRQNumber,uint8_t ENorDI){

	if(ENorDI==ENABLE){
	  if(IRQNumber<=31){// for irq 0 to 31
		  // enable ISER0
		  *ISER0_BASEADDR |= (1<<IRQNumber);//ISER enbale the specific irqno. interrupt
	  }

	  if(IRQNumber>31 && IRQNumber <64){// irq 32 to 63
       // enable ISER1
		  *ISER1_BASEADDR |= (1<<IRQNumber%32);
	  }

	 if(IRQNumber >= 64 && IRQNumber <96){//irq 64 to 95
		 *ISER2_BASEADDR |= (1<<IRQNumber%32);
	 }


}else{
	if(IRQNumber<=31){// for irq 0 to 31
			  // enable ICER0
			  *ICER0_BASEADDR |= (1<<IRQNumber);
		  }

		  if(IRQNumber>31 && IRQNumber <64){// irq 32 to 63
	       // enable ICER1
			  *ICER1_BASEADDR |= (1<<IRQNumber%32);
		  }

		 if(IRQNumber >= 64 && IRQNumber <96){//irq 64 to 95
			 *ICER2_BASEADDR |= (1<<IRQNumber%32);
		 }
}



}


/*
 * SPI irq priority
 */
void SPI_IRQ_Priority(uint8_t IRQNumber,uint8_t IRQPriority){

uint8_t temp1= (IRQNumber/4); //temp1 tells which Priority register to set for the irqno.
uint8_t temp2= ((IRQNumber%4)*8)+ 4; //temp2 tell how much to bit shift
//NOTE: plus 4 added to modulo of irqno. as the upper 4 bits of a 8bit slot assigned to a irq inside PR only
*(NVIC_IPR_BASEADDR + (temp1)) &= ~(0xF<<temp2);// clear last set priority if any
*(NVIC_IPR_BASEADDR + (temp1)) |= (IRQPriority<<temp2);

}



uint8_t SPI_SendData_IT (SPI_Handle_t *pSPIHandle,uint8_t *pTxBuffer,uint8_t Len){

	uint32_t status =  pSPIHandle->Txstate;// 2 diff state varibales Txstate and Rxstate neccesary tp ensure that
//system can be busy in tx and busy in rx at the same time
	if(status!=SPI_BUSY_IN_TX){
	pSPIHandle->TxLen=Len;
	pSPIHandle->Txstate=SPI_BUSY_IN_TX;
	// mark spi state as busy in transmetion so that no other int can interfere
	pSPIHandle->pTxBuffer=pTxBuffer;


	// enable the TXEIE (tx buffer empty enable interrupt ) when TXE(TX BUFFER EMPTY) FLAG IS SET
		pSPIHandle->pSPIx->CR2 |= (1<<SPI_CR2_TXEIE);



	}
	return status;
}
 uint8_t SPI_RecieveData_IT(SPI_Handle_t *pSPIHandle,uint8_t *pRxBuffer,uint8_t Len){
	 uint32_t status = pSPIHandle->Rxstate;//Rx state variable ensure that another receive data int cannot trigger without first receive data to completely finish ie len==0
	 if(status!=SPI_BUSY_IN_RX){
		 pSPIHandle->RxLen=Len;
		 	pSPIHandle->Rxstate=SPI_BUSY_IN_RX;
		 	// mark spi state as busy in RECEPTION so that no other int can interfere
		 	pSPIHandle->pRxBuffer=pRxBuffer;


		 	// enable the RXNEIE (rx buffer not empty enable interrupt ) when RXNE(RX BUFFER NOT EMPTY) FLAG IS SET
		 		pSPIHandle->pSPIx->CR2 |= (1<<SPI_CR2_RXNEIE);
	 }

	 return status;

 }

/*********************************************************************************************************************/
 /*
  * INT HANDLERS FOR RCEPTION TRANSMISSION AND OVER ERROR
  */

 void SPI_TXE_INTHandler(SPI_Handle_t *pHandle){



	 if (pHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF)) {//if dff 1 that is 16 bit data transfer that is 2 bytes at a time
	 			pHandle->pSPIx->DR = *((uint16_t*) pHandle->pTxBuffer); // typecast the pointer as 16bit


	 			pHandle->TxLen--; //decrement len twice as 2 bytes have been passed
	 			pHandle->TxLen--;
	 			pHandle->pTxBuffer += 2; // increment the address by 2
	 		}

	 else { //8 bit data transfer
	 			pHandle->pSPIx->DR = *(pHandle->pTxBuffer);


	 			pHandle->TxLen--;
	 			pHandle->pTxBuffer++;
	 		}

// stop transmission when Len becomes 0
	 if(!(pHandle->TxLen)){

	//when len is 0 diable the interrupt by reseting the CR1 TXEIE FLAG
	// reset the tx handle variables
		 SPI_CloseTransmission(pHandle);
	SPI_Application_Event_CallBack(pHandle,SPI_EVENT_TX_CMPLT);//send a call back to the cpu telling it that the transmission is complete






	 }




 }


 void SPI_RXNE_INTHandler(SPI_Handle_t *pHandle){
	 rxne_count++;
	 if(pHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF)){

	 			*((uint16_t*)pHandle->pRxBuffer)= (uint16_t)pHandle->pSPIx->DR;
	 			pHandle->RxLen--;
	 			pHandle->RxLen--;

	 			pHandle->pRxBuffer+=2;
	 		}
	 else{
	 			*(pHandle->pRxBuffer)=  (uint8_t)pHandle->pSPIx->DR;

	 			pHandle->RxLen--;

	 			pHandle->pRxBuffer++;
	 		}




if(!pHandle->RxLen){

	SPI_CloseReception(pHandle);
	SPI_Application_Event_CallBack(pHandle,SPI_EVENT_RX_CMPLT);

}
else{
	pHandle->pSPIx->DR=0xFF;//generate dummy byte//first dummy byte will be genrated in main
}

}



void SPI_OVR_err_INTHandler(SPI_Handle_t *pHandle){

	//clear the ovr flag
	if (pHandle->Txstate!=SPI_BUSY_IN_TX){
		uint8_t temp=0;
		temp = pHandle->pSPIx->DR;
		temp=pHandle->pSPIx->SR;
	}
	//inform the application
	SPI_Application_Event_CallBack(pHandle,SPI_EVENT_OVR_ERR);
//	pHandle->pSPIx->CR2 &= ~(1<<SPI_CR2_ERRIE);



}


/**************************************************************************************************************/



void SPI_IRQ_Handler(SPI_Handle_t *pHandle){



uint32_t temp1=0;
uint32_t temp2=0;

temp1=((pHandle->pSPIx->CR2)&(1<<SPI_CR2_TXEIE));// BASICALLY 2 BITS ONE BE SET TO ENABLE THE INT
temp2=((pHandle->pSPIx->SR)&(1<<SPI_SR_TXE));// THE OTHER A FLAG WHICH GETS SET WHEN CONDITION FOR INT OCCURS

if(temp1&&temp2){
	SPI_TXE_INTHandler(pHandle);
}

temp1=((pHandle->pSPIx->CR2)&(1<<SPI_CR2_RXNEIE));// CHECK IF RX BUFFER NOT EMPTY INT ENABLE BIT IS SET
temp2=((pHandle->pSPIx->SR)&(1<<SPI_SR_RXNE));//ALSO CHECK IF RX BUFFER NOT EMPTY FLAG IS SET

if(temp1&&temp2){
	SPI_RXNE_INTHandler(pHandle);
}


temp1=((pHandle->pSPIx->CR2)&(1<<SPI_CR2_ERRIE));
temp2=((pHandle->pSPIx->SR)&(1<<SPI_SR_OVR));

if(temp1&&temp2){
	SPI_OVR_err_INTHandler(pHandle);
}
}


void SPI_clrOvrFlag(SPI_RegDef_t *pSPIx){
	uint8_t temp=0;
			temp = pSPIx->DR;
			temp=pSPIx->SR;
}

void SPI_CloseReception(SPI_Handle_t *pHandle){
	pHandle->pSPIx->CR2 &= ~(1<<SPI_CR2_RXNEIE);//CLEAR THE RXNEIE BIT
			pHandle->pRxBuffer=NULL;
			pHandle->RxLen=0;
			pHandle->Rxstate=SPI_READY;
}

void SPI_CloseTransmission(SPI_Handle_t *pHandle){
		pHandle->pSPIx->CR2 &= ~(1<<SPI_CR2_TXEIE); // CLEAR THE TX bUFFER EMPTY INT ENABLE BIT
				pHandle->Txstate=SPI_READY;
				pHandle->pTxBuffer=NULL;
				pHandle->TxLen=0;
}


__attribute__((weak)) void SPI_Application_Event_CallBack(SPI_Handle_t *pHandle,uint8_t AppEv){
// weak implementation of callback function in case the application file does not define this
	// if defined by application file then the this is ignored by compiler due to weak attribute
	// look up undefined reference error if confused
}
void delay_1_second_approx(void) {
    // The target number depends on your clock speed and pipeline.
    // Assuming a ~16MHz clock, this might be roughly a second.
    // You will need to tune this number via trial and error.
    uint32_t loop_count = 1600000;

    for (volatile uint32_t i = 0; i < loop_count; i++) {
        __asm volatile("nop"); // Forces the CPU to do nothing for 1 cycle
    }
}
