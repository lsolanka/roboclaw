cmake_minimum_required(VERSION 3.2.0)
cmake_policy(SET CMP0048 NEW)

include(cmake/HunterGate.cmake)
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.19.161.tar.gz"
    SHA1 "9af482b6a54875bbbb221274f7290324dff29919"
)

project(roboclaw VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_UI "Include the QT Test UI in the build" OFF)

hunter_add_package(Boost COMPONENTS program_options system log filesystem
                                    thread)

add_subdirectory(roboclaw)
if (BUILD_UI)
    add_subdirectory(ui/roboclaw_ui)
endif()

# Packaging
include(CMakePackageConfigHelpers)

configure_file(cmake/${PROJECT_NAME}Config.cmake.in
               ${PROJECT_NAME}Config.cmake
               @ONLY)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    COMPATIBILITY ExactVersion)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION lib/cmake/${PROJECT_NAME})

install(EXPORT ${PROJECT_NAME}Targets
    DESTINATION lib/cmake/${PROJECT_NAME}
    NAMESPACE roboclaw::)
