#############################################################################################################################
# file:  CMakeLists.txt
# brief: Template "CMakeLists.txt" for building of executables and static libraries.
#
# usage: Edit "VARIABLES"-section to suit project requirements.
#        For debug build:
#          cmake -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Debug -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
#          make -C Debug VERBOSE=1 -j
#        For release build:
#          cmake -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
#          make -C Release VERBOSE=1 -j
#############################################################################################################################
cmake_minimum_required(VERSION 3.20)
set (CMAKE_TOOLCHAIN_FILE         		cubeide-gcc.cmake)        #include(cubeide-gcc.cmake RESULT_VARIABLE MY_MODULE_CUBEIDE_GCC_CMAKE)
###################### CONSTANTS ######################################
set (PROJECT_TYPE_EXECUTABLE          "exe")
set (PROJECT_TYPE_STATIC_LIBRARY      "static-lib")
set (MCPU_CORTEX_M0				      "-mcpu=cortex-m0")
set (MCPU_CORTEX_M0PLUS				  "-mcpu=cortex-m0plus")
set (MCPU_CORTEX_M3				      "-mcpu=cortex-m3")
set (MCPU_CORTEX_M4				      "-mcpu=cortex-m4")
set (MCPU_CORTEX_M7				      "-mcpu=cortex-m7")
set (MCPU_CORTEX_M33				  "-mcpu=cortex-m33")
set (MCPU_CORTEX_M55				  "-mcpu=cortex-m55")
set (MCPU_CORTEX_M85				  "-mcpu=cortex-m85")
set (MFPU_FPV4_SP_D16                 "-mfpu=fpv4-sp-d16")
set (MFPU_FPV5_D16                    "-mfpu=fpv5-d16")
set (RUNTIME_LIBRARY_REDUCED_C        "--specs=nano.specs")
set (RUNTIME_LIBRARY_STD_C            "")
set (RUNTIME_LIBRARY_SYSCALLS_MINIMAL "--specs=nosys.specs")
set (RUNTIME_LIBRARY_SYSCALLS_NONE    "")
set (MFLOAT_ABI_SOFTWARE              "-mfloat-abi=soft")
set (MFLOAT_ABI_HARDWARE              "-mfloat-abi=hard")
set (MFLOAT_ABI_MIX                   "-mfloat-abi=softfp")
#######################################################################

###################### VARIABLES ######################################
set (PROJECT_NAME             "c70")
set (PROJECT_TYPE             "exe")
set (LINKER_SCRIPT            "../Core/Configs/STM32F072RBTX_FLASH.ld")
set (MCPU                     "-mcpu=Cortex-M0")
set (MFLOAT_ABI               "")
set (RUNTIME_LIBRARY          "--specs=nano.specs")
set (RUNTIME_LIBRARY_SYSCALLS "--specs=nosys.specs")


set (PROJECT_SOURCES
	# LIST SOURCE FILES HERE
	Core/Sources/startup_stm32f072rbtx.s
	Core/Sources/main.c
	Core/Sources/syscalls.c
	Core/Sources/sysmem.c
	Core/Sources/probe.cpp


	)

set (PROJECT_DEFINES
	# LIST COMPILER DEFINITIONS HERE

    )

set (PROJECT_INCLUDES
	# LIST INCLUDE DIRECTORIES HERE
	Core/Sources/Include
	Drivers/CMSIS/Device/ST/STM32F0xx/Include
	Drivers/CMSIS/Include
  )

############ MODIFY ACCORDING TO REQUIREMENTS) ########################

#######################################################################

################## PROJECT SETUP ######################################
project(${PROJECT_NAME})
enable_language(ASM C CXX)

if (${PROJECT_TYPE} MATCHES ${PROJECT_TYPE_EXECUTABLE})
  add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
  add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>)
elseif (${PROJECT_TYPE} MATCHES ${PROJECT_TYPE_STATIC_LIBRARY})
  add_library(${PROJECT_NAME} ${PROJECT_SOURCES})
endif()

add_compile_definitions (${PROJECT_DEFINES})
include_directories (${PROJECT_INCLUDES})

set (CMAKE_EXECUTABLE_SUFFIX ".elf")
set (CMAKE_STATIC_LIBRARY_SUFFIX ".a")
# set (CMAKE_C_FLAGS   "-std=gnu11   -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Wall -Werror ${RUNTIME_LIBRARY} ${RUNTIME_LIBRARY_SYSCALLS}")
# set (CMAKE_CXX_FLAGS "-std=gnu++11 -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Wall -Werror ${RUNTIME_LIBRARY} ${RUNTIME_LIBRARY_SYSCALLS}")
# set (CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp")
# set (CMAKE_EXE_LINKER_FLAGS "-T${LINKER_SCRIPT} ${RUNTIME_LIBRARY} ${RUNTIME_LIBRARY_SYSCALLS} -Wl,-Map=test.map -Wl,--gc-sections -static -Wl,--start-group -lc -lm -Wl,--end-group")
set (CMAKE_C_FLAGS   "-std=gnu11   -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Wall") # -Werror
set (CMAKE_CXX_FLAGS "-std=gnu++11 -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Wall") # -Werror
set (CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp")
set (CMAKE_EXE_LINKER_FLAGS "-T${LINKER_SCRIPT} ${RUNTIME_LIBRARY} ${RUNTIME_LIBRARY_SYSCALLS} -Wl,-Map=test.map -Wl,--gc-sections -static -Wl,--start-group -lc -lm -Wl,--end-group")



###################### CUSTOM TARGETS & COMMANDS ######################
set (CMAKE_EXECUTABLE_SUFFIX_ASM        ".elf")
set (CMAKE_EXECUTABLE_SUFFIX_C          ".elf")         #set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")
set (CMAKE_EXECUTABLE_SUFFIX_CXX        ".elf")

add_custom_command(                     #size & .bin
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX_C}
    COMMAND ${CMAKE_OBJCOPY} -O binary -S "${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX_ASM}" "${PROJECT_NAME}.bin"
    COMMENT "Sizes of sections inside ${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX_C} (and create ${PROJECT_NAME}.bin):"
)
add_custom_target(program               #UPLOAD executable to flash
    openocd -f board/st_nucleo_f0.cfg -c "program ${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX_C} verify reset exit"
)

#add_compile_definitions()
#add_compile_options() #не влияет на CMAKE_C/ASM_FLAGS, а дополняет опц-строку
#add_link_options() #не влияет на CMAKE_EXE_LINKER_FLAGS, а дополняет опц-строку
#add_library()

