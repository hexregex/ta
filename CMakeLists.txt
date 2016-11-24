cmake_minimum_required (VERSION 2.8.4)

project (play C)

file (GLOB source_files src/*.c)

set (CMAKE_BUILD_TYPE Debug)
set (CMAKE_C_STANDARD 90)

message("${source_files}")
list (REMOVE_ITEM source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/termios_input.c)
message("${source_files}")


add_library (termios_input MODULE src/termios_input.c)

add_executable (play ${source_files})

#file (COPY "file.sdk" DESTINATION "build")

#message(STATUS ${CURSES_INCLUDE_DIRS})
#message(STATUS ${CURSES_LIBRARIES})

file (MAKE_DIRECTORY "build")
set (EXECUTABLE_OUTPUT_PATH "build")

#include_directories (${CURSES_INCLUDE_DIRS})

target_link_libraries(play ncursesw avutil avformat avcodec SDL ao pulse-simple ${CMAKE_DL_LIBS})
