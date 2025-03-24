# CMakeLists.txt        - STM32F0: NUCLEO-F072RB (STM32F072xB - stm32f072rbt6), за базу взял из "Create a new STM32 CMake Project..."
#    https://cmake.org/cmake/help/latest/
# $    rm -rf Debug; cmake -B Debug && cmake --build Debug
# $    cmake --build Debug --target program
# Управляется: в теку сорса билдить нельзя (cmake --build .) - даст ошибку; имя теки проекта (отбросив расширение) будет именем проекта; CMAKE_BUILD_TYPE задает тип билда (Debug/Release), если не задан, берется из CMAKE_BINARY_DIR (т.е. из команды запуска, п: cmake --build Debug); LINK_WITH_LIBS задает линковать ли библы (пока просто дефайненена или нет, далее может XXX какие: С/С++ ...).

#TODO:
#Предусмотреть компиляции:
#   - ASM C, без библиотек - ПРОСТО БЕЗ CXX? НЕ НАДО!, 
#   - ASM C CXX, без библиотек (т.е. только компиляционный CXX - что даст?),
#   - ASM C CXX, с библиотекой, обеспечивающей CXX.

###################### BEGIN ##########################################
cmake_minimum_required(VERSION 3.20)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "My BEGIN: Attempt to use build directory same source directory!")
endif()
###################### CMAKE_TOOLCHAIN_FILE ########################### перед PROJECT SETUP иначе игнорит arm-toolchain; можно "cmake ... -P cubeide-gcc.cmake"
set(CMAKE_TOOLCHAIN_FILE         gcc-arm-none-eabi.cmake)  #include(cubeide-gcc.cmake RESULT_VARIABLE MY_MODULE_CUBEIDE_GCC_CMAKE)
###################### PROJECT SETUP ################################## перед флагами, ибо там юзится CMAKE_PROJECT_NAME
cmake_path(GET CMAKE_SOURCE_DIR  STEM   PROJECT_NAME)         #STEM/FILENAME
project( ${PROJECT_NAME}         #This sets: CMAKE_PROJECT_NAME, PROJECT_SOURCE_DIR, PROJECT_BINARY_DIR...
    VERSION                      0.0.0.0                 #<major>[.<minor>[.<patch>[.<tweak>]]]
    DESCRIPTION                  "log for STM32F0: NUCLEO-F072RB (MC: stm32f072rbt6)"
    HOMEPAGE_URL                 "https://github.com/benjami-r/stm-log"
    LANGUAGES                    ASM C CXX
)
include_directories(
    Core/Sources/Include
    Drivers/CMSIS/Device/ST/STM32F0xx/Include      #взято из Repository
    Drivers/CMSIS/Include                          #взято из Repository
)
add_executable( ${PROJECT_NAME}
    Core/Sources/startup.s
    Core/Sources/main.c
    Core/Sources/probe.cpp
)
###################### VARIABLES ######################################
set(CMAKE_C_STANDARD             "11")     #-std=gnu11
set(CMAKE_CXX_STANDARD           "11")     #-std=gnu++11 (дает using-алиас)
set(CMAKE_CXX_STANDARD_REQUIRED  ON)       #требовать, ибо using-алиас с 11
set(CMAKE_EXECUTABLE_SUFFIX      ".elf")   #наверное после PROJECT SETUP, ибо иначе некуда лепить свойство
###################### MY GCC FLAGS ###################################
set(F_TARGET                     "-mcpu=cortex-m0 -mthumb -mfloat-abi=soft")
set(F_SPECS                      "-specs=nano.specs -specs=nosys.specs")
set(F_OPTIMIZATION               "-fdata-sections -ffunction-sections") #places each data and function into it's own section
set(F_DEVELOP                    "-fstack-usage") #выводить в .su инф. об использовании стека каждой функцией.
set(F_WARN                       "-Wall -Wextra -Wpedantic") #-Wextra:"unused variable"; -Wpedantic:0b10101010-"binary constants are a C2x feature or GCC extension"
set(F_GCC                        "${F_TARGET} ${F_SPECS} ${F_OPTIMIZATION} ${F_DEVELOP} ${F_WARN}")
set(F_GCC_ASM                    "${F_GCC} -x assembler-with-cpp -MMD -MP") #-MMD -MP - формировать файл зависимостей в формате Makefile, включать пустые цели
set(F_GCC_C                      "${F_GCC}")
set(F_GCC_CXX                    "${F_GCC}") #-fno-rtti -fno-exceptions -fno-threadsafe-statics
###################### GCC FLAGS ######################################
set(CMAKE_ASM_FLAGS              "${F_GCC_ASM}")
set(CMAKE_C_FLAGS                "${F_GCC_C}")
set(CMAKE_CXX_FLAGS              "${F_GCC_CXX}")
#::: DEBUG / RELEASE
if(NOT CMAKE_BUILD_TYPE) #присвоим переменной CMAKE_BUILD_TYPE имя бин-теки из текущего вызова cmake --build ... (например Debug если было "cmake --build Debug").
    cmake_path(GET CMAKE_BINARY_DIR  STEM  CMAKE_BUILD_TYPE) # set(CMAKE_BUILD_TYPE "Debug")
endif()
message("My DEBUG RELEASE: CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}.")
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_C_FLAGS_DEBUG      "-DDEBUG -O0 -g3") #не влияет на CMAKE_C/ASM_FLAGS, а дополняет опц-строку для BUILD_TYPE=DEBUG
    set(CMAKE_CXX_FLAGS_DEBUG    "-DDEBUG -O0 -g3") #не влияет на CMAKE_C/ASM_FLAGS, а дополняет опц-строку для BUILD_TYPE=DEBUG
    set(CMAKE_ASM_FLAGS_DEBUG    "-DDEBUG -O0 -g3") #не влияет на CMAKE_C/ASM_FLAGS, а дополняет опц-строку для BUILD_TYPE=DEBUG
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_C_FLAGS_DEBUG      "-DRELEASE -Os -g0")
    set(CMAKE_CXX_FLAGS_DEBUG    "-DRELEASE -Os -g0")
    set(CMAKE_ASM_FLAGS_DEBUG    "-DRELEASE -Os -g0")
else()
    message(FATAL_ERROR "My DEBUG RELEASE: Unknown CMAKE_BUILD_TYPE.")
endif()
#::: LINKER FLAGS   https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html
set(F_GCC_LINK_COMMON            "-Wl,-Map=${CMAKE_PROJECT_NAME}.map,--gc-sections,--print-memory-usage") #-Wl,-Map=% # -Wl,--gc-sections - не включать неиспользуемый код (требует для компилятора опций -ffunction-sections and -fdata-sections)
set(F_GCC_LINK_SCRIPT            "-T \"${CMAKE_SOURCE_DIR}/Core/Configs/linker.ld\"")  #"-T \"../Core/Configs/STM32F072RBTX_FLASH.ld\"")
set(F_GCC_LINK                   "${F_GCC_LINK_COMMON} ${F_GCC_LINK_SCRIPT}")
set(F_GCC_LINK_LIBS_NO           "-nostdlib -nostdlib++")
#set(F_GCC_LINK_C_LIBS            "-static -Wl,--start-group -lc -lm -Wl,--end-group") #-nostdlib  #-static - предпочитать стат-либы (.a) динамик-либам (.so) если найдены обе.; --start-group ...--end-group - объеденить архивы (-l-библы) в общее пространство имен и искать внешние ссылки друг у друга повторно их перепросматривая - нужно при взаимных ссылках; -lc -lm - стандартная С- и мат-библы.
set(F_GCC_LINK_LIBS_C            "-Wl,--start-group,-lc,-lm,--end-group") #-nostdlib  #-static - предпочитать стат-либы (.a) динамик-либам (.so) если найдены обе.; --start-group ...--end-group - объеденить архивы (-l-библы) в общее пространство имен и искать внешние ссылки друг у друга повторно их перепросматривая - нужно при взаимных ссылках; -lc -lm - стандартная С- и мат-библы.
#set(F_GCC_LINK_CXX_LIBS          "-static -Wl,--start-group -lstdc++ -lsupc++ -Wl,--end-group") #-nostdlib  #-static - предпочитать стат-либы (.a) динамик-либам (.so) если найдены обе.
set(F_GCC_LINK_LIBS_CXX          "-Wl,--start-group,-lstdc++,-lsupc++,--end-group") #-nostdlib  #-static - предпочитать стат-либы (.a) динамик-либам (.so) если найдены обе.
# set(CMAKE_C_LINK_FLAGS           "")--
# set(CMAKE_EXE_LINKER_FLAGS       "")--
if(NOT LINK_WITH_LIBS)
    message("My LINKER FLAGS: Link without libs.")
    set(CMAKE_EXE_LINKER_FLAGS   "${F_GCC_LINK} ${F_GCC_LINK_LIBS_NO}")
else()
    message("My LINKER FLAGS: Link with libs.")
    set(CMAKE_EXE_LINKER_FLAGS   "${F_GCC_LINK} ${F_GCC_LINK_LIBS_C} ${F_GCC_LINK_LIBS_CXX}")
endif()

###################### CUSTOM TARGETS & COMMANDS ######################
add_custom_command(                     #size & .bin   #добавляем команды к главной цели
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}
    COMMAND ${CMAKE_OBJCOPY} -O binary -S "${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}" "${PROJECT_NAME}.bin"
    COMMENT "Sizes of sections inside ${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX} (and create ${PROJECT_NAME}.bin):"
)
add_custom_target(program               #UPLOAD executable to flash
    openocd -f board/st_nucleo_f0.cfg -c "program ${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX} verify reset exit"
)
#######################################################################
#######################################################################
#######################################################################

############### MEMO
#set(CMAKE_STATIC_LIBRARY_SUFFIX ".a") #set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")

# set(CMAKE_EXECUTABLE_SUFFIX_ASM  ".elf")
# set(CMAKE_EXECUTABLE_SUFFIX_C    ".elf")         
# set(CMAKE_EXECUTABLE_SUFFIX_CXX  ".elf")

#add_compile_definitions()
#add_compile_options() #не влияет на CMAKE_C/ASM_FLAGS, а дополняет опц-строку
#add_link_options() #не влияет на CMAKE_EXE_LINKER_FLAGS, а дополняет опц-строку
#add_library()

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
# add_custom_target(size
#     ALL
#     ${CMAKE_SIZE} ${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX_C}
# )

# $ cmake --build Debug --target clean  - подчищает часть, но не удаляет Debug
# add_subdirectory() - добавить подпроекты со своими цмаками?
# add_custom_target (можно реализовать openocd, свой clean) https://stackoverflow.com/questions/9680420/looking-for-a-cmake-clean-command-to-clear-up-cmake-output
