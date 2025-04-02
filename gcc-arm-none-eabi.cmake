# gcc-arm-none-eabi.cmake

#TODO:
# c60_NEW_gcc-arm-none-eabi.cmake

###################### TOOLCHAIN ######################################
set(CMAKE_SYSTEM_NAME              "Generic")      #My: mandatory (требуется обязательно, действует принудительно)
set(CMAKE_SYSTEM_PROCESSOR         "arm")

set(CMAKE_TRY_COMPILE_TARGET_TYPE  STATIC_LIBRARY) #My: Skip link step during toolchain validation

set(TOOLCHAIN_PREFIX               "arm-none-eabi-")
set(CMAKE_C_COMPILER               "${TOOLCHAIN_PREFIX}gcc")
set(CMAKE_ASM_COMPILER             "${TOOLCHAIN_PREFIX}gcc") #gcc - если требуется препроцессор
set(CMAKE_CXX_COMPILER             "${TOOLCHAIN_PREFIX}g++")
#set(CMAKE_LINKER                   "${TOOLCHAIN_PREFIX}ld") #почему-то (язык CXX?) игнорится, в link.txt все равно g++.
set(CMAKE_LINKER                   "${TOOLCHAIN_PREFIX}g++") #gcc, g++ в MX, ld в IDE
set(CMAKE_OBJCOPY                  "${TOOLCHAIN_PREFIX}objcopy")
set(CMAKE_SIZE                     "${TOOLCHAIN_PREFIX}size")
# set(CMAKE_AR                       "${TOOLCHAIN_PREFIX}ar")
# set(CMAKE_RANLIB                   "${TOOLCHAIN_PREFIX}ranlib")
# set(CMAKE_STRIP                    "${TOOLCHAIN_PREFIX}ld")


#######################################################################
#наверное перед PROJECT SETUP
set(CMAKE_C_STANDARD             "23")     #-std=gnu11
set(CMAKE_CXX_STANDARD           "23")     #-std=gnu++11 (дает using-алиас)
#set(CMAKE_CXX_STANDARD_REQUIRED  ON)       #требовать, ибо using-алиас с 11
#######################################################################
