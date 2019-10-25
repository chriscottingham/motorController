/*
 * GpioMode.cpp
 *
 *  Created on: Oct 24, 2019
 *      Author: chris
 */

#include "GpioMode.h"

bool GpioMode::isAlternate() {
	return val == GpioMode::type::altOpenDrainOutput
			|| val == GpioMode::type::altPushPullOutput;
}
