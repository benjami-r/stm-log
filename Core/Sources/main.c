#include "stm32f072xb.h" //включит "core_cm0.h"
#include "m_periphery_GPIO.h"
#include "m_core_STK.h"

void SysTick_Handler() {
	static uint32_t f = 0;
	if (f) {
		m_GPIOxpin_RESET(A, 5);
		f=0;
	} else {
		m_GPIOxpin_SET(A, 5);
		f=1;
	}
}

int main() {
	m_GPIOx_ENABLE(A);
	m_GPIOxpin_CONFIG(A, 5, mee_GPIO_MODER_OUTPUT, mee_GPIO_OTYPER_PUSHPULL, mee_GPIO_OSPEEDR_LOW, mee_GPIO_PUPDR_NOPUPD);
	//m_GPIOxpin_SET(A, 5);

	m_STK_RVR_RELOAD_SET(5000000);
	m_STK_CVR_CURRENT_CLEAR;
	m_STK_CSR_SET(0,1,1);


	return 0;
}



