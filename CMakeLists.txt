cmake_minimum_required(VERSION 3.22)

set( CMAKE_C_COMPILER "gcc")
set( CMAKE_CXX_COMPILER "g++ -l sqllite3")

project(main)

set(CMAKE_CXX_STANDARD 17)
file(GLOB_RECURSE SOURCE_FILES
  src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

include_directories(
  include
  src/libs
  src/libs/sql
)