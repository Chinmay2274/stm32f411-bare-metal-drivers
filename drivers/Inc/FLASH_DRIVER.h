/*
 * FLASH.h
 *
 *  Created on: Jun 14, 2026
 *      Author: chinmay
 */

#ifndef INC__FLASH_DRIVER_H_
#define INC__FLASH_DRIVER_H_
#include "STM32F411E.h"
// Define a struct representing your physical flash chip
typedef struct {
	SPI_RegDef_t   *pSPI;       // The SPI peripheral it's connected to  // can be merged into spi_main fix this
    GPIO_RegDef_t  *pCSPort;    // The GPIO Port for CS (e.g., GPIOB)
    uint8_t        CSPin;      // The GPIO Pin for CS (e.g., GPIO_PIN_NO_12)
    SPI_PinConfig_t   SPI_Config;   // can be merged into spi_main fix this
    SPI_Handle_t    *SPI_Main;
} Flash_Device_t;

void Flash_Read_Async(Flash_Device_t *pHandle, uint32_t target_addr, uint8_t *rx, uint8_t *tx, uint8_t len,uint8_t cmd);
void write_to_segment(Flash_Device_t *pHandle,uint32_t target_addr,uint8_t *tx,uint8_t Len);
void read_jedec_id(Flash_Device_t *pHandle, uint8_t *id_out /* 3 bytes */);
void read_data(Flash_Device_t *pHandle, uint32_t target_addr, uint8_t *rx, uint8_t *tx, uint8_t len,uint8_t cmd);
void read_data(Flash_Device_t *pHandle, uint32_t target_addr, uint8_t *rx, uint8_t *tx, uint8_t len,uint8_t cmd);
void write_data(Flash_Device_t *pHandle,uint32_t target_addr,uint8_t *tx,uint8_t Len);

#endif /* INC__FLASH_DRIVER_H_ */
