/*
 * ExtiHandler.cpp
 *
 *  Created on: Oct 20, 2019
 *      Author: chris
 */

#include "ExtiHandler.h"

#include <string.h>

ExtiHandler::ExtiHandler(GPIO_TypeDef* gpio, std::vector<uint8_t>* const pins) :
		gpio(gpio) {

	this->pins = *pins;

	IoDriver::initPin(gpio, this->pins, GpioMode::pushPullOutput);
//	GPIOB->ODR |= 0xc00;
}

void ExtiHandler::setupTrigger(GPIO_TypeDef* gpio) {

	IoDriver::initPin(GPIOA, std::vector<uint8_t> { 0 }, GpioMode::pushPullInput);

	gpio->ODR &= ~1ul;
//	gpio->ODR |= 1;

	NVIC_ClearPendingIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI0_IRQn);

	EXTI->RTSR |= 1;
	EXTI->IMR |= 1;

}
