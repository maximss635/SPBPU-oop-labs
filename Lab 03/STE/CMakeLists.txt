cmake_minimum_required(VERSION 3.17)
project(STE)

set(CMAKE_CXX_STANDARD 17)

set(CURSES_NEED_NCURSES TRUE)
add_executable(STE main.cpp View.cpp View.h ArgParser.h Journal.h MyString.cpp MyString.h MyStringAdditions.h Kernel.cpp Kernel.h Controller.cpp Controller.h)

target_link_libraries(${PROJECT_NAME} ncurses)
