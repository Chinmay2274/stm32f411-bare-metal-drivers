/*
 * delay.c
 *
 *  Created on: Aug 7, 2026
 *      Author: chinmay
 */

#include "delay.h"

 void dwt_init(void) {
    *(DEMCR_BASEADDR)      |= (1 << 24);// enable the trace block (Set bit 24 in DEMCR)
    *(DWT_CYCCNT_BASEADDR)  = 0; // SET CLCK COUNTER TO 0
    *(DWT_CTRL_BASEADDR)   |= (1 << 0); //start the cycle counter ticking (Set bit 0 in DWT_CTRL)
}


void dwt_reset(void){
	 *(DEMCR_BASEADDR)      |= (1 << 24);// enable the trace block (Set bit 24 in DEMCR)
	 *(DWT_CYCCNT_BASEADDR)  = 0; // SET CLCK COUNTER TO 0
}

void dwt_delay(uint8_t us){

	uint32_t start =*(DWT_CYCCNT_BASEADDR) ;
	uint32_t end = us * 16u; // 16 clck cycles per microsecond for 16mhz system clck
	while((*(DWT_CYCCNT_BASEADDR)-start)<end);
}
