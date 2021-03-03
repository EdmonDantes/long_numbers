cmake_minimum_required(VERSION 3.16)
project(long_numbers)

set(CMAKE_CXX_STANDARD 14)

if(LINUX_X64)
    add_definitions(-DLINUX_X64)
endif(LINUX_X64)
if (LINUX_X32)
    add_definitions(-DLINUX_X32)
endif(LINUX_X32)

add_executable(long_numbers test/unit_test.cpp library/common.h library/bit_stream.h library/bit_stream.cpp library/size_util.h library/array_util.h library/convert_util.h library/convert_util.cpp library/number_system_converter.h library/number_system_converter.cpp library/bit_stream_util.h library/bit_stream_util.cpp library/exception.h library/big_integer.h library/big_integer.cpp)