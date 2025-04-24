# Target system is Linux, not the host OS
set(CMAKE_SYSTEM_NAME Linux)
# Target CPU architectre is ARM, not x86_64
set(CMAKE_SYSTEM_PROCESSOR arm)

# Compiler
set(CMAKE_C_COMPILER   arm-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnu-g++)

# Optional: define sysroot if you copy your RPi2 root filesystem (improves library compatibility)
# set(CMAKE_SYSROOT /path/to/raspberrypi/sysroot)


# Ensure CMake finds headers and libraries in the sysroot
# Host tools are used (because they can actually run)
# Target headers/libraries are used (because they match the Pi)

# Use host (PC) tools, not executables in Raspberry Pi sysroot.
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# Use libraries only from Raspberry Pi sysroot. Don't use host libraries.
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# Use headers only from Raspberry Pi sysroot. Don't check in host include paths.
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
