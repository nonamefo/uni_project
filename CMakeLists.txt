cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project(main)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Список ресурсов для копирования
# set(RESOURCES ../src/crypto/list_algoritms.json)

file(GLOB_RECURSE SOURCE_FILES
  src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

include_directories(
  include
  src/libs
  src/libs/sql
)