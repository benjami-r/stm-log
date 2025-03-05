#pragma once
//= = = = = = = = = = = = = = = = = = = = = = = = =
//#include "mPeriphCore_STK.h"
//Subject: SysTick macro definitions & enums.
//Used: CMSIS:
//  Drivers/CMSIS/Include/core_cm0.h
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
#ifndef __CORE_CM0_H_GENERIC
#include "core_cm0.h"
#endif
#ifndef __CMSIS_GCC_H
#include "cmsis_gcc.h"
#endif

    //Чтение/запись осуществляется безопасно - всегда применяем маску чтоб не тронуть Reserved.

    //0. SysTick обычно работает с прерыванием, поэтому прежде всего отдельно заготавливаем обработчик прерывания SysTick_Handler.

    //1. 24-бит-поле STK_RVR:RELOAD [1..0xFFFFFF] - стартовое значение счетчика, автозагружаемое в STK_CVR:CURRENT со следующего цикла (столько тиков будет отсчитываться до 0). Значение 0 фрозит SysTick (пустой цикл). Помним, что COUNTFLAG устанавливается и прерывание вызывается на следующем за переходом счетчика с 1 в 0 такте.
    #define m_STK_Get_RVR_RELOAD (SysTick->LOAD & SysTick_LOAD_RELOAD_Msk) //читаем из STK_RVR поле RELOAD
    #define m_STK_Set_RVR_RELOAD(RELOAD) { SysTick->LOAD = \
        (SysTick->LOAD & ~SysTick_LOAD_RELOAD_Msk) | (RELOAD & SysTick_LOAD_RELOAD_Msk); \
    } //пишем параметр в STK_RVR поле RELOAD

    //2. 24-бит-поле STK_CVR:CURRENT - текущее значение счетчика.
    #define m_STK_Get_CVR_CURRENT (SysTick->VAL & SysTick_VAL_CURRENT_Msk) //читаем из STK_CVR поле CURRENT
    #define m_STK_Down_CVR_CURRENT { SysTick->VAL &= ~SysTick_VAL_CURRENT_Msk; } //чистим счетчик STK_CVR_CURRENT актом записи, тем самым заставляя SysTick перейти к новому циклу, что также чистит флаг COUNTFLAG в STK_CSR. Это не вызывает прерывание (тот случается при переходе счетчика от 1 к 0).

    //3. Работу с STK_CALIB опускаем - для STM32F0 он какой-то не рабочий (только чтение)
    #define m_STK_Get_CALIB (SysTick->CALIB) //хотя, прочитаем.

    //4. Работаем с регистром управления и состояния STK_CSR (и консолидировано, и отдельно по полям)
    #define m_STK_Get_CSR_COUNTFLAG ( (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) >> SysTick_CTRL_COUNTFLAG_Pos ) //Вернет флаг - 1 если таймер досчитал до 0 с момента последнего считывания (т.е. такое чтение сбросит этот флаг). (Флаг сдвигается вправо на 0-й бит).
    #define m_STK_Get_CSR_CLKSOURCE ( (SysTick->CTRL & SysTick_CTRL_CLKSOURCE_Msk) >> SysTick_CTRL_CLKSOURCE_Pos ) //Вернет флаг - источник тактиторания: 0 = Внешний, 1 = Тактировщик ЦП (HCLK). (Флаг сдвигается вправо на 0-й бит).
    #define m_STK_Get_CSR_TICKINT   ( (SysTick->CTRL & SysTick_CTRL_TICKINT_Msk)   >> SysTick_CTRL_TICKINT_Pos )   //Вернет флаг - генерировать ли прерывание SysTick по достижению счетчика STK_CVR ноля: 0 = Нет, 1 = Да. (Флаг сдвигается вправо на 0-й бит).
    #define m_STK_Get_CSR_ENABLE    ( (SysTick->CTRL & SysTick_CTRL_ENABLE_Msk)     >> SysTick_CTRL_ENABLE_Pos )   //Вернет флаг - запущен ли счетчик SysTick: 0 = Нет, 1 = Да. (Флаг сдвигается вправо на 0-й бит).
    #define m_STK_Get_CSR           (SysTick->CTRL) //Вернет значение всего регистра, как есть.

    #define m_STK_Set_CSR_CLKSOURCE(CLKSOURCE) { SysTick->CTRL = \
        (SysTick->CTRL & ~SysTick_CTRL_CLKSOURCE_Msk) | (CLKSOURCE << SysTick_CTRL_CLKSOURCE_Pos & SysTick_CTRL_CLKSOURCE_Msk); \
    } //пишем параметр в STK_CSR поле CLKSOURCE - источник тактиторания: 0 = Внешний, 1 = Тактировщик ЦП (HCLK). (Флаг-параметр сдвигаем на место поля CLKSOURCE).
    #define m_STK_Set_CSR_TICKINT(TICKINT) { SysTick->CTRL = \
        (SysTick->CTRL & ~SysTick_CTRL_TICKINT_Msk) | (TICKINT << SysTick_CTRL_TICKINT_Pos & SysTick_CTRL_TICKINT_Msk); \
    } //пишем параметр в STK_CSR поле TICKINT - флаг - генерировать ли прерывание SysTick по достижению счетчика STK_CVR ноля: 0 = Нет, 1 = Да. (Флаг-параметр сдвигаем на место поля TICKINT).
    #define m_STK_Set_CSR_ENABLE(ENABLE) { SysTick->CTRL = \
        (SysTick->CTRL & ~SysTick_CTRL_ENABLE_Msk) | (ENABLE << SysTick_CTRL_ENABLE_Pos & SysTick_CTRL_ENABLE_Msk); \
    } //пишем параметр в STK_CSR поле ENABLE - флаг - запускать ли счетчик (рабочий цикл) SysTick: 0 = Нет, 1 = Да. По достижению счетчиком STK_CVR ноля (а именно, по переключению с 1 на 0) будет устанаовлен COUNTFLAG, а также генерировано прерывание SysTick (если подключено). (Флаг-параметр сдвигаем на место поля ENABLE).

    #define m_ASM_Do_DataSynchronizationBarrier { __DSB(); } //выполняем инструкцию барьера сразу после настройки и запуска SysTick, дабы убедиться, что он точно стартует в этой точке (команда из #include "cmsis_gcc.h")


    enum me_STK_CSR_CLKSOURCE {mee_STK_CSR_CLKSOURCE_SLOW, mee_STK_CSR_CLKSOURCE_FAST};
    enum me_STK_CSR_TICKINT {mee_STK_CSR_TICKINT_NO, mee_STK_CSR_TICKINT_YES};
    enum me_STK_CSR_ENABLE {mee_STK_CSR_ENABLE_NO, mee_STK_CSR_ENABLE_YES};
    #define m_STK_Set_CSR(CLKSOURCE, TICKINT, ENABLE) { SysTick->CTRL = \
        (SysTick->CTRL & ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk)) | \
        ( \
            (CLKSOURCE << SysTick_CTRL_CLKSOURCE_Pos & SysTick_CTRL_CLKSOURCE_Msk) | \
            (TICKINT << SysTick_CTRL_TICKINT_Pos     & SysTick_CTRL_TICKINT_Msk)   | \
            (ENABLE << SysTick_CTRL_ENABLE_Pos       & SysTick_CTRL_ENABLE_Msk) \
        ); \
    } //консолидировано в STK_CSR пишем три параметра: CLKSOURCE, TICKINT и ENABLE, и выполняем инструкцию барьера.
   //m_ASM_Do_DataSynchronizationBarrier
