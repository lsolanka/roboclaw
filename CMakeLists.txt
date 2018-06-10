cmake_minimum_required(VERSION 3.2.0)
cmake_policy(SET CMP0048 NEW)

include(cmake/HunterGate.cmake)
HunterGate(
    URL "https://github.com/lsolanka/hunter/archive/v0.22.4-rosdevel7.tar.gz"
    SHA1 "3d48fff4150ca3b0a9e19e6a095282e2636e22a4"
)

project(roboclaw VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_UI "Include the QT Test UI in the build" OFF)

hunter_add_package(Boost COMPONENTS program_options system thread)
hunter_add_package(spdlog)

find_package(spdlog CONFIG REQUIRED)

add_subdirectory(roboclaw)
if (BUILD_UI)
    # For fonts, use e.g. QT_QPA_FONTDIR=<path_to_hunter>/.hunter/_Base/3d48fff/51e9eea/70120ea/Install/lib/fonts
    # Otherwise they will not be displayed
    hunter_add_package(Qt)
    hunter_add_package(PNG)
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
