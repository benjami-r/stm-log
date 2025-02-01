cmake_minimum_required(VERSION 3.20)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Use build directory different from source directory!")
endif()

###################### CMAKE_TOOLCHAIN_FILE ########################### чтоб в cli не писать "cmake ... -P cubeide-gcc.cmake"

set (CMAKE_TOOLCHAIN_FILE         		cubeide-gcc.cmake)        #include(cubeide-gcc.cmake RESULT_VARIABLE MY_MODULE_CUBEIDE_GCC_CMAKE)

###################### VARIABLES ######################################

set (CMAKE_C_STANDARD                   "11")            #-std=gnu11
set (MY_MCU                             "STM32F072xB")            #-std=gnu11

#set (CMAKE_EXECUTABLE_SUFFIX            ".elf")         # set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")
#set (CMAKE_EXECUTABLE_SUFFIX_ASM        ".elf")
set (CMAKE_EXECUTABLE_SUFFIX_C          ".elf")

set (MY_MCU_FLAGS                       "-D${MY_MCU} -mcpu=cortex-m0 -mthumb -mfloat-abi=soft")
set (CMAKE_C_FLAGS                      "${MY_MCU_FLAGS} -Wall -Wextra -Wpedantic -ffunction-sections -fdata-sections -fstack-usage")
set (CMAKE_ASM_FLAGS                    "${CMAKE_C_FLAGS} -x assembler-with-cpp")
set (CMAKE_EXE_LINKER_FLAGS             "-T../linker.ld -nostdlib -static -Wl,-Map=% -Wl,--gc-sections -Wl,--print-memory-usage")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE                "Debug")
endif()
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(MY_BUILD_TYPE_FLAGS             "-DDEBUG -O0 -g3")
endif()
if(CMAKE_BUILD_TYPE MATCHES Release)
    set(MY_BUILD_TYPE_FLAGS             "-DRELEASE -Os -g0")
endif()
set(CMAKE_C_FLAGS_DEBUG             "${MY_BUILD_TYPE_FLAGS}") #не влияет на CMAKE_C/ASM_FLAGS, а дополняет опц-строку для BUILD_TYPE=DEBUG
set(CMAKE_ASM_FLAGS_DEBUG           "${MY_BUILD_TYPE_FLAGS}") #не влияет на CMAKE_C/ASM_FLAGS, а дополняет опц-строку для BUILD_TYPE=DEBUG

###################### PROJECT SETUP ##################################

cmake_path(GET CMAKE_SOURCE_DIR 		STEM   MY_PROJECT_NAME)         #STEM/FILENAME
project( ${MY_PROJECT_NAME}             #This sets: PROJECT_NAME, CMAKE_PROJECT_NAME, PROJECT_SOURCE_DIR, PROJECT_BINARY_DIR...
    VERSION 							0.0.0.1                 #<major>[.<minor>[.<patch>[.<tweak>]]]
    DESCRIPTION                         "My template for STM32F0: NUCLEO-F072RB (MC: stm32f072rbt6)"
    HOMEPAGE_URL                        "https://github.com/benjami-r/f000"
    LANGUAGES                           C ASM #CXX
)

include_directories(
    Sources/Inc
    Drivers/CMSIS/Device/ST/STM32F0xx/Include                     #взято из Repository
    Drivers/CMSIS/Include                                         #взято из Repository
)
add_compile_definitions(
)
add_compile_options( #не влияет на CMAKE_C/ASM_FLAGS, а дополняет опц-строку
)
add_link_options( #не влияет на CMAKE_EXE_LINKER_FLAGS, а дополняет опц-строку
)
#add_library()
add_executable( ${PROJECT_NAME}
    Startup/startup.s
    Sources/main.c
)

add_custom_target(binary_tgt ALL ${CMAKE_OBJCOPY} -O binary -S "${PROJECT_NAME}.elf" "${PROJECT_NAME}.bin")


# = = = = = = = = = = = = = = = = = = = = = = = 
# See: https://cmake.org/cmake/help/latest/
# $ rm -rf Debug; cmake -B Debug && cmake --build Debug
# $ cd f000
# $ rm -rf Debug                //$ make clean
# $ cmake -B Debug
# $ cmake --build Debug        //$ make -C Debug
# $ openocd -f board/st_nucleo_f0.cfg -c "program Debug/f000 verify reset exit"

# $ arm-none-eabi-objcopy -O binary -S ttt0.elf ttt0.bin
# https://github.com/ptrks/CMake-avr-example/blob/master/basic_example/CMakeLists.txt

# Создай проект и см. MX-noAll-Adv-CMake/cmake/gcc-arm-none-eabi.cmake

# add_subdirectory() - добавить подпроекты со своими цмаками?
