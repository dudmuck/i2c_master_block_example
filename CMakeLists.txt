
cmake_minimum_required(VERSION 2.6)
project (i2c_master)

list(APPEND test_srcs
    test.cpp
    i2c_master.cpp
    cmds.c
)

add_executable(test_i2c ${test_srcs})

include_directories("${PROJECT_BINARY_DIR}")

target_link_libraries(test_i2c wiringPi)

