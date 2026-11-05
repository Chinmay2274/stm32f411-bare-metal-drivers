#include <stdint.h>
#include <string.h>
#include "STM32F411E_uart_drivers.h"

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

uint8_t len = 5;
uint8_t tx[100];
uint8_t rx[100];
uint8_t bx[5];
volatile uint8_t tx_done = 0;
volatile uint8_t rx_done = 0;



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

int main(void){

	//tx_buffer_gen(tx,len);

	tx_usart2();
	rx_usart2();
	usart2_config();

	USART_IRQPriorityConfig(IRQ_NO_USART2, 0);
	USART_IRQInterruptConfig(IRQ_NO_USART2, ENABLE);

	// non-blocking: returns immediately, ISR feeds bytes on each TXE
	USART_ReceiveDataIT(&USART2Handle, bx, len);
	while(!rx_done);
	USART_SendDataIT(&USART2Handle, bx, len);

	// main is free here - TX happens in background via interrupts
	while(!tx_done);   // wait for callback to signal completion

	while(1);
}
