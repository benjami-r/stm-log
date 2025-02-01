#My: CMakeLists.txt - for STM32F0: NUCLEO-F072RB (MC: stm32f072rbt6), взят из "Create a new STM32 CMake Project..." с моим коментом.
# CMake toolchain definition for STM32CubeIDE

set (CMAKE_SYSTEM_PROCESSOR 		"arm")
set (CMAKE_SYSTEM_NAME 				"Generic") 		#My: mandatory (требуется обязательно, действует принудительно)
set (CMAKE_TRY_COMPILE_TARGET_TYPE 	STATIC_LIBRARY) #My: Skip link step during toolchain validation

set (TOOLCHAIN_PREFIX   "arm-none-eabi-") 			# NOTE When building from inside STM32CubeIDE the location of the toolchain is resolved by the "MCU Toolchain" project setting (via PATH).  
set (CMAKE_C_COMPILER   "${TOOLCHAIN_PREFIX}gcc")
set (CMAKE_ASM_COMPILER "${TOOLCHAIN_PREFIX}gcc")
set (CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}g++")
set (CMAKE_AR           "${TOOLCHAIN_PREFIX}ar")
set (CMAKE_LINKER       "${TOOLCHAIN_PREFIX}ld")
set (CMAKE_OBJCOPY      "${TOOLCHAIN_PREFIX}objcopy")
set (CMAKE_RANLIB       "${TOOLCHAIN_PREFIX}ranlib")
set (CMAKE_SIZE         "${TOOLCHAIN_PREFIX}size")
set (CMAKE_STRIP        "${TOOLCHAIN_PREFIX}ld")
