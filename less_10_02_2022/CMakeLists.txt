cmake_minimum_required(VERSION 3.16)
project(less_10_02_2022 C)

set(CMAKE_C_STANDARD 11)

add_executable(Client client.c)
add_executable(Server server.c)
