/* My */
/* ******************************************************************************
** STM32F072RBTX_FLASH_My.ld - My linker script for NUCLEO-F072RB Board (STM32F072RBTx).
** Set heap size, stack size and stack location.
****************************************************************************** */

MEMORY
{
  RAM     (xrw) : ORIGIN = 0x20000000,   LENGTH =  16K
  FLASH    (rx) : ORIGIN = 0x08000000,   LENGTH = 128K
}

ENTRY(Reset_Handler)
_ld_stack_end       = ORIGIN(RAM) + LENGTH(RAM);  /* _estack - Highest address of the user mode stack - end of "RAM" - адрес за последним словом RAM, конец стека */
_ld_stack_min_size  = 0x400;                      /* _Min_Stack_Size - required amount of stack */
_ld_heap_min_size   = 0x200;                      /* _Min_Heap_Size - required amount of heap */

SECTIONS
{
  /* -------------------- VMA : >FLASH -------------------- неизменяемое: код, константы - декларируются, дефенируются и остаются работать в ROM (таблица векторов потенциально перемещаема) */
  .isr_vectors :       /* Таблица векторов Interrupt Service Routine */
  {
    . = ALIGN(4);      /* Выровнять по 4-х байтовому (32 битному) слову, т.е. 2 младших бита адреса = '00' */
    KEEP(*(.isr_vector))
    . = ALIGN(4);
  } >FLASH

  .text :               /* The program code into "FLASH" Rom */
  {
    . = ALIGN(4);
    *(.text)
    *(.text*)
    . = ALIGN(4);
  } >FLASH

  .rodata :             /* Constant data into "FLASH" Rom (constants, strings, etc.) */
  {
    . = ALIGN(4);
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4);
  } >FLASH

  /* -------------------- VMA : >RAM AT> FLASH -------------------- */

  _ld_data_lma_start = LOADADDR(.data);  /* _sidata - LMA-адрес секции .data для копирования */

  .data :               /* Initialized data sections into "RAM" Ram */
  {
    . = ALIGN(4);
    _ld_data_vma_start = .;    /* _sdata - VMA-адрес начала секции .data (data start) */
    *(.data)
    *(.data*)
    . = ALIGN(4);
    _ld_data_vma_end = .;      /* _edata - define a global symbol at data end */
  } >RAM AT> FLASH

  /* -------------------- VMA : >RAM -------------------- */

  . = ALIGN(4);         /* Uninitialized data section into "RAM" Ram */
  .bss :
  {
    _ld_bss_start = .;     /* _sbss - define a global symbol at bss start */
    *(.bss)
    *(.bss*)
    *(COMMON)           /* см. директиву as .lcomm, см. ld.pdf : 3.6.4.3 Input Section for Common Symbols */
    . = ALIGN(4);
    _ld_bss_end = .;       /* _ebss - define a global symbol at bss end */
  } >RAM

  ._heap_stack :  /* _user_heap_stack User_heap_stack section, used to check that there is enough "RAM" Ram type memory left - только для проверки */
  {
    . = ALIGN(8);
    . = . + _ld_heap_min_size;   /* _Min_Heap_Size */
    . = . + _ld_stack_min_size;  /* _Min_Stack_Size */
    . = ALIGN(8);
  } >RAM

  /DISCARD/ :       /* Remove information from the compiler libraries */
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  /* 
  .ARM.attributes 0 : { *(.ARM.attributes) }
  */
}
