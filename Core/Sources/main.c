#include "stm32f072xb.h" //включает "core_cm0.h"
#include "mPeriphMCU_GPIO.h"
#include "mPeriphCore_STK.h"

//extern "C" {
	void SysTick_Handler() {
		static uint32_t bLedOn = 0;
		if (bLedOn == 0) {
			m_GPIOxpin_Up(A, 5);
			bLedOn = 1;
		} else {
			m_GPIOxpin_Down(A, 5);
			bLedOn = 0;
		}
	}
//}

int main() {
	m_GPIOx_Enable(A);
	m_GPIOxpin_Set(A, 5, mee_GPIO_MODER_OUTPUT, mee_GPIO_OTYPER_PUSHPULL, mee_GPIO_OSPEEDR_LOW, mee_GPIO_PUPDR_NOPUPD);
	//m_GPIOxpin_Up(A, 5);


	//uint32_t f = m_STK_Get_CSR;

	m_STK_Set_RVR_RELOAD(5000000);
	m_STK_Down_CVR_CURRENT;
	//m_STK_Set_CSR(1,1,1);
	m_STK_Set_CSR(mee_STK_CSR_CLKSOURCE_FAST, mee_STK_CSR_TICKINT_YES, mee_STK_CSR_ENABLE_YES);


	return 0;
}



