cmake_minimum_required(VERSION 3.28)
project(A)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

add_executable(Aa intersecto.h a_sofa.cpp)
add_executable(Ab intersecto.h b_sofa.cpp)

if(MSVC)
    foreach(target Aa Ab)
        target_compile_options(${target} PRIVATE /O2 /fp:fast)
    endforeach()
else()
    foreach(target Aa Ab)
        target_compile_options(${target} PRIVATE -O3 -march=native -ffast-math)
    endforeach()
endif()
