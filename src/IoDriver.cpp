#include "IoDriver.h"

void IoDriver::initOutput(GPIO_TypeDef* port, std::vector<uint8_t> const &pins,  GpioMode&& mode) {

	if (port == GPIOA) {
		RCC->APB2ENR |= RCC_APB2Periph_GPIOA;
	} else if (port == GPIOB) {
		RCC->APB2ENR |= RCC_APB2Periph_GPIOB;
	}
	if (mode.isAlternate()) {
		RCC->APB2ENR |= RCC_APB2Periph_AFIO;
	}

	uint32_t crl_one_mask = 0;
	uint32_t crl_zero_mask = 0xffffffff;

	uint32_t crh_one_mask = 0;
	uint32_t crh_zero_mask = 0xffffffff;

	for (uint8_t i = 0; i < sizeof(pins); i++) {

		if (pins[i] < 8) {
			crl_one_mask |= mode.getValue() << (4 * pins[i]);
			crl_zero_mask &= ~((~mode.getValue() & 0x0f) << 4 * pins[i]);
		} else {
			crh_one_mask |= mode.getValue() << (4 * (pins[i] - 8));
			crh_zero_mask &= ~((~mode.getValue() & 0x0f) << 4 * (pins[i] - 8));
		}
	}
	port->CRL |= crl_one_mask;
	port->CRL &= crl_zero_mask;

	port->CRH |= crh_one_mask;
	port->CRH &= crh_zero_mask;
}
