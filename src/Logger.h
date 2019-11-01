/*
 * Logger.h
 *
 *  Created on: Oct 31, 2019
 *      Author: chris
 */

#ifndef LOGGER_H_
#define LOGGER_H_

class Logger {
public:
	static char buffer[10];

	static void write(char* message);
	static void write(uint32_t registerValue);
	static void writeln(char* message);
	static void writeln(uint32_t registerValue);
};

#endif /* LOGGER_H_ */
