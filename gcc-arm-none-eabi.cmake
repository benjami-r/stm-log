#My: CMakeLists.txt - for STM32F0: NUCLEO-F072RB (MC: stm32f072rbt6), взят из "Create a new STM32 CMake Project..." с моим коментом.
#better name (MX): gcc-arm-none-eabi.cmake
# CMake toolchain definition for STM32CubeIDE

###################### ENVIRONMENT ######################################
set (CMAKE_SYSTEM_NAME 				"Generic") 		#My: mandatory (требуется обязательно, действует принудительно)
set (CMAKE_SYSTEM_PROCESSOR 		"arm")
# set(CMAKE_C_COMPILER_FORCED TRUE)                 #deprecated from cmake 3.6
# set(CMAKE_CXX_COMPILER_FORCED TRUE)
# set(CMAKE_C_COMPILER_ID GNU)
# set(CMAKE_CXX_COMPILER_ID GNU)
set (CMAKE_TRY_COMPILE_TARGET_TYPE 	STATIC_LIBRARY) #My: Skip link step during toolchain validation

set (TOOLCHAIN_PREFIX   "arm-none-eabi-") 			# NOTE When building from inside STM32CubeIDE the location of the toolchain is resolved by the "MCU Toolchain" project setting (via PATH).  
set (CMAKE_C_COMPILER   "${TOOLCHAIN_PREFIX}gcc")
set (CMAKE_ASM_COMPILER "${TOOLCHAIN_PREFIX}gcc")
set (CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}g++")
#set (CMAKE_LINKER       "${TOOLCHAIN_PREFIX}ld")
set (CMAKE_LINKER       "${TOOLCHAIN_PREFIX}g++") #MX
set (CMAKE_OBJCOPY      "${TOOLCHAIN_PREFIX}objcopy")
set (CMAKE_SIZE         "${TOOLCHAIN_PREFIX}size")
# set (CMAKE_AR           "${TOOLCHAIN_PREFIX}ar")
# set (CMAKE_RANLIB       "${TOOLCHAIN_PREFIX}ranlib")
# set (CMAKE_STRIP        "${TOOLCHAIN_PREFIX}ld")

set (CMAKE_EXECUTABLE_SUFFIX_ASM        ".elf")
set (CMAKE_EXECUTABLE_SUFFIX_C          ".elf")         #set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")
set (CMAKE_EXECUTABLE_SUFFIX_CXX        ".elf")

###################### VARIABLES ######################################
set (CMAKE_C_STANDARD                   "11")           #-std=gnu11
set (CMAKE_CXX_STANDARD                 "11")           #-std=gnu++11

set (MY_MCU                             "STM32F072xB")
set (MY_MCU_FLAGS                       "-D${MY_MCU} -mcpu=cortex-m0 -mthumb -mfloat-abi=soft")
set (CMAKE_C_FLAGS                      "${MY_MCU_FLAGS} -ffunction-sections -fdata-sections -fstack-usage -Wall -Wextra -Wpedantic --specs=nano.specs") #-Wextra:"unused variable"; -Wpedantic:0b10101010-"binary constants are a C2X feature or GCC extension"
set (CMAKE_CXX_FLAGS                    "${CMAKE_C_FLAGS}")
set (CMAKE_ASM_FLAGS                    "${CMAKE_C_FLAGS} -x assembler-with-cpp")
set (CMAKE_EXE_LINKER_FLAGS             "-T../Core/Configs/linker.ld -static -Wl,-Map=% -Wl,--gc-sections -Wl,--print-memory-usage --specs=nano.specs") #-nostdlib --specs=nosys.specs

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE                "Debug")
endif()
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_C_FLAGS_DEBUG             "-DDEBUG -O0 -g3") #не влияет на CMAKE_C/ASM_FLAGS, а дополняет опц-строку для BUILD_TYPE=DEBUG
    set(CMAKE_CXX_FLAGS_DEBUG           "-DDEBUG -O0 -g3") #не влияет на CMAKE_C/ASM_FLAGS, а дополняет опц-строку для BUILD_TYPE=DEBUG
    set(CMAKE_ASM_FLAGS_DEBUG           "-DDEBUG -O0 -g3") #не влияет на CMAKE_C/ASM_FLAGS, а дополняет опц-строку для BUILD_TYPE=DEBUG
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_C_FLAGS_DEBUG             "-DRELEASE -Os -g0")
    set(CMAKE_CXX_FLAGS_DEBUG           "-DRELEASE -Os -g0")
    set(CMAKE_ASM_FLAGS_DEBUG           "-DRELEASE -Os -g0")
endif()
