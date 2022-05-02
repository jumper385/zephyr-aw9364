# Copyright (c) 2022 XiNGRZ
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

list(APPEND ZEPHYR_EXTRA_MODULES ${CMAKE_SOURCE_DIR}/../aw9364)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(aw9364)

target_sources(app PRIVATE src/main.c)
