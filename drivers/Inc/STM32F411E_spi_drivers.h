/*
 * STM32F411E_spi_drivers.h
 *  Created on: May 30, 2026
 *      Author: chinmay
 *
 *
 */
#include "STM32F411E.h"
#ifndef INC_STM32F411E_SPI_DRIVERS_H_
#define INC_STM32F411E_SPI_DRIVERS_H_

typedef struct {
uint8_t  SPI_DeviceMode; // device master or slave
uint8_t  SPI_BusConfig;
uint8_t  SPI_Sclk_Speed;
uint8_t  SPI_DFF;
uint8_t  SPI_SSM;
uint8_t  SPI_CPOL;
uint8_t  SPI_CPHA;
}SPI_PinConfig_t;


typedef struct{
 SPI_RegDef_t  *pSPIx;
 SPI_PinConfig_t  SPI_Config;
 uint8_t   *pTxBuffer;
 uint8_t   *pRxBuffer;
 uint8_t    TxLen;
 uint8_t   RxLen;
 uint8_t   Txstate;
 uint8_t   Rxstate;


}SPI_Handle_t;


#define SPI_READY      0
#define SPI_BUSY_IN_TX 1
#define SPI_BUSY_IN_RX 2

/*
 * DEVICE MODE MACROS
 */
#define SPI_DEVICE_MODE_MASTER 1
#define SPI_DEVICE_MODE_SLAVE 0

/*
 * BUS CONFIG MACROS
 *
 * NOTE: FOR BUS CONFIG THERE IS NO NEED FOR A SIMPLEX TRANSMIT ONLY MACRO AS YOU SIMPLY IGNORE THE RECIEVE BUFFER
 *
 */
#define SPI_BUSCONFIG_FD 1  // FULL DUPLEX
#define SPI_BUSCONFIG_HD 2  //HALF DUPLEX
#define SPI_BUSCONFIG_SIMPLEX_RX 3  // SIMPLEX MASTER RECIEVER


/*
 * SCLK MACROS
 */

#define SPI_SCLK_SPEED_DIV2 0
#define SPI_SCLK_SPEED_DIV4 1
#define SPI_SCLK_SPEED_DIV8 2
#define SPI_SCLK_SPEED_DIV16 3
#define SPI_SCLK_SPEED_DIV32 4
#define SPI_SCLK_SPEED_DIV64 5
#define SPI_SCLK_SPEED_DIV128 6
#define SPI_SCLK_SPEED_DIV256 7

/*
 * DFF MACROS 8BIT OR 16BIT
 */
#define SPI_DFF_8BIT 0
#define SPI_DFF_16BIT 1

/*
 * SSM MACROS HARDWARE MANGED OR SOFTWARE MANGED SLAVE SELECT
 */
#define SPI_SSM_SW_EN 1
#define SPI_SSM_SW_DI 0//BY DEFAULT SW IS DIASBLE IE HARDWARE MANGED BY DEFAULT

/*
 * CPOL MACROS
 */
#define CPOL_LOW 0
#define CPOL_HIGH 1

/*
 * CPHA MACROS
 */
#define CPHA_LOW 0
#define CPHA_HIGH 1

/*
 * SPI CR1 REGISTER POSITION MACROS
 */
#define SPI_CR1_CPHA 0
#define SPI_CR1_CPOL 1
#define SPI_CR1_MSTR 2
#define SPI_CR1_BR   3
#define SPI_CRI_SPE     6
#define SPI_CR1_LSBFIRST 7
#define SPI_CR1_SSI 8
#define SPI_CR1_SSM 9
#define SPI_CR1_RXONLY 10
#define SPI_CR1_DFF 11
#define SPI_CR1_CRCNEXT 12
#define SPI_CR1_CRCEN 13
#define SPI_CR1_BIDIOE 14
#define SPI_CR1_BIDIMODE 15

/*
 * SPI CR2 REGISTER POSITION MACROS
 */
#define SPI_CR2_ERRIE  5
#define SPI_CR2_RXNEIE 6
#define SPI_CR2_TXEIE  7



/*
 * SPI FLAG POSITION MACROS
 */
#define SPI_SR_RXNE 0
#define SPI_SR_TXE 1
#define SPI_SR_OVR 6
#define SPI_SR_BSY 7

/*
 * SPI STATUS MACROS
 */
#define SPI_EVENT_TX_CMPLT 1
#define SPI_EVENT_RX_CMPLT 2
#define SPI_EVENT_OVR_ERR  3


void ssi_control(SPI_RegDef_t *pSPIx,uint8_t ENorDI);
void spe_control(SPI_RegDef_t *pSPIx,uint8_t ENorDI);

void SPI_PCLK_CONTROL(SPI_RegDef_t *pSPIx, uint8_t ENorDI);

void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_Deit(SPI_RegDef_t *pSPIx);

void SPI_SendData(SPI_RegDef_t *pSPIx,uint8_t *pTxBuffer,uint32_t Len);//*pSPIx points to the spi pin output value is sent to the Tx register
// NOTE: Len ie Len of data to be sent 8 bit or 16 bit is always denoted as 32bit integer
void  SPI_ReceiveData(SPI_RegDef_t *pSPIx,uint8_t *pRxBuffer,uint32_t Len);

void SPI_IRQConfig(uint8_t IRQNumber, uint8_t ENorDI);

void SPI_IRQ_Priority(uint8_t IRQNumber,uint8_t IRQPriority);


void SPI_IRQ_Handler(SPI_Handle_t *pHandle);

uint8_t SpiGetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName);

uint8_t SPI_SendData_IT (SPI_Handle_t *pSPIHandle,uint8_t *pTxBuffer,uint8_t Len);

void SPI_clrOvrFlag(SPI_RegDef_t *pSPIx);
void SPI_CloseReception(SPI_Handle_t *pSPIHandle);
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle);
void SPI_Application_Event_CallBack(SPI_Handle_t *pSPIHandle,uint8_t AppEv);

uint8_t SPI_SendData_IT (SPI_Handle_t *pSPIHandle,uint8_t *pTxBuffer,uint8_t Len);
uint8_t SPI_RecieveData_IT(SPI_Handle_t *pSPIHandle,uint8_t *pRxBuffer,uint8_t Len);
void SPI_ReceiveData_Dummy(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);

#endif /* INC_STM32F411E_SPI_DRIVERS_H_ */
