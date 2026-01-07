cmake_minimum_required(VERSION 3.21)
project(gdf_int LANGUAGES C)

add_library(gdf_int STATIC)
target_include_directories(gdf_int PUBLIC .)
set_target_properties(gdf_int PROPERTIES C_STANDARD 23)

target_compile_options(gdf_int PUBLIC "-Wall" "-Wextra" "-Werror" "$<$<CONFIG:RELEASE>:-O3>")

add_subdirectory(inc)
add_subdirectory(src)