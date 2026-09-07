add_executable(
  Application
  Application.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/CommandInterpreter/src/CommandInterpreter.cpp
  main.cpp
)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/Application/Subsystems/Logger)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/Application/Subsystems/EventDispatcher)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/Application/Subsystems/SubsystemManager)

target_include_directories(
  Application PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/ApplicationContext/include
  ${CMAKE_CURRENT_SOURCE_DIR}/CommandInterpreter/include
  ${CMAKE_CURRENT_SOURCE_DIR}/Subsystems/SubsystemManager/include
  ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/BitField/include
  ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/TimerManager/include
)

target_link_libraries(
  Application PRIVATE
  EventDispatcher
  SubsystemManager
)

add_custom_command(
  TARGET Application
  POST_BUILD
  COMMAND size Application --format=gnu
  COMMENT "Размеры секций:"
)
