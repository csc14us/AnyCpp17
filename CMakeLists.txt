cmake_minimum_required(VERSION 3.15)
project(AnyDev)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(AnyDev
    src/utility/Any.cpp
    src/utility/impl/LargeAny.cpp
    src/utility/impl/SmallAny.cpp
        TestMain.cpp)