/*
 * Logger.cpp
 *
 *  Created on: Oct 31, 2019
 *      Author: chris
 */

#include "Logger.h"

void Logger::log(char* message, uint32_t registerValue) {

	trace_puts(message);
	trace_puts(buffer);
}

static void Logger::write(char* message) {
	trace_write(s, strlen(s));

}
static void Logger::write(uint32_t registerValue) {
	sprintf(buffer, "0x%08X", registerValue);
	trace_write(buffer, 10);
}

static void Logger::writeln(char* message) {

	return trace_write("\n", 1);
}
static void Logger::writeln(uint32_t registerValue) {
	write(registerValue);
	trace_write("\n", 1);
}
