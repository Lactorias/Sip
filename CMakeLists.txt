# CMakeLists.txt

# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(SipProject)

# Set the C++ standard (optional but recommended)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable by specifying the source file
add_executable(Sip Sip.cpp)
