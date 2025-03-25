cmake_minimum_required(VERSION 3.10)

# Project name
project(DataGen)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable debug symbols
set(CMAKE_BUILD_TYPE Debug)

# Include directories (for headers)
include_directories(${CMAKE_SOURCE_DIR})

# Add the executable
add_executable(DataGenExec main.cpp details/DataGen.cpp)