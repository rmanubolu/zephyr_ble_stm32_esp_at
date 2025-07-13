#minimum Cmake verison
cmake_minimum_required(VERSION 3.20.0)

# find the zephyr base dir
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

# project name
project(zephyr_ble_stm32_esp_at)

# Project target files
target_sources(app PRIVATE src/main.c)