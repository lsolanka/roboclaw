cmake_minimum_required(VERSION 3.2.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)

add_executable(roboclaw_test_ui main.cpp roboclawmain.cpp)
target_link_libraries(roboclaw_test_ui Qt5::Widgets roboclaw)

