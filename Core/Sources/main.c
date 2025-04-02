//main.c
#include "mPeriphCore_STK.h"
#include "mPeriphMCU_GPIO.h"

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


// int fc(){return 0;}
// int fcpp();

int main(void){
  m_GPIOx_Enable(A);
  m_GPIOxpin_Set(A, 5, mee_GPIO_MODER_OUTPUT, mee_GPIO_OTYPER_PUSHPULL, mee_GPIO_OSPEEDR_LOW, mee_GPIO_PUPDR_NOPUPD);
  //m_GPIOxpin_Up(A, 5);


  //uint32_t f = m_STK_Get_CSR;

  //if(sizeof(int) == 4) {m_STK_Set_RVR_RELOAD(5000000);} else {m_STK_Set_RVR_RELOAD(50000000);}
  m_STK_Set_RVR_RELOAD(5000000); //5000000
  m_STK_Down_CVR_CURRENT;
  //m_STK_Set_CSR(1,1,1);
  m_STK_Set_CSR(mee_STK_CSR_CLKSOURCE_FAST, mee_STK_CSR_TICKINT_YES, mee_STK_CSR_ENABLE_YES);
  
  // int x = fc();
  // int y = fcpp();



	//for(;;); // Loop forever
}
