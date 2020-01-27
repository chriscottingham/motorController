/*
 * System.hpp
 *
 *  Created on: Jan 23, 2020
 *      Author: chris
 */

#ifndef SYSTEM_HPP_
#define SYSTEM_HPP_

#include <stdint.h>
#include <vector>

#include "Timer.hpp"

using namespace std;

class System {
private:
	System();
	System(System const&);
	void operator=(System const&);

	uint32_t sysTick;
	vector<Timer> timers = vector<Timer>();

public:

	static System& getInstance() {
		static System system;
		return system;
	}
	void tick();
	uint32_t getSysTick();
	uint32_t getDiffTicks(uint32_t lastTick);
};

#endif /* SYSTEM_HPP_ */
