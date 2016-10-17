cmake_minimum_required (VERSION 2.8.4)

project (play C)

file (GLOB source_files src/*.c)

add_executable (play ${source_files})

#file (COPY "file.sdk" DESTINATION "build")

message(STATUS ${CURSES_INCLUDE_DIRS})
message(STATUS ${CURSES_LIBRARIES})

file (MAKE_DIRECTORY "build")
set (EXECUTABLE_OUTPUT_PATH "build")

include_directories (${CURSES_INCLUDE_DIRS})

target_link_libraries(play ncursesw)
