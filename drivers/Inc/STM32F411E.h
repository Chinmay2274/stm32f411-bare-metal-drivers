/*
 * STM32F411E.h
 *
 *  Created on: May 21, 2026
 *      Author: chinmay
 */
#include <stdint.h>

#ifndef INC_STM32F411E_H_
#define INC_STM32F411E_H_

/*************************** START: Processor Specific Details ***************************/

#define ISER0_BASEADDR      ((volatile uint32_t*)(0xE000E100U))// Need to make baseaddr as a pointer as we dindt create a struct for it
#define ISER1_BASEADDR      ((volatile uint32_t*)(0xE000E104U))
#define ISER2_BASEADDR      ((volatile uint32_t*)(0xE000E108U))
#define ISER3_BASEADDR      ((volatile uint32_t*)(0xE000E10CU))


#define ICER0_BASEADDR       ((volatile uint32_t*)0xE000E180U)
#define ICER1_BASEADDR       ((volatile uint32_t*)(0xE000E184U))
#define ICER2_BASEADDR       ((volatile uint32_t*)(0xE000E188U))
#define ICER3_BASEADDR       ((volatile uint32_t*)(0xE000E18CU))


#define NVIC_IPR_BASEADDR    ((volatile uint32_t*)0xE000E400U)


#define DWT_CYCCNT_BASEADDR  ((volatile uint32_t*)0xE0001004U)// COUNTS NO. OF CPU CLCK CYCLES ELAPSED

#define DEMCR_BASEADDR       ((volatile uint32_t*)0xE000EDFCU)// SETTING ITS BIT 24 ENABLE DWT REGISTERS

#define DWT_CTRL_BASEADDR             ((volatile uint32_t*)0xE0001000U)//SETTING BIT 0 ENBALES THE CYCCNT (SHOULD TOGGLE AFTER TOGGLING DEMCR FIRST)


/**************************** END: Processor Specific Details ****************************/




/*
 * creating memory macros
 */
#define SRAM1_BASEADDR 0x20000000U
#define SRAM           SRAM1_BASEADDR
#define FLASH_BASEADDR 0x08000000U
#define ROM            0x1FFF0000U // LOOK FOR SYSTEM MEMEORY IN REFERENCE MANUAL


/*
 * creating system bus macros
 */

#define AHB1_BASEADDR 0x40020000U
#define AHB2_BASEADDR 0x50000000U
#define APB1_BASEADDR 0x40000000U
#define APB2_BASEADDR 0x40010000U


/*
 * peripheral macros for AHB1 bus
 */

#define GPIOA_BASEADDR  AHB1_BASEADDR
#define GPIOB_BASEADDR (AHB1_BASEADDR + 0X0400)
#define GPIOC_BASEADDR (AHB1_BASEADDR + 0X0800)
#define GPIOD_BASEADDR (AHB1_BASEADDR + 0X0C00)
#define GPIOE_BASEADDR (AHB1_BASEADDR + 0X1000)
#define GPIOH_BASEADDR (AHB1_BASEADDR + 0X1C00)
#define RCC_BASEADDR   (AHB1_BASEADDR + 0X3800)


/*
 * PERIPHERAL MACROS FOR APB2 BUS
 */
#define EXTI_BASEADDR   (APB2_BASEADDR+ 0X3C00)
#define SYSCFG_BASEADDR (APB2_BASEADDR+ 0X3800)
#define SPI1_BASEADDR   (APB2_BASEADDR + 0X3000)
#define SPI4_BASEADDR   (APB2_BASEADDR + 0X3400)
#define SPI5_BASEADDR   (APB2_BASEADDR + 0X5000)
#define USART6_BASEADDR  (APB2_BASEADDR+ 0X1000)
#define USART1_BASEADDR  (APB2_BASEADDR+ 0X1400)

/*
 * PERIPHERAL MACROS FOR APB1 BUS
 */
#define SPI2_BASEADDR   (APB1_BASEADDR + 0X3800)
#define SPI3_BASEADDR   (APB1_BASEADDR + 0X3C00)
#define USART2_BASEADDR  (APB1_BASEADDR + 0X4400)




/*
 *gpio register struct
 */
typedef struct
{
    volatile uint32_t MODER;    /* 0x00: GPIO port mode register                 */
    volatile uint32_t OTYPER;   /* 0x04: GPIO port output type register          */
    volatile uint32_t OSPEEDR;  /* 0x08: GPIO port output speed register         */
    volatile uint32_t PUPDR;    /* 0x0C: GPIO port pull-up/pull-down register    */
    volatile uint32_t IDR;      /* 0x10: GPIO port input data register           */
    volatile uint32_t ODR;      /* 0x14: GPIO port output data register          */
    volatile uint32_t BSRR;     /* 0x18: GPIO port bit set/reset register        */
    volatile uint32_t LCKR;     /* 0x1C: GPIO port configuration lock register   */
    volatile uint32_t AFRL;     /* 0x20: GPIO alternate function low register    */
    volatile uint32_t AFRH;     /* 0x24: GPIO alternate function high register   */
} GPIO_RegDef_t;

/*
 * rcc rgeister struct
 *
 */

typedef struct
{
    volatile uint32_t CR;            /* 0x00: RCC clock control register                   */
    volatile uint32_t PLLCFGR;       /* 0x04: RCC PLL configuration register               */
    volatile uint32_t CFGR;          /* 0x08: RCC clock configuration register             */
    volatile uint32_t CIR;           /* 0x0C: RCC clock interrupt register                 */
    volatile uint32_t AHB1RSTR;      /* 0x10: RCC AHB1 peripheral reset register           */
    volatile uint32_t AHB2RSTR;      /* 0x14: RCC AHB2 peripheral reset register           */
    uint32_t RESERVED0[2];           /* 0x18 - 0x1C: Reserved padding                      */

    volatile uint32_t APB1RSTR;      /* 0x20: RCC APB1 peripheral reset register           */
    volatile uint32_t APB2RSTR;      /* 0x24: RCC APB2 peripheral reset register           */
    uint32_t RESERVED1[2];           /* 0x28 - 0x2C: Reserved padding                      */

    volatile uint32_t AHB1ENR;       /* 0x30: RCC AHB1 peripheral clock enable register    */
    volatile uint32_t AHB2ENR;       /* 0x34: RCC AHB2 peripheral clock enable register    */
    uint32_t RESERVED2[2];           /* 0x38 - 0x3C: Reserved padding                      */

    volatile uint32_t APB1ENR;       /* 0x40: RCC APB1 peripheral clock enable register    */
    volatile uint32_t APB2ENR;       /* 0x44: RCC APB2 peripheral clock enable register    */
    uint32_t RESERVED3[2];           /* 0x48 - 0x4C: Reserved padding                      */

    volatile uint32_t AHB1LPENR;     /* 0x50: RCC AHB1 peripheral clock enable in LP mode  */
    volatile uint32_t AHB2LPENR;     /* 0x54: RCC AHB2 peripheral clock enable in LP mode  */
    uint32_t RESERVED4[2];           /* 0x58 - 0x5C: Reserved padding                      */

    volatile uint32_t APB1LPENR;     /* 0x60: RCC APB1 peripheral clock enable in LP mode  */
    volatile uint32_t APB2LPENR;     /* 0x64: RCC APB2 peripheral clock enable in LP mode  */
    uint32_t RESERVED5[2];           /* 0x68 - 0x6C: Reserved padding                      */

    volatile uint32_t BDCR;          /* 0x70: RCC Backup domain control register           */
    volatile uint32_t CSR;           /* 0x74: RCC clock control & status register          */
    uint32_t RESERVED6[2];           /* 0x78 - 0x7C: Reserved padding                      */

    volatile uint32_t SSCGR;         /* 0x80: RCC spread spectrum clock generation register*/
    volatile uint32_t PLLI2SCFGR;    /* 0x84: RCC PLLI2S configuration register            */
    uint32_t RESERVED7;              /* 0x88: Reserved padding                             */
    volatile uint32_t DCKCFGR;       /* 0x8C: RCC dedicated clocks configuration register  */
} RCC_RegDef_t;

/*
 * EXTI register struct
 */

typedef struct
{
    volatile uint32_t IMR;    /* 0x00: EXTI Interrupt mask register          */
    volatile uint32_t EMR;    /* 0x04: EXTI Event mask register              */
    volatile uint32_t RTSR;   /* 0x08: EXTI Rising trigger selection register*/
    volatile uint32_t FTSR;   /* 0x0C: EXTI Falling trigger selection register*/
    volatile uint32_t SWIER;  /* 0x10: EXTI Software interrupt event register*/
    volatile uint32_t PR;     /* 0x14: EXTI Pending register                 */
} EXTI_RegDef_t;


/*
 * SYSCFG register struct
 */
typedef struct
{
    volatile uint32_t MEMRMP;     /* 0x00 */
    volatile uint32_t PMC;        /* 0x04 */
    volatile uint32_t EXTICR[4];  /* 0x08 - 0x14: EXTICR[0]=EXTICR1, EXTICR[1]=EXTICR2, etc. */
    uint32_t RESERVED[2];         /* 0x18 - 0x1C */
    volatile uint32_t CMPCR;      /* 0x20 */
} SYSCFG_RegDef_t;


typedef struct {
    volatile uint32_t CR1;     /* SPI Control Register 1,       Address offset: 0x00 */
    volatile uint32_t CR2;     /* SPI Control Register 2,       Address offset: 0x04 */
    volatile uint32_t SR;      /* SPI Status Register,          Address offset: 0x08 */
    volatile uint32_t DR;      /* SPI Data Register,            Address offset: 0x0C */
    volatile uint32_t CRCPR;   /* SPI CRC Polynomial Register,  Address offset: 0x10 */
    volatile uint32_t RXCRCR;  /* SPI Rx CRC Register,          Address offset: 0x14 */
    volatile uint32_t TXCRCR;  /* SPI Tx CRC Register,          Address offset: 0x18 */
    volatile uint32_t I2SCFGR; /* SPI I2S Configuration Reg,    Address offset: 0x1C */
    volatile uint32_t I2SPR;   /* SPI I2S Prescaler Register,   Address offset: 0x20 */
} SPI_RegDef_t;



typedef struct
{
    volatile uint32_t SR;      /* USART Status register,                    Address offset: 0x00 */
    volatile uint32_t DR;      /* USART Data register,                      Address offset: 0x04 */
    volatile uint32_t BRR;     /* USART Baud rate register,                 Address offset: 0x08 */
    volatile uint32_t CR1;     /* USART Control register 1,                 Address offset: 0x0C */
    volatile uint32_t CR2;     /* USART Control register 2,                 Address offset: 0x10 */
    volatile uint32_t CR3;     /* USART Control register 3,                 Address offset: 0x14 */
    volatile uint32_t GTPR;    /* USART Guard time and prescaler register,  Address offset: 0x18 */
}USART_RegDef_t;





/* * Peripheral Definitions
 * Typecasting base addresses to register structure pointers
 */
#define GPIOA             ((GPIO_RegDef_t *)GPIOA_BASEADDR) //equivalent to this but without creating a variable GPIO_RegDef_t *pGPIOA = (GPIO_RegDef_t *)GPIOA_BASEADDR;
#define GPIOB             ((GPIO_RegDef_t *)GPIOB_BASEADDR)
#define GPIOC             ((GPIO_RegDef_t *)GPIOC_BASEADDR)
#define GPIOD             ((GPIO_RegDef_t *)GPIOD_BASEADDR)
#define GPIOE             ((GPIO_RegDef_t *)GPIOE_BASEADDR)
#define GPIOH             ((GPIO_RegDef_t *)GPIOH_BASEADDR)

#define RCC               ((RCC_RegDef_t *)RCC_BASEADDR)

#define EXTI              ((EXTI_RegDef_t *)EXTI_BASEADDR)

#define SYSCFG            ((SYSCFG_RegDef_t*)SYSCFG_BASEADDR)

#define SPI1               ((SPI_RegDef_t *)SPI1_BASEADDR)
#define SPI2               ((SPI_RegDef_t *)SPI2_BASEADDR)
#define SPI3               ((SPI_RegDef_t *)SPI3_BASEADDR)
#define SPI4               ((SPI_RegDef_t *)SPI4_BASEADDR)
#define SPI5               ((SPI_RegDef_t *)SPI5_BASEADDR)

#define USART6              ((USART_RegDef_t *)USART6_BASEADDR)
#define USART1              ((USART_RegDef_t *)USART1_BASEADDR)
#define USART2              ((USART_RegDef_t *)USART2_BASEADDR)














/*
 * clock enable macros for GPIO
 */
#define GPIOA_PCLK_EN()    (RCC->AHB1ENR |= (1 << 0))  /* Bit 0: GPIOA clock enable */
#define GPIOB_PCLK_EN()    (RCC->AHB1ENR |= (1 << 1))  /* Bit 1: GPIOB clock enable */
#define GPIOC_PCLK_EN()    (RCC->AHB1ENR |= (1 << 2))  /* Bit 2: GPIOC clock enable */
#define GPIOD_PCLK_EN()    (RCC->AHB1ENR |= (1 << 3))  /* Bit 3: GPIOD clock enable */
#define GPIOE_PCLK_EN()    (RCC->AHB1ENR |= (1 << 4))  /* Bit 4: GPIOE clock enable */
#define GPIOH_PCLK_EN()    (RCC->AHB1ENR |= (1 << 7))  /* Bit 7: GPIOH clock enable */

/*
 * SYSCGF clock enable macro
 */
#define SYSCFG_PCLK_EN()   (RCC->APB2ENR|=(1<<14))
/*
 * SPI clock enable macros
 */
#define SPI2_PCLK_EN() (RCC->APB1ENR |= (1<<14))
#define SPI3_PCLK_EN() (RCC->APB1ENR |= (1<<15))
#define SPI1_PCLK_EN() (RCC->APB2ENR |= (1<<12))
#define SPI4_PCLK_EN() (RCC->APB2ENR |= (1<<13))
#define SPI5_PCLK_EN() (RCC->APB2ENR |= (1<<20))

#define USART2_PCLK_EN() (RCC->APB1ENR |= (1<<17))
#define USART1_PCLK_EN() (RCC->APB2ENR |= (1<<4))
#define USART6_PCLK_EN() (RCC->APB2ENR |= (1<<5))


/*
 * Clock Disable Macros for GPIO Peripherals
 * Clearing the respective bits in the RCC AHB1ENR register
 */
#define GPIOA_PCLK_DI()    (RCC->AHB1ENR &= ~(1 << 0))
#define GPIOB_PCLK_DI()    (RCC->AHB1ENR &= ~(1 << 1))
#define GPIOC_PCLK_DI()    (RCC->AHB1ENR &= ~(1 << 2))
#define GPIOD_PCLK_DI()    (RCC->AHB1ENR &= ~(1 << 3))
#define GPIOE_PCLK_DI()    (RCC->AHB1ENR &= ~(1 << 4))
#define GPIOH_PCLK_DI()    (RCC->AHB1ENR &= ~(1 << 7))

/*SYSCFG clock disable macro
 *
 */
#define SYSCFG_PCLK_DI()   (RCC->APB2ENR &= ~(1<<14))







/*
 * SPI clock disable macros
 */

#define SPI2_PCLK_DI() (RCC->APB1ENR &= ~(1<<14))
#define SPI3_PCLK_DI() (RCC->APB1ENR &= ~(1<<15))
#define SPI1_PCLK_DI() (RCC->APB2ENR &= ~(1<<12))
#define SPI4_PCLK_DI() (RCC->APB2ENR &= ~(1<<13))
#define SPI5_PCLK_DI() (RCC->APB2ENR &= ~(1<<20))

#define USART2_PCLK_DI() (RCC->APB1ENR &= ~(1<<17))
#define USART1_PCLK_DI() (RCC->APB2ENR &= ~(1<<4))
#define USART6_PCLK_DI() (RCC->APB2ENR &= ~(1<<5))


/*
 *
 *
 * GPIO peripheral reset macros
 * to reset first enable the reset register then disable the reset register back to 0
 */
#define GPIOA_REG_RESET()  do{(RCC->AHB1RSTR |= (1 << 0)); (RCC->AHB1RSTR &= ~(1 << 0));}while(0)
#define GPIOB_REG_RESET()  do{(RCC->AHB1RSTR |= (1 << 1)); (RCC->AHB1RSTR &= ~(1 << 1));}while(0)
#define GPIOC_REG_RESET()  do{(RCC->AHB1RSTR |= (1 << 2)); (RCC->AHB1RSTR &= ~(1 << 2));}while(0)
#define GPIOD_REG_RESET()  do{(RCC->AHB1RSTR |= (1 << 3)); (RCC->AHB1RSTR &= ~(1 << 3));}while(0)
#define GPIOE_REG_RESET()  do{(RCC->AHB1RSTR |= (1 << 4)); (RCC->AHB1RSTR &= ~(1 << 4));}while(0)
#define GPIOH_REG_RESET()  do{(RCC->AHB1RSTR |= (1 << 7)); (RCC->AHB1RSTR &= ~(1 << 7));}while(0)

#define SPI1_REG_RESET()  do{(RCC->APB2RSTR |= (1 << 12)); (RCC->APB2RSTR &= ~(1 << 12));}while(0)
#define SPI2_REG_RESET()  do{(RCC->APB1RSTR |= (1 << 14)); (RCC->APB1RSTR &= ~(1 << 14));}while(0)
#define SPI3_REG_RESET()  do{(RCC->APB1RSTR |= (1 << 15)); (RCC->APB1RSTR &= ~(1 << 15));}while(0)
#define SPI4_REG_RESET()  do{(RCC->APB2RSTR |= (1 <<13)); (RCC->APB2RSTR &= ~(1 << 13));}while(0)
#define SPI5_REG_RESET()  do{(RCC->APB2RSTR |= (1 << 20)); (RCC->APB2RSTR &= ~(1 << 20));}while(0)



#define ENABLE 1
#define DISABLE 0
#define SET   ENABLE
#define RESET DISABLE
#define GPIO_PIN_SET   ENABLE
#define GPIO_PIN_RESET DISABLE
#define FLAG_SET  SET
#define FLAG_RESET RESET

#define GPIO_MODE_IN      0
#define GPIO_MODE_OUT     1
#define GPIO_MODE_ALTFN   2
#define GPIO_MODE_ANALOG  3
#define GPIO_MODE_IT_FT   4
#define GPIO_MODE_IT_RT   5
#define GPIO_MODE_IT_FRT  6

#define GPIO_SPEED_LOW     0
#define GPIO_SPEED_MED     1
#define GPIO_SPEED_FAST    2
#define GPIO_SPEED_HIGH    3

#define GPIO_NOPu_NOPd    0
#define GPIO_PU           1
#define GPIO_PD          2

#define GPIO_OP_TYPE_PP     0
#define GPIO_OP_TYPE_OD     1

#define GPIO_PIN_NO_0      0
#define GPIO_PIN_NO_1      1
#define GPIO_PIN_NO_2      2
#define GPIO_PIN_NO_3      3
#define GPIO_PIN_NO_4      4
#define GPIO_PIN_NO_5      5
#define GPIO_PIN_NO_6      6
#define GPIO_PIN_NO_7      7
#define GPIO_PIN_NO_8      8
#define GPIO_PIN_NO_9      9
#define GPIO_PIN_NO_10     10
#define GPIO_PIN_NO_11     11
#define GPIO_PIN_NO_12     12
#define GPIO_PIN_NO_13     13
#define GPIO_PIN_NO_14     14
#define GPIO_PIN_NO_15     15




/*
 * IRQ (Interrupt Request) Numbers for EXTI Lines
 * Placed in the NVIC position vector table for STM32F411
 */

//GPIO specific IRQ no.
#define IRQ_NO_EXTI0          6
#define IRQ_NO_EXTI1          7
#define IRQ_NO_EXTI2          8
#define IRQ_NO_EXTI3          9
#define IRQ_NO_EXTI4          10
#define IRQ_NO_EXTI9_5        23
#define IRQ_NO_EXTI15_10      40

//SPI irq no.
#define IRQ_NO_SPI1           35
#define IRQ_NO_SPI2           36
#define IRQ_NO_SPI3           51
#define IRQ_NO_SPI4           84
#define IRQ_NO_SPI5           85



#define SPI2_CLCK_EN (RCC->APB1_BASEADDR |= (1<<14))
#define SPI3_CLCK_EN (RCC->APB1_BASEADDR |= (1<<15))
#define SPI1_CLCK_EN (RCC->APB2_BASEADDR |= (1<<12))
#define SPI4_CLCK_EN (RCC->APB2_BASEADDR |= (1<<13))
#define SPI5_CLCK_EN (RCC->APB2_BASEADDR |= (1<<20))





#include "STM32F411E_spi_drivers.h"
#include "STM32F411E_gpio_drivers.h"



#endif /* INC_STM32F411E_H_ */
