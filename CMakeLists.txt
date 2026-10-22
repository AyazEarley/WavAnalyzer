cmake_minimum_required(VERSION 3.15)
project(analyzer C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(analyzer main.c wav.c dsp.c)

if(NOT WIN32)
  target_link_libraries(analyzer PRIVATE m)
endif()

if(MSVC)
  target_compile_options(analyzer PRIVATE /W4)
  target_compile_definitions(analyzer PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
  target_compile_options(analyzer PRIVATE -Wall -Wextra)
endif()