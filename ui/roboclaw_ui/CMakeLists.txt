cmake_minimum_required(VERSION 3.2.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets CONFIG REQUIRED)
# Caveat: linking against but not provided by Qt5::Widgets
find_package(PNG CONFIG REQUIRED)

add_executable(roboclaw_test_ui main.cpp roboclawmain.cpp)
target_link_libraries(roboclaw_test_ui roboclaw spdlog::spdlog Qt5::Widgets PNG::png)
