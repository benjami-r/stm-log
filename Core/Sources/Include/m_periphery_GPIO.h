#pragma once
//= = = = = = = = = = = = = = = = = = = = = = = = = 
//#include "m_periphery_GPIO.h"
//Subject: GPIOx macro definitions & enums.
//Used: CMSIS:
//  Drivers/CMSIS/Device/ST/STM32F0xx/Include/stm32f072xb.h
//Names (v0.1):
//  'm_...'     - My define
//  'me_...'    - My enum
//  'mee_...'   - My enum element
//  'ms_...'    - My struct
//  'mu_...'    - My union
//  'm_UNIT_Enable/Disable_SUBUNIT' - unit on/off
//  'm_UNIT_Up/Down_REG_FLAG'       - bit on/off (1/0 - flag bit, fields, i/o)
//  'm_UNIT_Get/Set_REG_FLAG'       - get/set configuration (flag bit, fields)
//  'm_UNIT_Reset  _REG_FLAG'       - reset to 'Reset value' (flag bit, fields)
//  'm_ACTION_Do_SUBACTION'         - do some action (no set data - commands)
//= = = = = = = = = = = = = = = = = = = = = = = = =
#ifndef __STM32F072xB_H
#include "stm32f072xb.h"
#endif

    //ВКЛЮЧАЕМ/ОТКЛЮЧАЕМ тактирование конкретного порта X[A..F] (RM0091:47,136,149)
    #define m_GPIOx_Enable(X)  { RCC->AHBENR |=  RCC_AHBENR_GPIO##X##EN; } //Use: m_GPIOx_ENABLE(A)
    #define m_GPIOx_Disable(X) { RCC->AHBENR &= ~RCC_AHBENR_GPIO##X##EN; }
    //5-ТЬ МАКРОСОВ для конфигурования конкретного пина ([0..15]) конкретного порта X[A..F]: для 4-х управляющих регистров (GPIOx_MODER, GPIOx_OTYPER, GPIOx_OSPEEDR, GPIOx_PUPDR) и объединенный макрос.
    //MODER: для pin[0..15] порта X[A..F] чистим его 2 бита, затем устанавливаем туда заданное значение.
    enum me_GPIO_MODER {mee_GPIO_MODER_INPUT, mee_GPIO_MODER_OUTPUT, mee_GPIO_MODER_ALTERNATE, mee_GPIO_MODER_ANALOG};
    #define m_GPIOxpin_Set_MODER(X, PIN, MODE) { \
        GPIO##X->MODER      &= ~GPIO_MODER_MODER##PIN##_Msk; \
        GPIO##X->MODER      |= (MODE << GPIO_MODER_MODER##PIN##_Pos); \
    }
    //OTYPER: для pin[0..15] порта X[A..F] чистим его 1 бит, затем устанавливаем туда заданное значение.
    enum me_GPIO_OTYPER {mee_GPIO_OTYPER_PUSHPULL, mee_GPIO_OTYPER_OPENDRAIN};
    #define m_GPIOxpin_Set_OTYPER(X, PIN, OTYPE) { \
        GPIO##X->OTYPER     &= ~GPIO_OTYPER_OT_##PIN; \
        GPIO##X->OTYPER     |= (OTYPE << PIN); \
    }
    //OSPEEDR: для pin[0..15] порта X[A..F] чистим его 2 бита, затем устанавливаем туда заданное значение.
    enum me_GPIO_OSPEEDR {mee_GPIO_OSPEEDR_LOW, mee_GPIO_OSPEEDR_MEDIUM, mee_GPIO_OSPEEDR_LOW_, mee_GPIO_OSPEEDR_HIGH}; //x0: Low speed; 01: Medium speed; 11: High speed
    #define m_GPIOxpin_Set_OSPEEDR(X, PIN, OSPEED) { \
        GPIO##X->OSPEEDR    &= ~GPIO_OSPEEDR_OSPEEDR##PIN##_Msk; \
        GPIO##X->OSPEEDR    |= (OSPEED << GPIO_OSPEEDR_OSPEEDR##PIN##_Pos); \
    }
    //PUPDR: PIN[0..15] порт X[A..F] - чистим его 2 бита, затем устанавливаем туда заданное значение:
    enum me_GPIO_PUPDR { mee_GPIO_PUPDR_NOPUPD, mee_GPIO_PUPDR_PU, mee_GPIO_PUPDR_PD, mee_GPIO_PUPDR_RESERVED}; //00: No pull-up, pull-down; 01: Pull-up; 10: Pull-down; 11: Reserved (RM0091:149mode..158regs..164rmap..165)
    #define m_GPIOxpin_Set_PUPDR(X, PIN, PUPD) { \
        GPIO##X->PUPDR      &= ~GPIO_PUPDR_PUPDR##PIN##_Msk; \
        GPIO##X->PUPDR      |= (PUPD << GPIO_PUPDR_PUPDR##PIN##_Pos); \
    }
    //SET: объединенное конфигурирование всех управляющих регистров GPIO посредством 4-х макросов выше
    //Use: m_GPIOxpin_Set(A, 5, mee_GPIO_MODER_GENERAL, mee_GPIO_OTYPER_PUSHPULL, mee_GPIO_OSPEEDR_LOW, mee_GPIO_PUPDR_NOPUPD)
    #define m_GPIOxpin_Set(X, PIN, MODE, OTYPE, OSPEED, PUPD) { \
        m_GPIOxpin_Set_MODER(X, PIN, MODE) \
        m_GPIOxpin_Set_OTYPER(X, PIN, OTYPE) \
        m_GPIOxpin_Set_OSPEEDR(X, PIN, OSPEED) \
        m_GPIOxpin_Set_PUPDR(X, PIN, PUPD) \
    }

/*
    //вариант "все в одном"
    #define mGPIOxpin_CONFIG(X, PIN, MODE, OTYPE, OSPEED, PUPD) { \
        GPIO##X->MODER      &= ~GPIO_MODER_MODER##PIN##_Msk; \
        GPIO##X->MODER      |= (MODE << GPIO_MODER_MODER##PIN##_Pos); \
        GPIO##X->OTYPER     &= ~GPIO_OTYPER_OT_##PIN; \
        GPIO##X->OTYPER     |= (OTYPE << PIN); \
        GPIO##X->OSPEEDR    &= ~GPIO_OSPEEDR_OSPEEDR##PIN##_Msk; \
        GPIO##X->OSPEEDR    |= (OSPEED << GPIO_OSPEEDR_OSPEEDR##PIN##_Pos); \
        GPIO##X->PUPDR      &= ~GPIO_PUPDR_PUPDR##PIN##_Msk; \
        GPIO##X->PUPDR      |= (PUPD << GPIO_PUPDR_PUPDR##PIN##_Pos); \
    }
*/
    //ПОДАЕМ 0/1 в конкретный пин ([0..15]) конкретного порта X[A..F]:
    #define m_GPIOxpin_Up(X, PIN)   { GPIO##X->BSRR |= GPIO_BSRR_BS_##PIN; } //Use: m_GPIOxpin_SET(A, 5)
    #define m_GPIOxpin_Down(X, PIN) { GPIO##X->BSRR |= GPIO_BSRR_BR_##PIN; }

//= = = = = = = = = = = = = = = = = = = = = = = = = 
//прямая работа (CMSIS без моих макросов):
    // //включаем LD - 1
    // *((volatile uint32_t*)(RCC_BASE + 0x14))     |= RCC_AHBENR_GPIOAEN;
    // *((volatile uint32_t*)(GPIOA_BASE + 0x00))   &= ~GPIO_MODER_MODER5;
    // *((volatile uint32_t*)(GPIOA_BASE + 0x00))   |= GPIO_MODER_MODER5_0;
    // *((volatile uint32_t*)(GPIOA_BASE + 0x18))   |= GPIO_BSRR_BS_5;

    // //включаем LD -2
    // RCC->AHBENR  |= RCC_AHBENR_GPIOAEN; //включаем тактирование GPIOA (RM0091:47,136,149)
    // GPIOA->MODER &= ~ GPIO_MODER_MODER5_Msk; //чистим MODER5 (LD на GPIOA5) (RM0091:164,158)
    // GPIOA->MODER |= GPIO_MODER_MODER5_0; //и устанавливаем в "01: General purpose output mode"
    // GPIOA->BSRR      |= GPIO_BSRR_BS_5; //устанавливаем порт в 1 (зажигаем LD)

//CMSIS с примитивными макросами:
    // //включаем LD - 3
    // #define mGPIOA_ON RCC->AHBENR |= RCC_AHBENR_GPIOAEN //включаем тактирование GPIOA (RM0091:47,136,149)
    // #define mGPIOA5_MODE_GEN {GPIOA->MODER &= ~ GPIO_MODER_MODER5_Msk; GPIOA->MODER |= GPIO_MODER_MODER5_0;} //чистим MODER5 (LD на GPIOA5) (RM0091:164,158); и устанавливаем в "01: General purpose output mode"
    // #define mGPIOA5_ON GPIOA->BSRR |= GPIO_BSRR_BS_5 //устанавливаем порт в 1 (зажигаем LD)
    // mGPIOA_ON;
    // mGPIOA5_MODE_GEN; //PA5 в режим
    // mGPIOA5_ON; //подаем 1(включаем LD)
