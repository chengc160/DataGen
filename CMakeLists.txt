cmake_minimum_required(VERSION 3.10)
project(DataGenProject)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure Debugging Symbols (-g)
set(CMAKE_BUILD_TYPE Debug)

# Create executable from source files
add_executable(DataGen
    main.cpp
    details/DataGen.cpp
)

# Include the root directory so it can find "DataGen.h"
target_include_directories(DataGen PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)