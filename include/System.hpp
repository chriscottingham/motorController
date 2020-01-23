/*
 * System.hpp
 *
 *  Created on: Jan 23, 2020
 *      Author: chris
 */

#ifndef SYSTEM_HPP_
#define SYSTEM_HPP_

class System {
private:
	static int sysTick;

public:
	static void incrementSysTick();
	static int getSysTick();
};

#endif /* SYSTEM_HPP_ */
