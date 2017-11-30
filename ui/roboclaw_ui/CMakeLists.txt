cmake_minimum_required(VERSION 3.2.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Force system version, do not use HUNTER-generated variables
find_package(Qt5Widgets CONFIG REQUIRED NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_CMAKE_FIND_ROOT_PATH)

add_executable(roboclaw_test_ui main.cpp roboclawmain.cpp)
target_link_libraries(roboclaw_test_ui Qt5::Widgets roboclaw)

