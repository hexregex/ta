cmake_minimum_required (VERSION 2.8.4)

project (ta C)

file (GLOB source_files src/*.c)

#set (CMAKE_C_STANDARD 99)
#set (CMAKE_VERBOSE_MAKEFILE TRUE)

set (CMAKE_BUILD_TYPE Debug)
set (CMAKE_C_FLAGS "-Wall -Wextra -std=c99 -pedantic -lpthread -D_XOPEN_SOURCE=700")

message("${source_files}")
list (REMOVE_ITEM source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/termios_input.c)
message("${source_files}")

add_library (termios_input MODULE src/termios_input.c)

add_executable (ta ${source_files})

file (MAKE_DIRECTORY "build")
set (EXECUTABLE_OUTPUT_PATH "build")

target_link_libraries(ta avutil avformat avcodec ao swscale ncursesw ${CMAKE_DL_LIBS})
