# Check if gcc is installed
find_program(GCC_COMPILER gcc)
if(NOT GCC_COMPILER)
	message(FATAL_ERROR "gcc is not installed!")
endif()

# Check if g++ is installed
find_program(GXX_COMPILER g++)
if(NOT GXX_COMPILER)
	message(FATAL_ERROR "g++ (gcc-c++) is not installed!")
endif()
