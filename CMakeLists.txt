cmake_minimum_required(VERSION 3.21)
project(gdf)

add_subdirectory(raylib)
add_subdirectory(tomlc17)
add_subdirectory(gdf_int)

add_executable(gdf)

add_subdirectory(frontend)

target_compile_options(gdf PUBLIC "-Wall" "-Wextra" "-Werror" "$<$<CONFIG:RELEASE>:-O3>")

target_link_libraries(gdf PUBLIC raylib)
target_link_libraries(gdf PUBLIC tomlc17)
target_link_libraries(gdf PUBLIC gdf_int)

set(RESOURCES_SRC "${CMAKE_SOURCE_DIR}/resources")
set(RESOURCES_DST "${CMAKE_CURRENT_BINARY_DIR}/resources")
add_custom_command(
 TARGET gdf POST_BUILD
 COMMAND ${CMAKE_COMMAND} -E create_symlink ${RESOURCES_SRC} ${RESOURCES_DST} 
 DEPENDS ${destination}
 COMMENT "symbolic link resources folder from ${RESOURCES_SRC} => ${RESOURCES_DST}"
)