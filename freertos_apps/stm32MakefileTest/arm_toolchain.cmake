include(CMakeForceCompiler)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_CROSSCOMPILING 1)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(PLATFORM_NAME "Nucleo")

set(CMAKE_SYSROOT /home/mego/microros_ws/firmware/freertos_apps/stm32MakefileTest/../..)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Makefile flags
set(CROSSDEV /home/mego/microros_ws/firmware/freertos_apps/stm32MakefileTest/../../toolchain/bin/arm-none-eabi-)
set(ARCH_CPU_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -DUSE_HAL_DRIVER -DSTM32F407xx -D_TIMEVAL_DEFINE -O0 -Wall -fdata-sections -ffunction-sections")
set(ARCH_OPT_FLAGS "")

# Compiler tools
foreach(tool gcc ld ar)
	string(TOUPPER ${tool} TOOL)
    find_program(${TOOL} ${CROSSDEV}${tool})
	if(NOT ${TOOL})
		message(FATAL_ERROR "could not find ${CROSSDEV}${tool}")
	endif()
endforeach()

set(CMAKE_C_COMPILER ${CROSSDEV}gcc)
set(CMAKE_CXX_COMPILER ${CROSSDEV}g++)

set(CMAKE_C_FLAGS_INIT "-std=c11 ${ARCH_CPU_FLAGS} ${ARCH_OPT_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_INIT "-std=c++14 ${ARCH_CPU_FLAGS} ${ARCH_OPT_FLAGS} " CACHE STRING "" FORCE)


include_directories(SYSTEM 
	/home/mego/microros_ws/firmware/freertos_apps/stm32MakefileTest/lib/FreeRTOS-Plus-POSIX/include
 /home/mego/microros_ws/firmware/freertos_apps/stm32MakefileTest/lib/include
 /home/mego/microros_ws/firmware/freertos_apps/stm32MakefileTest/lib/include/private
 /home/mego/microros_ws/firmware/freertos_apps/stm32MakefileTest/lib/include/FreeRTOS_POSIX
 /home/mego/microros_ws/firmware/freertos_apps/stm32MakefileTest/lib/include/FreeRTOS_POSIX/sys
 /home/mego/microros_ws/firmware/freertos_apps/stm32MakefileTest/Core/Inc
 /home/mego/microros_ws/firmware/freertos_apps/stm32MakefileTest/Drivers/STM32F4xx_HAL_Driver/Inc
 /home/mego/microros_ws/firmware/freertos_apps/stm32MakefileTest/Drivers/CMSIS/Device/ST/STM32F4xx/Include
 /home/mego/microros_ws/firmware/freertos_apps/stm32MakefileTest/Drivers/CMSIS/Include

    )
    
set(__BIG_ENDIAN__ 0)