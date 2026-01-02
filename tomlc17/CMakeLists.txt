cmake_minimum_required(VERSION 3.21)
project(tomlc17 LANGUAGES C)

add_library(tomlc17 STATIC src/tomlc17.c)
target_include_directories(tomlc17 PUBLIC src/)