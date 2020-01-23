/*
 * RotaryEncoder.cpp
 *
 *  Created on: Nov 3, 2019
 *      Author: chris
 */

#include <cstring>

#include "RotaryEncoder.h"
#include "System.hpp"

RotaryEncoder::RotaryEncoder(GPIO_TypeDef* timerPort, vector<uint8_t>* const encoderPins) :
		timerPort(timerPort) {

	this->encoderPins = *encoderPins;

	IoDriver::initPin(timerPort, this->encoderPins, GpioMode::floatingInput);

	RCC->APB1ENR |= 1;

	TIM2->SMCR |= 0x0300 | 2; //SMS[2:0]
	TIM2->CR1 |= TIM_CR1_CEN;
}

int RotaryEncoder::getSpeed() {

	int rpm = 3600;

	uint16_t currentCount = TIM2->CNT;
	int diffEncoder = currentCount - previousEncoderCount;
	if (!(TIM2->CR1 & TIM_CR1_DIR) && diffEncoder < 0) {
		diffEncoder += 65535;
	}

	int currentTicks = System::getSysTick();
	int diffTicks = currentTicks - previousSysTick;
	if (diffTicks < 0) {
		diffTicks += 2147483647;
	}

	if (diffEncoder > 50 || diffTicks > 20) {
		int multiplier = 60 * 1000 / 200; //60 (seconds per minute) * 1000 (ms in second) / 200 (ticks per rotation)
		rpm = diffEncoder / diffTicks * multiplier;

		previousSysTick = currentTicks;
		previousEncoderCount = currentCount;
	}

	return rpm;
}

Direction RotaryEncoder::getDirection() {

}
