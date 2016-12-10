cmake_minimum_required (VERSION 2.8.4)

project (ta C)

file (GLOB source_files src/*.c)

set (CMAKE_BUILD_TYPE Debug)
#set (CMAKE_C_STANDARD 99)
#set (CMAKE_C_FLAGS "-Wall -Wextra -std=c99")
#set (CMAKE_C_FLAGS "-Wall -std=c99")
set (CMAKE_C_FLAGS "-Wall -Wextra -std=c99 -pedantic -lpthread -D_XOPEN_SOURCE=700")
#set (CMAKE_C_FLAGS "-std=c99")
#set (CMAKE_VERBOSE_MAKEFILE TRUE)

message("${source_files}")
list (REMOVE_ITEM source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/termios_input.c)
message("${source_files}")


add_library (termios_input MODULE src/termios_input.c)

add_executable (ta ${source_files})

#file (COPY "file.sdk" DESTINATION "build")

#message(STATUS ${CURSES_INCLUDE_DIRS})
#message(STATUS ${CURSES_LIBRARIES})

file (MAKE_DIRECTORY "build")
set (EXECUTABLE_OUTPUT_PATH "build")

#include_directories (${CURSES_INCLUDE_DIRS})

# History
#target_link_libraries(ta ncursesw avutil avformat avcodec SDL ao pulse-simple swscale ${CMAKE_DL_LIBS})
target_link_libraries(ta avutil avformat avcodec ao swscale ncursesw ${CMAKE_DL_LIBS})
