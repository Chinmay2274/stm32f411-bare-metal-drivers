/*
 * STM32F411E_gpio_drivers.h
 *
 *  Created on: May 22, 2026
 *      Author: chinmay
 */

#include <stdint.h>
#include "STM32F411E.h"
#ifndef INC_STM32F411E_GPIO_DRIVERS_H_
#define INC_STM32F411E_GPIO_DRIVERS_H_

/*
 * GPIO pin configuration struct
 */
typedef struct {
 uint8_t GPIO_PinMode;
 uint8_t GPIO_PinNumber;
 uint8_t GPIO_PinSpeed;
 uint8_t GPIO_PinPuPdControl;
 uint8_t GPIO_PinOPType;
 uint8_t GPIO_PinALTFunMode;

}GPIO_PinConfig_t;


/*
 * GPIO Handle struct
 */
typedef struct {
GPIO_RegDef_t *pGPIOx ;
GPIO_PinConfig_t GPIO_PinConfig;

} GPIO_Handle_t;

// to access pGPIOx->




void GPIO_PCLCK_CONTROL(GPIO_RegDef_t *pGPIOx, uint8_t ENorDI);
void SYSCFG_PCLCK_CONTROL(uint8_t ENorDI);

void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_Deit(GPIO_RegDef_t *pGPIOx);

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint16_t value);
void GPIO_ToggleOutput_Pin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t ENorDI);

uint8_t GPIO_PORT_VALUE(GPIO_RegDef_t *pGPIOx);
void GPIO_IRQ_Priority(uint8_t IRQNumber,uint8_t IRQPriority);
void GPIO_IRQ_Handler(uint8_t PinNumber);

#endif /* INC_STM32F411E_GPIO_DRIVERS_H_ */

