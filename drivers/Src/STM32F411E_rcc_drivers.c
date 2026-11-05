/*
 * STM32F411E_rcc_drivers.c
 *
 *  Created on: Jul 13, 2026
 *      Author: chinmay
 */


#include "STM32F411E_rcc_drivers.h"
//APB1 BUS
uint32_t RCC_GetPCLK1Value(void) {
    uint32_t pclk1, SystemClk;
    uint8_t clksrc, temp, ahbp, apb1p;

    clksrc = ((RCC->CFGR >> 2) & 0x3);  // bits 3:2 = SWS (system clock switch status)

    if (clksrc == 0) {
        SystemClk = 16000000;   // HSI
    } else if (clksrc == 1) {
        SystemClk = 8000000;    // HSE (adjust to your board's crystal)
    } else if (clksrc == 2) {
       // SystemClk = RCC_GetPLLOutputClock();  // if using PLL, you'd need this too
    }

    // AHB prescaler (bits 7:4 of CFGR)
    temp = ((RCC->CFGR >> 4) & 0xF);
    if (temp < 8) {
        ahbp = 1;
    } else {
        uint16_t AHB_PreScaler[8] = {2,4,8,16,64,128,256,512};
        ahbp = AHB_PreScaler[temp - 8];
    }

    // APB1 prescaler (bits 12:10 of CFGR)
    temp = ((RCC->CFGR >> 10) & 0x7);
    if (temp < 4) {
        apb1p = 1;
    } else {
        uint8_t APB1_PreScaler[4] = {2,4,8,16};
        apb1p = APB1_PreScaler[temp - 4];
    }

    pclk1 = (SystemClk / ahbp) / apb1p;

    return pclk1;
}

//APB2 BUS
uint32_t RCC_GetPCLK2Value(void) {
    uint32_t pclk2, SystemClk;
    uint8_t clksrc, temp, ahbp, apb2p;

    clksrc = ((RCC->CFGR >> 2) & 0x3);

    if (clksrc == 0) {
        SystemClk = 16000000;
    } else if (clksrc == 1) {
        SystemClk = 8000000;
    } else if (clksrc == 2) {
       // SystemClk = RCC_GetPLLOutputClock();
    }

    temp = ((RCC->CFGR >> 4) & 0xF);
    if (temp < 8) {
        ahbp = 1;
    } else {
        uint16_t AHB_PreScaler[8] = {2,4,8,16,64,128,256,512};
        ahbp = AHB_PreScaler[temp - 8];
    }

    // APB2 prescaler is bits 15:13
    temp = ((RCC->CFGR >> 13) & 0x7);
    if (temp < 4) {
        apb2p = 1;
    } else {
        uint8_t APB2_PreScaler[4] = {2,4,8,16};
        apb2p = APB2_PreScaler[temp - 4];
    }

    pclk2 = (SystemClk / ahbp) / apb2p;

    return pclk2;
}
