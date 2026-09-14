cmake_minimum_required(VERSION 3.10)
project(Application
  VERSION 0.0.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Опции.

# Пользовательские опции имеют префикс USER_OPTION.

# option(USER_OPTION_ "" ON)

option(USER_OPTION_PROFILE_INFERENCE "Включить профилирование вывода." ON)
option(USER_OPTION_SHOW_MODEL_INFO "Включить вывод информации о модели при отладке." ON)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/Application)
