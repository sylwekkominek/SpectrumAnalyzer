cmake_minimum_required(VERSION 3.5)
project(Glad)

add_library(glad include/glad/glad.h  src/glad.c)
target_include_directories(glad PUBLIC include/)
