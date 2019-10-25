/*
 * ExtiHandler.cpp
 *
 *  Created on: Oct 20, 2019
 *      Author: chris
 */

#include "ExtiHandler.h"

void ExtiHandler::setupTrigger(GPIO_TypeDef* gpio) {

	IoDriver::initOutput(GPIOA, std::vector<uint8_t>(0), GpioMode::pushPullInput);

	gpio->ODR &= ~1ul;

	NVIC_ClearPendingIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI0_IRQn);

	EXTI->RTSR |= 1;
	EXTI->IMR |= 1;

}
