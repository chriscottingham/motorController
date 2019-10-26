/*
 * IliDisplayDriver.h
 *
 *  Created on: Oct 8, 2019
 *      Author: chris
 */

#include "IliDisplayDriver.h"

#define kWaitTime 120u


const std::vector<uint8_t> IliDisplayDriver::PARALLEL_PINS = { 0, 1, 2, 5, 6, 7, 8,
		9 };

const std::vector<uint8_t> IliDisplayDriver::CONTROL_PINS = {
		IliDisplayDriver::RESET_PIN, IliDisplayDriver::READ_PIN,
		IliDisplayDriver::WRITE_PIN, IliDisplayDriver::REGISTER_SELECT_PIN,
		IliDisplayDriver::CHIP_SELECT_PIN };

enum REGISTER_SELECT_STATE {
	REGISTER_SELECT_COMMAND, REGISTER_SELECT_DATA
};

enum CHIP_SELECT_STATE {
	CHIP_SELECT_ENABLED, CHIP_SELECT_DISABLED
};

void IliDisplayDriver::initPins() {

	IoDriver::initPin(GPIOB, PARALLEL_PINS, GpioMode::pushPullOutput);
	IoDriver::initPin(GPIOB, CONTROL_PINS, GpioMode::pushPullOutput);

	timer.start();
}

void IliDisplayDriver::setFunctionPin(uint8_t pin, uint8_t value) {

	uint8_t offset = 16 * !value;
	uint32_t mask = 1 << (pin + offset);
	GPIOB->BSRR = mask;
}

void IliDisplayDriver::writeParallelPort(uint8_t value, uint8_t registerSelect) {

	setFunctionPin(READ_PIN, 1);

	setFunctionPin(CHIP_SELECT_PIN, CHIP_SELECT_ENABLED);
	setFunctionPin(REGISTER_SELECT_PIN, registerSelect);

	setFunctionPin(WRITE_PIN, 0);
	setPortValue(value);
	timer.sleep(10u);
	setFunctionPin(WRITE_PIN, 1);
	timer.sleep(10u);

	setFunctionPin(CHIP_SELECT_PIN, CHIP_SELECT_DISABLED);
}

void IliDisplayDriver::setPortValue(uint8_t value) {

	uint32_t bsrr_mask = 0;

	uint8_t shift = 0;
	for (uint16_t i = 0; i < 8; i++) {
		shift = 0x10 * !(value & (1 << i));
		bsrr_mask |= 1 << (shift + PARALLEL_PINS[i]);
	}
	GPIOB->BSRR = bsrr_mask;
}

void IliDisplayDriver::setupDisplay(void) {

	setFunctionPin(IliDisplayDriver::READ_PIN, 1);

	setFunctionPin(IliDisplayDriver::RESET_PIN, 0);
	timer.sleep(kWaitTime);
	setFunctionPin(IliDisplayDriver::RESET_PIN, 1);
	timer.sleep(kWaitTime);

	setFunctionPin(IliDisplayDriver::RESET_PIN, 0);
	timer.sleep(kWaitTime);
	setFunctionPin(IliDisplayDriver::RESET_PIN, 1);
	timer.sleep(kWaitTime);

	writeParallelPort(0x1, REGISTER_SELECT_COMMAND);
	timer.sleep(kWaitTime);

	writeParallelPort(0x11, REGISTER_SELECT_COMMAND);
	timer.sleep(kWaitTime);

	writeParallelPort(0xCF, REGISTER_SELECT_COMMAND);
	writeParallelPort(0x00, REGISTER_SELECT_DATA);
	writeParallelPort(0xc3, REGISTER_SELECT_DATA);
	writeParallelPort(0X30, REGISTER_SELECT_DATA);

	writeParallelPort(0xED, REGISTER_SELECT_COMMAND);
	writeParallelPort(0x64, REGISTER_SELECT_DATA);
	writeParallelPort(0x03, REGISTER_SELECT_DATA);
	writeParallelPort(0X12, REGISTER_SELECT_DATA);
	writeParallelPort(0X81, REGISTER_SELECT_DATA);

	writeParallelPort(0xE8, REGISTER_SELECT_COMMAND);
	writeParallelPort(0x85, REGISTER_SELECT_DATA);
	writeParallelPort(0x10, REGISTER_SELECT_DATA);
	writeParallelPort(0x79, REGISTER_SELECT_DATA);

	writeParallelPort(0xCB, REGISTER_SELECT_COMMAND);
	writeParallelPort(0x39, REGISTER_SELECT_DATA);
	writeParallelPort(0x2C, REGISTER_SELECT_DATA);
	writeParallelPort(0x00, REGISTER_SELECT_DATA);
	writeParallelPort(0x34, REGISTER_SELECT_DATA);
	writeParallelPort(0x02, REGISTER_SELECT_DATA);

	writeParallelPort(0xF7, REGISTER_SELECT_COMMAND);
	writeParallelPort(0x20, REGISTER_SELECT_DATA);

	writeParallelPort(0xEA, REGISTER_SELECT_COMMAND);
	writeParallelPort(0x00, REGISTER_SELECT_DATA);
	writeParallelPort(0x00, REGISTER_SELECT_DATA);

	writeParallelPort(0xC0, REGISTER_SELECT_COMMAND);    //Power control
	writeParallelPort(0x22, REGISTER_SELECT_DATA);   //VRH[5:0]

	writeParallelPort(0xC1, REGISTER_SELECT_COMMAND);    //Power control
	writeParallelPort(0x11, REGISTER_SELECT_DATA);   //SAP[2:0];BT[3:0]

	writeParallelPort(0xC5, REGISTER_SELECT_COMMAND);    //VCM control
	writeParallelPort(0x3d, REGISTER_SELECT_DATA);
	//LCD_DataWrite_ILI9341(0x30);
	writeParallelPort(0x20, REGISTER_SELECT_DATA);

	writeParallelPort(0xC7, REGISTER_SELECT_COMMAND);    //VCM control2
	//LCD_DataWrite_ILI9341(0xBD);
	writeParallelPort(0xA0, REGISTER_SELECT_DATA); //0xB0

	writeParallelPort(0x36, REGISTER_SELECT_COMMAND);   // Memory Access Control
	writeParallelPort(0x08, REGISTER_SELECT_DATA);

	writeParallelPort(0x3A, REGISTER_SELECT_COMMAND);
	writeParallelPort(0x55, REGISTER_SELECT_DATA);

	writeParallelPort(0xB1, REGISTER_SELECT_COMMAND);
	writeParallelPort(0x00, REGISTER_SELECT_DATA);
	writeParallelPort(0x13, REGISTER_SELECT_DATA);

	writeParallelPort(0xB6, REGISTER_SELECT_COMMAND); // Display Function Control
	writeParallelPort(0x0A, REGISTER_SELECT_DATA);
	writeParallelPort(0xA2, REGISTER_SELECT_DATA);

	writeParallelPort(0xF6, REGISTER_SELECT_COMMAND);
	writeParallelPort(0x01, REGISTER_SELECT_DATA);
	writeParallelPort(0x30, REGISTER_SELECT_DATA);

	writeParallelPort(0xF2, REGISTER_SELECT_COMMAND); // 3Gamma Function Disable
	writeParallelPort(0x00, REGISTER_SELECT_DATA);

	writeParallelPort(0x26, REGISTER_SELECT_COMMAND);    //Gamma curve selected
	writeParallelPort(0x01, REGISTER_SELECT_DATA);

	writeParallelPort(0xE0, REGISTER_SELECT_COMMAND);    //Set Gamma
	writeParallelPort(0x0F, REGISTER_SELECT_DATA);
	writeParallelPort(0x3F, REGISTER_SELECT_DATA);
	writeParallelPort(0x2F, REGISTER_SELECT_DATA);
	writeParallelPort(0x0C, REGISTER_SELECT_DATA);
	writeParallelPort(0x10, REGISTER_SELECT_DATA);
	writeParallelPort(0x0A, REGISTER_SELECT_DATA);
	writeParallelPort(0x53, REGISTER_SELECT_DATA);
	writeParallelPort(0XD5, REGISTER_SELECT_DATA);
	writeParallelPort(0x40, REGISTER_SELECT_DATA);
	writeParallelPort(0x0A, REGISTER_SELECT_DATA);
	writeParallelPort(0x13, REGISTER_SELECT_DATA);
	writeParallelPort(0x03, REGISTER_SELECT_DATA);
	writeParallelPort(0x08, REGISTER_SELECT_DATA);
	writeParallelPort(0x03, REGISTER_SELECT_DATA);
	writeParallelPort(0x00, REGISTER_SELECT_DATA);

	writeParallelPort(0XE1, REGISTER_SELECT_COMMAND);    //Set Gamma
	writeParallelPort(0x00, REGISTER_SELECT_DATA);
	writeParallelPort(0x00, REGISTER_SELECT_DATA);
	writeParallelPort(0x10, REGISTER_SELECT_DATA);
	writeParallelPort(0x03, REGISTER_SELECT_DATA);
	writeParallelPort(0x0F, REGISTER_SELECT_DATA);
	writeParallelPort(0x05, REGISTER_SELECT_DATA);
	writeParallelPort(0x2C, REGISTER_SELECT_DATA);
	writeParallelPort(0xA2, REGISTER_SELECT_DATA);
	writeParallelPort(0x3F, REGISTER_SELECT_DATA);
	writeParallelPort(0x05, REGISTER_SELECT_DATA);
	writeParallelPort(0x0E, REGISTER_SELECT_DATA);
	writeParallelPort(0x0C, REGISTER_SELECT_DATA);
	writeParallelPort(0x37, REGISTER_SELECT_DATA);
	writeParallelPort(0x3C, REGISTER_SELECT_DATA);
	writeParallelPort(0x0F, REGISTER_SELECT_DATA);

	writeParallelPort(0x11, REGISTER_SELECT_COMMAND);    //Exit Sleep
	timer.sleep(120);
	writeParallelPort(0x29, REGISTER_SELECT_COMMAND);    //Display on
	timer.sleep(50);

}

void IliDisplayDriver::setupSecond() {
	writeParallelPort(0xCF, REGISTER_SELECT_COMMAND);
	writeParallelPort(0x00, REGISTER_SELECT_DATA);
	writeParallelPort(0x83, REGISTER_SELECT_DATA);
	writeParallelPort(0X30, REGISTER_SELECT_DATA);

	writeParallelPort(0xED, REGISTER_SELECT_COMMAND);
	writeParallelPort(0x64, REGISTER_SELECT_DATA);
	writeParallelPort(0x03, REGISTER_SELECT_DATA);
	writeParallelPort(0X12, REGISTER_SELECT_DATA);
	writeParallelPort(0X81, REGISTER_SELECT_DATA);

	writeParallelPort(0xE8, REGISTER_SELECT_COMMAND);
	writeParallelPort(0x85, REGISTER_SELECT_DATA);
	writeParallelPort(0x00, REGISTER_SELECT_DATA);
	writeParallelPort(0x78, REGISTER_SELECT_DATA);

	writeParallelPort(0xCB, REGISTER_SELECT_COMMAND);
	writeParallelPort(0x39, REGISTER_SELECT_DATA);
	writeParallelPort(0x2C, REGISTER_SELECT_DATA);
	writeParallelPort(0x00, REGISTER_SELECT_DATA);
	writeParallelPort(0x34, REGISTER_SELECT_DATA);
	writeParallelPort(0x02, REGISTER_SELECT_DATA);

	writeParallelPort(0xF7, REGISTER_SELECT_COMMAND);
	writeParallelPort(0x20, REGISTER_SELECT_DATA);

	writeParallelPort(0xEA, REGISTER_SELECT_COMMAND);
	writeParallelPort(0x00, REGISTER_SELECT_DATA);
	writeParallelPort(0x00, REGISTER_SELECT_DATA);

	writeParallelPort(0xC0, REGISTER_SELECT_COMMAND);    //Power control
	writeParallelPort(0x19, REGISTER_SELECT_DATA);   //VRH[5:0]

	writeParallelPort(0xC1, REGISTER_SELECT_COMMAND);    //Power control
	writeParallelPort(0x11, REGISTER_SELECT_DATA);   //SAP[2:0];BT[3:0]

	writeParallelPort(0xC5, REGISTER_SELECT_COMMAND);    //VCM control
	writeParallelPort(0x3C, REGISTER_SELECT_DATA);
	writeParallelPort(0x3F, REGISTER_SELECT_DATA);

	writeParallelPort(0xC7, REGISTER_SELECT_COMMAND);    //VCM control2
	writeParallelPort(0X90, REGISTER_SELECT_DATA);

	writeParallelPort(0x36, REGISTER_SELECT_COMMAND);    // Memory Access Control
	writeParallelPort(0x28, REGISTER_SELECT_DATA);

	writeParallelPort(0x3A, REGISTER_SELECT_COMMAND);
	writeParallelPort(0x55, REGISTER_SELECT_DATA);   // 16Bit

	writeParallelPort(0xB1, REGISTER_SELECT_COMMAND);
	writeParallelPort(0x00, REGISTER_SELECT_DATA);
	writeParallelPort(0x17, REGISTER_SELECT_DATA);

	writeParallelPort(0xB6, REGISTER_SELECT_COMMAND);    // Display Function Control
	writeParallelPort(0x0A, REGISTER_SELECT_DATA);
	writeParallelPort(0xA2, REGISTER_SELECT_DATA);

	writeParallelPort(0xF6, REGISTER_SELECT_COMMAND);
	writeParallelPort(0x01, REGISTER_SELECT_DATA);
	writeParallelPort(0x30, REGISTER_SELECT_DATA);

	writeParallelPort(0xF2, REGISTER_SELECT_COMMAND);    // 3Gamma Function Disable
	writeParallelPort(0x00, REGISTER_SELECT_DATA);

	writeParallelPort(0x26, REGISTER_SELECT_COMMAND);    //Gamma curve selected
	writeParallelPort(0x01, REGISTER_SELECT_DATA);

	writeParallelPort(0xE0, REGISTER_SELECT_COMMAND);    //Set Gamma
	writeParallelPort(0x0F, REGISTER_SELECT_DATA);
	writeParallelPort(0x26, REGISTER_SELECT_DATA);
	writeParallelPort(0x22, REGISTER_SELECT_DATA);
	writeParallelPort(0x0A, REGISTER_SELECT_DATA);
	writeParallelPort(0x10, REGISTER_SELECT_DATA);
	writeParallelPort(0x0A, REGISTER_SELECT_DATA);
	writeParallelPort(0x4C, REGISTER_SELECT_DATA);
	writeParallelPort(0XCA, REGISTER_SELECT_DATA);
	writeParallelPort(0x36, REGISTER_SELECT_DATA);
	writeParallelPort(0x00, REGISTER_SELECT_DATA);
	writeParallelPort(0x15, REGISTER_SELECT_DATA);
	writeParallelPort(0x00, REGISTER_SELECT_DATA);
	writeParallelPort(0x10, REGISTER_SELECT_DATA);
	writeParallelPort(0x10, REGISTER_SELECT_DATA);
	writeParallelPort(0x00, REGISTER_SELECT_DATA);

	writeParallelPort(0XE1, REGISTER_SELECT_COMMAND);    //Set Gamma
	writeParallelPort(0x00, REGISTER_SELECT_DATA);
	writeParallelPort(0x19, REGISTER_SELECT_DATA);
	writeParallelPort(0x1B, REGISTER_SELECT_DATA);
	writeParallelPort(0x05, REGISTER_SELECT_DATA);
	writeParallelPort(0x0F, REGISTER_SELECT_DATA);
	writeParallelPort(0x05, REGISTER_SELECT_DATA);
	writeParallelPort(0x33, REGISTER_SELECT_DATA);
	writeParallelPort(0x35, REGISTER_SELECT_DATA);
	writeParallelPort(0x49, REGISTER_SELECT_DATA);
	writeParallelPort(0x0F, REGISTER_SELECT_DATA);
	writeParallelPort(0x1F, REGISTER_SELECT_DATA);
	writeParallelPort(0x0F, REGISTER_SELECT_DATA);
	writeParallelPort(0x3F, REGISTER_SELECT_DATA);
	writeParallelPort(0x3F, REGISTER_SELECT_DATA);
	writeParallelPort(0x0F, REGISTER_SELECT_DATA);

	writeParallelPort(0x11, REGISTER_SELECT_COMMAND);    //Exit Sleep
	timer.sleep(120);

	writeParallelPort(0x29, REGISTER_SELECT_COMMAND);    //Display on
}
