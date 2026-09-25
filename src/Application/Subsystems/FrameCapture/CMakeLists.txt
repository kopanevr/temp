add_library(FrameCapture SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/FrameCapture.cpp)

find_package(OpenCV 4 REQUIRED)

target_include_directories(
  FrameCapture PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/src/Application/Subsystems/Subsystem/include
  ${CMAKE_SOURCE_DIR}/src/Application/Subsystems/Logger/include
  ${CMAKE_SOURCE_DIR}/src/Application/Subsystems/EventDispatcher/include
)

target_link_libraries(FrameCapture PUBLIC ${OpenCV_LIBS})

target_link_libraries(FrameCapture PRIVATE Logger)
target_link_libraries(FrameCapture PRIVATE EventDispatcher)
