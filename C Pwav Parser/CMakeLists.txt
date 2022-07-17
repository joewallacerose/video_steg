cmake_minimum_required(VERSION 3.22)
project(C_Pwav_Parser C)

set(CMAKE_C_STANDARD 99)

add_executable(C_Pwav_Parser main.c wave.c wave.h steg_write.c steg_write.h steg_read.c steg_read.h)
