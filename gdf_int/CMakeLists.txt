cmake_minimum_required(VERSION 3.21)
project(gdf_int LANGUAGES C)

add_library(gdf_int STATIC)
target_include_directories(gdf_int PUBLIC .)
set_target_properties(gdf_int PROPERTIES C_STANDARD 23)

# Enable strict compilation
if(MSVC)
    add_compile_options("/W4" "$<$<CONFIG:RELEASE>:/O2>")
else()
    add_compile_options("-Wall" "-Wextra" "-Werror" "$<$<CONFIG:RELEASE>:-O3>")
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options("-stdlib=libc++")
    else()
        # nothing special for gcc at the moment
    endif()
endif()

add_subdirectory(inc)
add_subdirectory(src)