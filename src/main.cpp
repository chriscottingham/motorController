/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include "Timer.h"
#include "BlinkLed.h"
#include "LedDisplayDriver.h"
#include "ExtiHandler.h"
// ----------------------------------------------------------------------------
//
// Standalone STM32F1 led blink sample (trace via DEBUG).
//
// In debug configurations, demonstrate how to print a greeting message
// on the trace device. In release configurations the message is
// simply discarded.
//
// Then demonstrates how to blink a led with 1 Hz, using a
// continuous loop and SysTick delays.
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//
// The external clock frequency is specified as a preprocessor definition
// passed to the compiler via a command line option (see the "C/C++ General" ->
// "Paths and Symbols" -> the "Symbols" tab, if you want to change it).
// The value selected during project creation was HSE_VALUE=8000000.
//
// Note: The default clock settings take the user defined HSE_VALUE and try
// to reach the maximum possible system clock. For the default 8 MHz input
// the result is guaranteed, but for other values it might not be possible,
// so please adjust the PLL settings in system/src/cmsis/system_stm32f10x.c
//

// Definitions visible only within this translation unit.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-variable"

//	RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;

LedDisplayDriver displayDriver;

extern "C" void EXTI0_IRQHandler(void) {
	trace_puts("Exti0 triggered");
	EXTI->PR = 1;
	__ISB();
	displayDriver.start();
//	NVIC_ClearPendingIRQ(EXTI0_IRQn);
}

int main(int argc, char* argv[]) {

	trace_puts("Hello ARM World!");

	trace_printf("System clock: %u Hz\n", SystemCoreClock);

//	__enable_irq();

	Timer timer;
	timer.start();

//  RCC_APB2PeriphClockCmd(BLINK_RCC_MASKx(BLINK_PORT_NUMBER) | RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 , ENABLE);

	IoDriver::initPin(GPIOA, std::vector<uint8_t>{8}, GpioMode::altPushPullOutput);
	RCC->CFGR |= 0x4000000;

	uint32_t seconds = 0;

	ExtiHandler buttonHandler;
	buttonHandler.setupTrigger(GPIOA);

	displayDriver.init();

	bool printedI2cStart = false;

	while (1) {

//		if (!printedI2cStart) {
//			if (I2C1->SR1 & 1) {
//				trace_puts("got start condition");
//				printedI2cStart = true;
//			}
//		}
		timer.sleep(1);

//      blinkLed.turnOn();
//      displayDriver.writeParallelPort(0xa5, 1);
//      timer.sleep(seconds == 0 ? Timer::FREQUENCY_HZ : BLINK_ON_TICKS);

//      displayDriver.writeParallelPort(0x5a, 1);
//      blinkLed.turnOff();
//      timer.sleep(BLINK_OFF_TICKS);

//      ++seconds;

		// Count seconds on the trace device.
//      trace_printf("Second %u\n", seconds);
	}
	// Infinite loop, never return.
}




#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
