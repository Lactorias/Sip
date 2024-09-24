# CMakeLists.txt

# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(SipProject LANGUAGES CXX)

# Set the C++ standard (optional but recommended)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Add the executable by specifying the source file

add_compile_options(
  -Wall
  -Wextra
  -pedantic-errors
  -Wconversion
  -Wsign-conversion
  -Wdouble-promotion
  -Wcast-align
  -Wformat=2
  -Wuninitialized
  -Wnull-dereference
  -Wnon-virtual-dtor
  -Woverloaded-virtual
  -Wdeprecated-copy-dtor
  -Wold-style-cast
  -Wzero-as-null-pointer-constant
  -Wsuggest-override
  -fstack-protector-strong
)

add_library(token src/Token.hpp)
link_libraries(token)
set_target_properties(token PROPERTIES LINKER_LANGUAGE CXX)
add_library(sip src/Sip.hpp)
link_libraries(sip)
add_library(grammar src/Grammar.hpp)
link_libraries(grammar)
set_target_properties(grammar PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(sip PROPERTIES LINKER_LANGUAGE CXX)
add_executable(compile src/main.cpp)
add_executable(ast_generate tools/mainAST.cpp)






