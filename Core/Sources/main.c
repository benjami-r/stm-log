#include "stm32f072xb.h" //включит "core_cm0.h"
#include "m_periphery_GPIO.h"
#include "m_core_STK.h"

void SysTick_Handler() {
	static uint32_t f = 0;
	if (f) {
		m_GPIOxpin_Down(A, 5);
		f=0;
	} else {
		m_GPIOxpin_Up(A, 5);
		f=1;
	}
}

int main() {
	m_GPIOx_Enable(A);
	m_GPIOxpin_Set(A, 5, mee_GPIO_MODER_OUTPUT, mee_GPIO_OTYPER_PUSHPULL, mee_GPIO_OSPEEDR_LOW, mee_GPIO_PUPDR_NOPUPD);
	//m_GPIOxpin_Up(A,5);

	uint32_t f = m_STK_Get_CSR;

	m_STK_Set_RVR_RELOAD(5000000);
	m_STK_Down_CVR_CURRENT;
	m_STK_Set_CSR(0,1,1);


	return 0;
}



