cmake_minimum_required(VERSION 3.9)
project(lab5 C)

set(CMAKE_C_STANDARD 99)

add_executable(lab5 server1.c common.h client1.c server2.c client2.c task4.c)