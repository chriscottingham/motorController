	/*
	 * DisplayDriver.h
 *
 *  Created on: Oct 8, 2019
 *      Author: chris
 */

#ifndef DISPLAYDRIVER_H_
#define DISPLAYDRIVER_H_

#include <inttypes.h>
#include <vector>

#include "stm32f10x.h"
#include "IoDriver.h"
#include "GpioMode.h"

class IliDisplayDriver {

	public:
		static const uint8_t RESET_COMMAND = 0x01;
		static const uint8_t SLEEP_OUT_COMMAND = 0x11;
		static const uint8_t DISPLAY_ON_COMMAND = 0x29;

		static const uint8_t RESET_PIN = 10;
		static const uint8_t READ_PIN = 11;
		static const uint8_t WRITE_PIN = 12;
		static const uint8_t REGISTER_SELECT_PIN = 13;
		static const uint8_t CHIP_SELECT_PIN = 14;

		static const std::vector<uint8_t> PARALLEL_PINS;
		static const std::vector<uint8_t> CONTROL_PINS;

		void initPins();

		void setupDisplay();
		void setupSecond();

		void writeParallelPort(uint8_t value, uint8_t registerSelect);

		void setFunctionPin(uint8_t pin_number, uint8_t value);

	protected:
		void setPortValue(uint8_t value);
};



#endif /* DISPLAYDRIVER_H_ */
