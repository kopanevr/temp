add_library(temp SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/temp.cpp)

target_include_directories(
  temp PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/src/Application/Subsystems/Subsystem/include
  ${CMAKE_SOURCE_DIR}/src/Application/Subsystems/Logger/include
)

target_link_libraries(temp PRIVATE Logger)
