/*
 * Logger.cpp
 *
 *  Created on: Oct 31, 2019
 *      Author: chris
 */

#include "Logger.h"

void Logger::write(char* message) {
	trace_write(message, strlen(message));
}

void Logger::write(uint32_t registerValue) {

	static char buffer[10];
	sprintf(buffer, "0x%08X", registerValue);
	trace_write(buffer, 10);
}

void Logger::writeln(char* message) {
	trace_write(message, strlen(message));
	trace_write("\n", 1);
}

void Logger::writeln(uint32_t registerValue) {
	write(registerValue);
	trace_write("\n", 1);
}
