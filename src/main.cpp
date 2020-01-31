#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic push

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <MotorDisplay.h>
#include "SpeedInput.h"
#include "ExtiHandler.h"
#include "RotaryEncoder.h"
#include "PwmControl.h"
#include "AdcController.h"
#include "System.hpp"

#define configASSERT_DEFINED 1
#define MAX_MOTOR_RPM 3600

//	__enable_irq();

	//Clock output
//	IoDriver::initPin(GPIOA, std::vector<uint8_t>{8}, GpioMode::altPushPullOutput);
//	RCC->CFGR |= 0x4000000;

//	ExtiHandler buttonHandler(GPIOB, std::vector<uint8_t> {10,11});
//	buttonHandler.setupTrigger(GPIOA);

MotorDisplay motorDisplay;
RotaryEncoder encoder(GPIOA, {0,1});
AdcController adcController(GPIOA, {2,3,4});
PwmControl pwm(GPIOA, 6);

int main(int argc, char* argv[]) {

	trace_puts("Hello ARM World!");

	trace_printf("System clock: %u Hz\n", SystemCoreClock);

	SysTick_Config(SystemCoreClock / 1000);

	SpeedInput speedInput(0);
	speedInput.setMaxRpm(3600);
	speedInput.setAdcController(&adcController);

	motorDisplay.setSpeedInput(&speedInput);
	motorDisplay.setRotaryEncoder(&encoder);
	motorDisplay.setAdcController(&adcController);

	pwm.setSpeedInput(&speedInput);
	pwm.setAdcController(&adcController);
	pwm.setMaxMotorRpm(3600);
	pwm.setAdcCurrentIndex(1);
	pwm.setAdcVoltageIndex(2);

	adcController.startAdc();

	while (1) {
		System::getInstance().offerRun();
	}
}

extern "C" {
	void SysTick_Handler(void)  {                               /* SysTick interrupt Handler. */
		System::getInstance().tick();
	}

	void EXTI0_IRQHandler(void) {

		//	NVIC_ClearPendingIRQ(EXTI0_IRQn);

		EXTI->PR = 1;
		__ISB();

	}

	void I2C1_EV_IRQHandler(void) {
		motorDisplay.handleI2cInterrupt();
	}

	void I2C1_ER_IRQHandler(void) {
		motorDisplay.handleI2cError();
	}

	void DMA1_Channel6_IRQHandler(void) {
		motorDisplay.handleDma();
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
