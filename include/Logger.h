/*
 * Logger.h
 *
 *  Created on: Oct 31, 2019
 *      Author: chris
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>
#include "diag/Trace.h"
#include "string.h"

class Logger {
public:
	static void write(char* message);
	static void write(uint32_t registerValue);
	static void writeln(char* message);
	static void writeln(uint32_t registerValue);
};

#endif /* LOGGER_H_ */
