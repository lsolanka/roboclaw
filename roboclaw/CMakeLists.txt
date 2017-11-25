cmake_minimum_required(VERSION 3.2.0)
cmake_policy(SET CMP0048 NEW)

include(cmake/HunterGate.cmake)
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.19.161.tar.gz"
    SHA1 "9af482b6a54875bbbb221274f7290324dff29919"
)

project(roboclaw VERSION 0.1.0 LANGUAGES C CXX)

hunter_add_package(Boost COMPONENTS program_options system log)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost CONFIG REQUIRED
    COMPONENTS program_options system log filesystem thread)

include_directories(include)

add_executable(${PROJECT_NAME}_test src/main.cpp)
target_link_libraries(${PROJECT_NAME}_test
    Boost::system Boost::program_options Boost::log Boost::filesystem Boost::system Boost::thread)
