/*
 * STM32F411E_usart.c
 *
 *  Created on: Jul 11, 2026
 *      Author: chinmay
 */
#include "STM32F411E_uart_drivers.h"

// HELPER FUNCTION set baud rate used in usart intialize function
void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate){


	//Variable to hold the APB clock
	uint32_t PCLKx;

	uint32_t usartdiv;

	//variables to hold Mantissa and Fraction values
	uint32_t M_part,F_part;

  uint32_t tempreg=0;

  //Get the value of APB bus clock in to the variable PCLKx
  if(pUSARTx == USART1 || pUSARTx == USART6)
  {
	   //USART1 and USART6 are hanging on APB2 bus
	   PCLKx = RCC_GetPCLK2Value();
  }else
  {
	   PCLKx = RCC_GetPCLK1Value();
  }

  //Check for OVER8 configuration bit
  if(pUSARTx->CR1 & (1 << USART_CR1_OVER8))
  {
	   //OVER8 = 1 , over sampling by 8
	   usartdiv = ((25 * PCLKx) / (2 *BaudRate));
  }else
  {
	   //over sampling by 16
	   usartdiv = ((25 * PCLKx) / (4 *BaudRate));
  }

  //Calculate the Mantissa part
  M_part = usartdiv/100;

  //Place the Mantissa part in appropriate bit position . refer USART_BRR
  tempreg |= M_part << 4;

  //Extract the fraction part
  F_part = (usartdiv - (M_part * 100));

  //Calculate the final fractional
  if(pUSARTx->CR1 & ( 1 << USART_CR1_OVER8))
   {
	  //OVER8 = 1 , over sampling by 8
	  F_part = ((( F_part * 8)+ 50) / 100)& ((uint8_t)0x07);

   }else
   {
	   //over sampling by 16
	   F_part = ((( F_part * 16)+ 50) / 100) & ((uint8_t)0x0F);

   }

  //Place the fractional part in appropriate bit position . refer USART_BRR
  tempreg |= F_part;

  //copy the value of tempreg in to BRR register
  pUSARTx->BRR = tempreg;
}



void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi){
	if (EnorDi == ENABLE) {
			if (pUSARTx == USART1) {
				USART1_PCLK_EN();
			} else if (pUSARTx == USART2) {
				USART2_PCLK_EN();
			} else if (pUSARTx == USART6) {
				USART6_PCLK_EN();
			}

		}

		else if (EnorDi == DISABLE) {
			if (pUSARTx == USART1) {
							USART1_PCLK_DI();
						} else if (pUSARTx == USART2) {
							USART2_PCLK_DI();
						} else if (pUSARTx == USART6) {
							USART6_PCLK_DI();
						}
		}
}

uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx , uint32_t FlagName){
	if (pUSARTx->SR & (1 << FlagName)) { // A MACRO FlagName is sent as an argument containsthe psotion of it in sr register
			return FLAG_SET;
		}
		return FLAG_RESET;
}


void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority){
	uint8_t temp1= (IRQNumber/4); //temp1 tells which Priority register to set for the irqno.
	uint8_t temp2= ((IRQNumber%4)*8)+ 4; //temp2 tell how much to bit shift
	//NOTE: plus 4 added to modulo of irqno. as the upper 4 bits of a 8bit slot assigned to a irq inside PR only
	*(NVIC_IPR_BASEADDR + (temp1)) &= ~(0xF<<temp2);// clear last set priority if any
	*(NVIC_IPR_BASEADDR + (temp1)) |= (IRQPriority<<temp2);
}

void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t ENorDI){
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

void USART_Init(USART_Handle_t *pUSARTHandle){
	USART_PeriClockControl(pUSARTHandle->pUSARTx, ENABLE);

uint32_t temp =0;

//cr1

// mode select
if(pUSARTHandle->USART_Config.USART_Mode==USART_MODE_ONLY_TX){
	temp |= (1<<3);
}
if(pUSARTHandle->USART_Config.USART_Mode==USART_MODE_ONLY_RX){
	temp |= (1<<2);
}
if(pUSARTHandle->USART_Config.USART_Mode==USART_MODE_TXRX){
	temp |= (1<<2);// enable both tx and rx
	temp |= (1<<3);
}
//parity select
if(pUSARTHandle->USART_Config.USART_ParityControl==USART_PARITY_EN_EVEN){
	temp |= (1<<10); //enable parity
	temp |= (0<<9);//set parity to even
}

if(pUSARTHandle->USART_Config.USART_ParityControl==USART_PARITY_EN_ODD){
	temp |= (1<<10);
	temp |= (1<<9);
}
if(pUSARTHandle->USART_Config.USART_ParityControl==USART_PARITY_DISABLE){
temp|=(0<<10);
}
//word len select
if(pUSARTHandle->USART_Config.USART_WordLength==USART_WORDLEN_8BITS){
	temp |=(0<<12);
}
if(pUSARTHandle->USART_Config.USART_WordLength==USART_WORDLEN_9BITS){
	temp |=(1<<12);
}
//temp|=(1<<13);//usart enable to turn on usart peripheral
pUSARTHandle->pUSARTx->CR1= temp;
temp=0;
//cr2

// no of stop bits select
temp |= (pUSARTHandle->USART_Config.USART_NoOfStopBits<<12);

pUSARTHandle->pUSARTx->CR2=temp;
temp=0;
//cr3

//rts and cts setuo
if ( pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS)
	{
		temp |= ( 1 << 9);
	}else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS)
	{
		temp |= (1<<8);
	}else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS)
	{
		temp |= (1<<8);
		temp |= ( 1 << 9);
	}
temp=0;
//USART_SetBaudRate(pUSARTHandle->pUSARTx,pUSARTHandle->BaudRate);
pUSARTHandle->pUSARTx->BRR = 1667;
temp|=(1<<13);//usart enable to turn on usart peripheral
pUSARTHandle->pUSARTx->CR1|= temp;
temp=0;
}


void USART_SendData(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len){
	while (Len > 0) {
			while (((USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_SR_TXE)) == FLAG_RESET));
						//keep looping till tx buffer empty ie txe flag set

			uint16_t *pdata;

			if(pUSARTHandle->USART_Config.USART_WordLength==USART_WORDLEN_9BITS){

				pdata = (uint16_t*)pTxBuffer;//typecast as 16 bit array

				pUSARTHandle->pUSARTx->DR=(*pdata & (uint16_t)0x1FF);//CLEAR THE GARABGE VALUE IN THE LAST 6 BITS AS TYPECASTING TO LARGER VALUE LEAVES GARBAGE IN THE EMPTY BITS

				if(pUSARTHandle->USART_Config.USART_ParityControl==USART_PARITY_DISABLE){
					++pTxBuffer;//if parity diabled whole 9 bits must br shifted via code
					++pTxBuffer;// hence 16 bits of txbuffer consumed hence incrment 2 address
					Len--;

				}
				else{
					++pTxBuffer;// parity means only 8 bits shifted via software into dr the parity bit is send via hardware
					Len--;
				}
			}
			else{
				if(pUSARTHandle->USART_Config.USART_ParityControl==USART_PARITY_DISABLE){
				pUSARTHandle->pUSARTx->DR=*pTxBuffer;

				++pTxBuffer;
				Len--;
				}
				else{
					pUSARTHandle->pUSARTx->DR=(*pTxBuffer & (uint8_t)0x7F);

				}
			}
	}
	while (((USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_SR_TC)) == FLAG_RESET));// wait till transmission complete

}

void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len){
	while (Len > 0) {
				while (((USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_SR_RXNE)) == FLAG_RESET));


				if(pUSARTHandle->USART_Config.USART_WordLength==USART_WORDLEN_9BITS){

					 *((uint16_t*)pRxBuffer)=(pUSARTHandle->pUSARTx->DR& (uint16_t)0x01FF);


					if(pUSARTHandle->USART_Config.USART_ParityControl==USART_PARITY_DISABLE){
						++pRxBuffer;
						++pRxBuffer;
						Len--;
					}
					else{
						++pRxBuffer;//no parity means only 8 bits shifted via software into dr the parity bit is send via hardware
						Len--;
					}
				}

				else{// for 8bit
               if(pUSARTHandle->USART_Config.USART_ParityControl==USART_PARITY_DISABLE){// no parity
					*pRxBuffer=(pUSARTHandle->pUSARTx->DR);
					++pRxBuffer;
					Len--;
				}
               else{// parity
            	   *pRxBuffer=(pUSARTHandle->pUSARTx->DR & (uint8_t)0x7F);
            	   					++pRxBuffer;
            	   					Len--;
               }
				}

		}


	}

uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len){

	uint32_t status =  pUSARTHandle->Txstate;// 2 diff state varibales Txstate and Rxstate neccesary tp ensure that
//system can be busy in tx and busy in rx at the same time
	if(status!=USART_BUSY_IN_TX){
	pUSARTHandle->TxLen=Len;
	pUSARTHandle->Txstate=USART_BUSY_IN_TX;
	// mark USART state as busy in transmetion so that no other int can interfere
	pUSARTHandle->pTxBuffer=pTxBuffer;


	// enable the TXEIE (tx buffer empty enable interrupt ) when TXE(TX BUFFER EMPTY) FLAG IS SET
		pUSARTHandle->pUSARTx->CR1 |= (1<<USART_CR1_TXEIE);



	}
	return status;

}



uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len){
	uint32_t status = pUSARTHandle->Rxstate;
	if(status!=USART_BUSY_IN_RX){
		pUSARTHandle->RxLen=Len;
		pUSARTHandle->Rxstate=USART_BUSY_IN_RX;
		pUSARTHandle->pRxBuffer=pRxBuffer;

		pUSARTHandle->pUSARTx->CR1 |= (1<<USART_CR1_RXNEIE);

	}
	return status;
}


/******************Interrupt helper functions*************************************/
void USART_TXE_INTHandler(USART_Handle_t *pUSARTHandle){
	uint16_t temp1=0;
		uint16_t temp2=0;
	//Implement the code to check the state of TXE bit in the SR
		temp1 = pUSARTHandle->pUSARTx->SR & ( 1 << USART_SR_TXE);

		//Implement the code to check the state of TXEIE bit in CR1
		temp2 = pUSARTHandle->pUSARTx->CR1 & ( 1 << USART_CR1_TXEIE);


		if(temp1 && temp2 )
		{
			//this interrupt is because of TXE

			if(pUSARTHandle->Txstate == USART_BUSY_IN_TX)
			{
				//Keep sending data until Txlen reaches to zero
				if(pUSARTHandle->TxLen > 0)
				{
					//Check the USART_WordLength item for 9BIT or 8BIT in a frame
					if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
					{
						//if 9BIT , load the DR with 2bytes masking the bits other than first 9 bits
						uint16_t *pdata;
						pdata = (uint16_t*) pUSARTHandle->pTxBuffer;

						//loading only first 9 bits , so we have to mask with the value 0x01FF
						pUSARTHandle->pUSARTx->DR = (*pdata & (uint16_t)0x01FF);

						//check for USART_ParityControl
						if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
						{
							//No parity is used in this transfer , so, 9bits of user data will be sent
							//Implement the code to increment pTxBuffer twice
							pUSARTHandle->pTxBuffer++;
							pUSARTHandle->pTxBuffer++;

							//Implement the code to decrement the length
							pUSARTHandle->TxLen--;
						}
						else
						{
							//Parity bit is used in this transfer . so , 8bits of user data will be sent
							//The 9th bit will be replaced by parity bit by the hardware
							pUSARTHandle->pTxBuffer++;

							//Implement the code to decrement the length
							pUSARTHandle->TxLen--;
						}
					}
					else
					{
						//This is 8bit data transfer
						pUSARTHandle->pUSARTx->DR = (*pUSARTHandle->pTxBuffer  & (uint8_t)0xFF);

						//Implement the code to increment the buffer address
						pUSARTHandle->pTxBuffer++;

						//Implement the code to decrement the length
						pUSARTHandle->TxLen--;
					}

				}
				if (pUSARTHandle->TxLen == 0 )
				{
					//TxLen is zero
					//Implement the code to clear the TXEIE bit (disable interrupt for TXE flag )
					pUSARTHandle->pUSARTx->CR1 &= ~( 1 << USART_CR1_TXEIE );
					pUSARTHandle->Txstate = USART_READY;
					USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_TX_CMPLT);
				}
			}
		}

}
void USART_RXNE_INTHandler(USART_Handle_t *pUSARTHandle){
	uint16_t temp1=0;
	uint16_t temp2=0;
	//Implement the code to check the state of RXNE bit in the SR
		temp1 = pUSARTHandle->pUSARTx->SR & ( 1 << USART_SR_RXNE);

		//Implement the code to check the state of RXNEIE bit in CR1
		temp2 = pUSARTHandle->pUSARTx->CR1 & ( 1 << USART_CR1_RXNEIE);


		if(temp1 && temp2 )
		{
			//this interrupt is because of rxne

			if(pUSARTHandle->Rxstate == USART_BUSY_IN_RX)
			{
				//RXNE is set so receive data
				if(pUSARTHandle->RxLen > 0)
				{
					//Check the USART_WordLength to decide whether we are going to receive 9bit of data in a frame or 8 bit
					if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
					{
						//We are going to receive 9bit data in a frame

						//Now, check are we using USART_ParityControl control or not
						if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
						{
							//No parity is used. so, all 9bits will be of user data

							//read only first 9 bits so mask the DR with 0x01FF
							*((uint16_t*) pUSARTHandle->pRxBuffer) = (pUSARTHandle->pUSARTx->DR  & (uint16_t)0x01FF);

							//Now increment the pRxBuffer two times
							pUSARTHandle->pRxBuffer++;
							pUSARTHandle->pRxBuffer++;

							//Implement the code to decrement the length
							pUSARTHandle->RxLen--;
						}
						else
						{
							//Parity is used. so, 8bits will be of user data and 1 bit is parity
							 *pUSARTHandle->pRxBuffer = (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);

							 //Now increment the pRxBuffer
							 pUSARTHandle->pRxBuffer++;

							 //Implement the code to decrement the length
							pUSARTHandle->RxLen--;
						}
					}
					else
					{
						//We are going to receive 8bit data in a frame

						//Now, check are we using USART_ParityControl control or not
						if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
						{
							//No parity is used , so all 8bits will be of user data

							//read 8 bits from DR
							 *pUSARTHandle->pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);
						}

						else
						{
							//Parity is used, so , 7 bits will be of user data and 1 bit is parity

							//read only 7 bits , hence mask the DR with 0X7F
							 *pUSARTHandle->pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->DR  & (uint8_t)0x7F);

						}

						//Now , increment the pRxBuffer
						pUSARTHandle->pRxBuffer++;

						//Implement the code to decrement the length
						pUSARTHandle->RxLen--;
					}


				}//if of >0

				if( pUSARTHandle->RxLen == 0)
				{
					//disable the rxne
					pUSARTHandle->pUSARTx->CR1 &= ~( 1 << USART_CR1_RXNEIE );
					pUSARTHandle->Rxstate = USART_READY;
					USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_RX_CMPLT);
				}
			}
		}
}
__attribute__((weak)) void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,uint8_t AppEv){
// weak implementation of callback function in case the application file does not define this
	// if defined by application file then the this is ignored by compiler due to weak attribute
	// look up undefined reference error if confused
}

