cmake_minimum_required(VERSION 3.10)
project(p0 C)

set(CMAKE_C_STANDARD 99)

include_directories(.)

add_executable(msh.X msh.c util.c jobs.c)
add_executable(psh.X psh.c util.c)
