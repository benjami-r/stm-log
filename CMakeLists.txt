# $ rm -rf Build; cmake -B Build && cmake --build Build
# $ cmake --build Build --target program

cmake_minimum_required(VERSION 3.20)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "My: Use build directory different from source directory!")
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
set (CMAKE_EXE_LINKER_FLAGS             "-T../Core/linker.ld -nostdlib -static -Wl,-Map=% -Wl,--gc-sections -Wl,--print-memory-usage") #"-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map"

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE                "Debug")
endif()
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_C_FLAGS_DEBUG             "-DDEBUG -O0 -g3") #не влияет на CMAKE_C/ASM_FLAGS, а дополняет опц-строку для BUILD_TYPE=DEBUG
    set(CMAKE_ASM_FLAGS_DEBUG           "-DDEBUG -O0 -g3") #не влияет на CMAKE_C/ASM_FLAGS, а дополняет опц-строку для BUILD_TYPE=DEBUG
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_C_FLAGS_DEBUG             "-DRELEASE -Os -g0")
    set(CMAKE_ASM_FLAGS_DEBUG           "-DRELEASE -Os -g0")
endif()


###################### PROJECT SETUP ##################################

cmake_path(GET CMAKE_SOURCE_DIR 		STEM   PROJECT_NAME)         #STEM/FILENAME
project( ${PROJECT_NAME}                #This sets: CMAKE_PROJECT_NAME, PROJECT_SOURCE_DIR, PROJECT_BINARY_DIR...
    VERSION 							0.0.0.1                 #<major>[.<minor>[.<patch>[.<tweak>]]]
    DESCRIPTION                         "My template for STM32F0: NUCLEO-F072RB (MC: stm32f072rbt6)"
    HOMEPAGE_URL                        "https://github.com/benjami-r/f000"
    LANGUAGES                           C ASM #CXX
)

include_directories(
    Core/Sources/Include
    Core/Drivers/CMSIS/Device/ST/STM32F0xx/Include                     #взято из Repository
    Core/Drivers/CMSIS/Include                                         #взято из Repository
)
add_executable( ${PROJECT_NAME}
    Core/Sources/startup.s
    Core/Sources/main.c
)
#add_compile_definitions()
#add_compile_options() #не влияет на CMAKE_C/ASM_FLAGS, а дополняет опц-строку
#add_link_options() #не влияет на CMAKE_EXE_LINKER_FLAGS, а дополняет опц-строку
#add_library()


###################### CUSTOM TARGETS & COMMANDS ######################

#add_custom_target(objcopy_binary ALL ${CMAKE_OBJCOPY} -O binary -S "${PROJECT_NAME}.elf" "${PROJECT_NAME}.bin")
# add_custom_target("${PROJECT_NAME}.bin"
#     #ALL 
#     ${CMAKE_OBJCOPY} -O binary -S "${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX_C}" "${PROJECT_NAME}.bin"
#     DEPENDS ${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX_C}
# )
# add_custom_target(UPLOAD
#         arm-none-eabi-gdb -iex "target remote tcp:127.0.0.1:3333"
#         -iex "monitor program $<TARGET_FILE:${PROJECT_NAME}.elf>"
#         -iex "monitor reset init"
#         -iex "disconnect" -iex "quit"
# )
add_custom_target(program
    openocd -f board/st_nucleo_f0.cfg -c "program ${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX_C} verify reset exit"
)
# add_custom_target(size
#     ALL
#     ${CMAKE_SIZE} ${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX_C}
# )
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX_C}
    COMMENT "Displays the sizes of sections inside binary file ${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX_C}:"
)
# = = = = = = = = = = = = = = = = = = = = = = = 
# See: https://cmake.org/cmake/help/latest/
# $ cmake --build Debug --target clean  - подчищает часть, но не удаляет Debug
# $ cd f000
# $ rm -rf Debug                //$ make clean
# $ cmake -B Debug
# $ cmake --build Debug        //$ make -C Debug
# $ openocd -f board/st_nucleo_f0.cfg -c "program Debug/f000 verify reset exit"

# $ arm-none-eabi-objcopy -O binary -S ttt0.elf ttt0.bin
# https://github.com/ptrks/CMake-avr-example/blob/master/basic_example/CMakeLists.txt

# Создай проект и см. MX-noAll-Adv-CMake/cmake/gcc-arm-none-eabi.cmake

# add_subdirectory() - добавить подпроекты со своими цмаками?
# add_custom_target (можно реализовать openocd, свой clean) https://stackoverflow.com/questions/9680420/looking-for-a-cmake-clean-command-to-clear-up-cmake-output
