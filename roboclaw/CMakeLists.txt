find_package(Boost CONFIG REQUIRED COMPONENTS program_options system thread)

set(WRITE_CMD_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/roboclaw/io/write_commands)
set(READ_CMD_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/roboclaw/io/read_commands)

set (ROBOCLAW_SRC_FILES
    ${WRITE_CMD_SRC_DIR}/motor_velocity_pid.cpp
    ${WRITE_CMD_SRC_DIR}/drive_duty.cpp
    ${READ_CMD_SRC_DIR}/status.cpp
    ${READ_CMD_SRC_DIR}/encoder_count.cpp
    ${READ_CMD_SRC_DIR}/encoder_mode.cpp
    ${READ_CMD_SRC_DIR}/encoder_speed.cpp
    ${READ_CMD_SRC_DIR}/firmware_version.cpp
    ${READ_CMD_SRC_DIR}/motor_instantaneous_speed.cpp
    ${READ_CMD_SRC_DIR}/motor_pwm_values.cpp
    ${READ_CMD_SRC_DIR}/motor_velocity_pid.cpp
)
add_library(roboclaw ${ROBOCLAW_SRC_FILES})
target_link_libraries(roboclaw
    spdlog::spdlog
    Boost::system Boost::program_options Boost::system Boost::thread)
target_include_directories(roboclaw PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_executable(${PROJECT_NAME}_test src/main.cpp)
target_link_libraries(${PROJECT_NAME}_test roboclaw)

add_executable(read_serial src/read_serial.cpp)
target_link_libraries(read_serial Boost::system Boost::thread)

install(TARGETS ${PROJECT_NAME}_test read_serial DESTINATION bin/)

install(TARGETS roboclaw
    EXPORT roboclawTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/roboclaw/
    DESTINATION include/roboclaw
    FILES_MATCHING PATTERN "*.hpp")
