add_library(Temp SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/Temp.cpp)

find_package(OpenCV 4 REQUIRED)

target_include_directories(
  Temp PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/src/Application/Subsystems/Subsystem/include
  ${CMAKE_SOURCE_DIR}/src/Application/Subsystems/Logger/include
  ${CMAKE_SOURCE_DIR}/src/Application/Subsystems/EventDispatcher/include
)

target_link_libraries(Temp PRIVATE ${OpenCV_LIBS})

target_link_libraries(Temp PRIVATE Logger)
target_link_libraries(Temp PRIVATE EventDispatcher)
