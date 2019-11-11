/*
 * RotaryEncoder.cpp
 *
 *  Created on: Nov 3, 2019
 *      Author: chris
 */

#include <cstring>
#include "RotaryEncoder.h"

RotaryEncoder::RotaryEncoder(GPIO_TypeDef* timerPort, uint8_t encoderPins[2])
	: timerPort(timerPort) {

	memcpy(this->encoderPins, encoderPins, 2);


}

uint16_t RotaryEncoder::getRpm() {

}
Direction RotaryEncoder::getDirection() {

}
void RotaryEncoder::tick() {

}
