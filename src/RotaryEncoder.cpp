/*
 * RotaryEncoder.cpp
 *
 *  Created on: Nov 3, 2019
 *      Author: chris
 */

#include <cstring>

#include "RotaryEncoder.h"
#include "System.hpp"


RotaryEncoder::RotaryEncoder(GPIO_TypeDef* timerPort, const vector<uint8_t>& encoderPins) :
		timerPort(timerPort) {

	this->encoderPins = encoderPins;

	IoDriver::initPin(timerPort, this->encoderPins, GpioMode::floatingInput);

	RCC->APB1ENR |= 1;

	TIM2->SMCR |= 0x0300 | 2; //SMS[2:0]
	TIM2->CR1 |= TIM_CR1_CEN;
}

int RotaryEncoder::getSpeed() {

	constexpr int multiplier = 60 * 1000 / 200; //60 (seconds per minute) * 1000 (ms in second) / 200 (ticks per rotation)

	static int previousSpeed = 0;

	int rpm = previousSpeed;

	uint16_t currentCount = TIM2->CNT;
	int diffEncoder = currentCount - previousEncoderCount;
	if (!(TIM2->CR1 & TIM_CR1_DIR) && diffEncoder < 0) {
		diffEncoder += 65535;
	}
	System& system = System::getInstance();
	uint32_t diffTicks = system.getDiffTicks(previousSysTick);
	uint32_t currentTicks = system.getSysTick();

	if (diffEncoder > 50 || diffTicks > 20) {
		rpm = ((float) diffEncoder) / diffTicks * multiplier;
		previousSpeed = rpm;

		previousSysTick = currentTicks;
		previousEncoderCount = currentCount;
	}

	return rpm;
}

Direction RotaryEncoder::getDirection() {

}
