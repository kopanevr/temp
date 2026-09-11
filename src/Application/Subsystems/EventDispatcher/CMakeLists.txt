add_library(EventDispatcher SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/EventDispatcher.cpp)

target_include_directories(
  EventDispatcher PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/src/Application/Subsystems/Subsystem/include
  ${CMAKE_SOURCE_DIR}/src/Application/Subsystems/Logger/include
  ${CMAKE_SOURCE_DIR}/src/Application/Utilities/BitField/include
)

target_link_libraries(EventDispatcher PRIVATE Logger)
