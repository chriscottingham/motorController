/*
 * System.cpp
 *
 *  Created on: Jan 23, 2020
 *      Author: chris
 */

#include "System.hpp"

int System::sysTick = 0;

void System::incrementSysTick() {
	System::sysTick++;
}

int System::getSysTick() {
	return System::sysTick;
}

