cmake_minimum_required(VERSION 3.10)
project(Application
  VERSION 0.0.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Опции.

option(USER_OPTION_ "Включить." OFF)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/Application)
