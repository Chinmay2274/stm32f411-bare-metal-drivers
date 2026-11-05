/*
 * delay.h
 *
 *  Created on: Aug 7, 2026
 *      Author: chinmay
 */

#ifndef INC_DELAY_H_
#define INC_DELAY_H_

#include <stdint.h>
#include "STM32F411E.h"

void dwt_delay(uint8_t us);
void dwt_reset(void);
void dwt_init(void);



#endif /* INC_DELAY_H_ */
