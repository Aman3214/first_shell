# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Define the project name and programming language
project(MyCustomShell C)

# Set the C standard to C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Enable compiler warnings and add debugging symbols (-g flag)
add_compile_options(-Wall -Wextra -g)

# Define the executable name and list all source files required to build it
add_executable(myshell 
    main.c 
    commands.c
)