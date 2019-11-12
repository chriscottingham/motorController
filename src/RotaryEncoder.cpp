/*
 * RotaryEncoder.cpp
 *
 *  Created on: Nov 3, 2019
 *      Author: chris
 */

#include <cstring>
#include "RotaryEncoder.h"

#include "task.h"

RotaryEncoder::RotaryEncoder(GPIO_TypeDef* timerPort, vector<uint8_t>* const encoderPins)
	: timerPort(timerPort) {

	this->encoderPins = *encoderPins;

	IoDriver::initPin(timerPort, this->encoderPins, GpioMode::floatingInput);

	RCC->APB1ENR |= 1;

	TIM2->SMCR |= 1; //SMS[2:0]
	TIM2->CR1 |= TIM_CR1_CEN;
}

uint16_t RotaryEncoder::getRpm() {

}

Direction RotaryEncoder::getDirection() {

}
void RotaryEncoder::tick() {

	vTaskDelay(pdMS_TO_TICKS(100));
}

void RotaryEncoderTask(void* param) {
	((RotaryEncoder*) param)->tick();
}
