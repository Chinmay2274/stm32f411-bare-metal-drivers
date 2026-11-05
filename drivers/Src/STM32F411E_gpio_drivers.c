/*
 * STM32F411E_gpio_drivers.c
 *
 *  Created on: May 22, 2026
 *      Author: chinmay
 */
#include "STM32F411E_gpio_drivers.h"
//#include "STM32F411E.h"

/*
 * API FOR GPIO
 */


/*
 * this controls the clock config for a gpio port the base addr of the port pGPIOx and a Enable or diable varivale
 * ENorDI is required
 */

void GPIO_PCLCK_CONTROL(GPIO_RegDef_t *pGPIOx,uint8_t ENorDI){

	if(ENorDI==ENABLE){
		if(pGPIOx==GPIOA){
			GPIOA_PCLK_EN();
		}
		else if (pGPIOx==GPIOB){
			GPIOB_PCLK_EN();
		}
		else if (pGPIOx==GPIOC){
			GPIOC_PCLK_EN();
		}
		else if (pGPIOx==GPIOD){
					GPIOD_PCLK_EN();
				}
		else if (pGPIOx==GPIOE){
					GPIOE_PCLK_EN();
				}
		else if (pGPIOx==GPIOH){
					GPIOH_PCLK_EN();
				}
	}


else if(ENorDI==DISABLE){
	         if(pGPIOx==GPIOA){
				GPIOA_PCLK_DI();
			}
			else if (pGPIOx==GPIOB){
				GPIOB_PCLK_DI();
			}
			else if (pGPIOx==GPIOC){
				GPIOC_PCLK_DI();
			}
			else if (pGPIOx==GPIOD){
						GPIOD_PCLK_DI();
					}
			else if (pGPIOx==GPIOE){
						GPIOE_PCLK_DI();
					}
			else if (pGPIOx==GPIOH){
						GPIOH_PCLK_DI();
					}
}


}

void SYSCFG_PCLCK_CONTROL(uint8_t ENorDI){
	if(ENorDI==ENABLE){
		SYSCFG_PCLK_EN();
	}else{
		SYSCFG_PCLK_DI();
	}

}
/*
 * RETURNS THE VALUE OF PORT PA=0000,PB=0001...
 * FOR SYSCFG
 */
uint8_t GPIO_PORT_VALUE(GPIO_RegDef_t *pGPIOx){
if(pGPIOx==GPIOA){
	return 0;
}
if(pGPIOx==GPIOB){
	return 1;
}
if(pGPIOx==GPIOC){
	return 2;
}
if(pGPIOx==GPIOD){
	return 3;
}
if(pGPIOx==GPIOE){
	return 4;
}
if(pGPIOx==GPIOH){
	return 7;
}

}


void GPIO_Init(GPIO_Handle_t *pGPIOHandle){

	GPIO_PCLCK_CONTROL(pGPIOHandle->pGPIOx,ENABLE);
	uint32_t temp =0;
	// initializing port mode

	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG){
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode  <<   (2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER &= ~(0x3<<(2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));// clear the respec moder bits before initializing the,
		pGPIOHandle->pGPIOx->MODER |= temp;

	}

	//INTERRRUPT MODE
	else{
		temp = (0 <<   (2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
				pGPIOHandle->pGPIOx->MODER &= ~(0x3<<(2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));// clear the respec moder bits before initializing the,
				pGPIOHandle->pGPIOx->MODER |= temp;

	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode== GPIO_MODE_IT_FT){
		//enable the Falling trigger enable register for the specific pin
		EXTI->FTSR|= (1<<(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		//DISABLE THE RT REGISTER RSTR JUST IN CASE IT IS ACTIVE
		EXTI->RTSR&= ~(1<<(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	} if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode== GPIO_MODE_IT_RT){
		//enable the RISING EDGE trigger enable register for the specific pin
		EXTI->RTSR|= (1<<(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		//DIABLE THE FTSR REGISTER JUST IN CASE IT IS ACTIVE
		EXTI->FTSR&= ~(1<<(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	}
	//ENABLING BOTH FT AND RT TOGETHER FOR A PIN
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode== GPIO_MODE_IT_FRT)
	{
		//enable the Falling trigger enable register for the specific pin
				EXTI->FTSR|= (1<<(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));

	    //enable the RISING EDGE trigger enable register for the specific pin
				EXTI->RTSR|= (1<<(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	}

	//enable the syscfg register for specific pin
	/*
	 * temp1 tells which exti reg to acess 0,1,2,3 temp1=pin no./4
	 * temp2 decribes what bit to acess in the specific exti reg for the pin (temp2=pin no.%4)*4
	 */
	uint8_t temp1=0;
	temp1= pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber/4;
	uint8_t temp2=0;
	temp2=(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber%4)*4;
	SYSCFG_PCLCK_CONTROL(ENABLE);
uint8_t temp3 = GPIO_PORT_VALUE(pGPIOHandle->pGPIOx);//to know what value to write to the exti register PA=0000,PB=0001...
	SYSCFG->EXTICR[temp1] &= ~(0xF<< (temp2));
	SYSCFG->EXTICR[temp1] |= (temp3<<temp2);






//set the imr register for the interrurpt
  EXTI->IMR|=(1<<(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));



	}
	temp=0;
	            //initialize speed

		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed  <<   (2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3<<(2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->OSPEEDR |= temp;

		temp=0;
                // initialize push pull
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl  <<   (2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->PUPDR &= ~(0x3<<(2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
				pGPIOHandle->pGPIOx->PUPDR |= temp;

				temp=0;
				//initialize output type
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType  <<   (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->OTYPER &= ~(0x1<<(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	    		pGPIOHandle->pGPIOx->OTYPER|= temp;

				temp=0;

				//initialize alternative functionality
				if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN){

		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber <= 7){
  temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinALTFunMode  <<   (4*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->AFRL &= ~(0xF<<(4*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
  pGPIOHandle->pGPIOx->AFRL |= temp;
  temp=0;
		}

        if(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber > 7){
   temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinALTFunMode  <<   (4*((pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)%8)));
   pGPIOHandle->pGPIOx->AFRH &= ~(0xF<<(4*((pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)%8)));
   pGPIOHandle->pGPIOx->AFRH |= temp;
   temp=0;
				}

				}



}

/*
 * API do de initialize the gpio register
 * uses the AHB1RSTR register to reset the peripheral
 */

void GPIO_Deit(GPIO_RegDef_t *pGPIOx){
	if(pGPIOx==GPIOA){
		GPIOA_REG_RESET();
		}
		else if (pGPIOx==GPIOB){
			GPIOB_REG_RESET();
		}
		else if (pGPIOx==GPIOC){
			GPIOC_REG_RESET();
		}
		else if (pGPIOx==GPIOD){
			GPIOD_REG_RESET();
				}
		else if (pGPIOx==GPIOE){
			GPIOE_REG_RESET();
				}
		else if (pGPIOx==GPIOH){
			GPIOH_REG_RESET();
				}

}

/*
 * API to read from a input pin
 */

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber){

	uint8_t value=0;

	value =  (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x1);// we bit shift the value of relatvant register by the pinnumber to the right such that the register value is at the 0th index then it is anded with 1 to clear all other index

	return value;


}


uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber){

	uint16_t value=0;

	value =  (uint16_t)(pGPIOx->IDR );

	return value;


}

void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber,uint8_t value){
	if(value==GPIO_PIN_SET){
		//WRITE 1 TO THE CORRESPONDING GPIO PIN
		pGPIOx->ODR |= ((1 << PinNumber));

	}
	else{
		pGPIOx->ODR &= ~((1<< PinNumber));
	}
}

void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber,uint16_t value){
	pGPIOx->ODR= value;

}
/*
 * toggles the pin  that is it negates the current output
 */
 void GPIO_ToggleOutput_Pin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber){
	 pGPIOx->ODR ^=(1<<PinNumber);
 }

 /*
  * configures the specific irqno. interrupt
  */
void GPIO_IRQConfig(uint8_t IRQNumber,uint8_t ENorDI){

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

void GPIO_IRQ_Priority(uint8_t IRQNumber,uint8_t IRQPriority){

uint8_t temp1= (IRQNumber/4); //temp1 tells which Priority register to set for the irqno.
uint8_t temp2= ((IRQNumber%4)*8)+ 4; //temp2 tell how much to bit shift
//NOTE: plus 4 added to modulo of irqno. as the upper 4 bits of a 8bit slot assigned to a irq inside PR only
*(NVIC_IPR_BASEADDR + (temp1)) &= ~(0xF<<temp2);// clear last set priority if any
*(NVIC_IPR_BASEADDR + (temp1)) |= (IRQPriority<<temp2);

}


/*
 * does different work for different pinno,
 */
void GPIO_IRQ_Handler(uint8_t PinNumber){
	//

	if(EXTI->PR & (1<<PinNumber)){//for exti 0 ie pin 0 as int
		//CLEAR THE PENDING RESGITER
		EXTI->PR |=(1<<PinNumber);
	}

}






