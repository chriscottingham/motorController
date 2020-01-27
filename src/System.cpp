/*
 * System.cpp
 *
 *  Created on: Jan 23, 2020
 *      Author: chris
 */

#include "System.hpp"

System::System() {}

void System::tick() {
	System::sysTick++;
}

uint32_t System::getSysTick() {
	return System::sysTick;
}

uint32_t System::getDiffTicks(uint32_t lastTick) {

	static uint32_t max = 0 - 1;

	int diff;
	if (sysTick > lastTick) {
		diff = sysTick - lastTick;
	} else {
		diff = max - lastTick + sysTick;
	}
	return diff;
}

