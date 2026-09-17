add_library(VideoCapture SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/VideoCapture.cpp)

find_package(OpenCV 4 REQUIRED)

target_include_directories(
  VideoCapture PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/src/Application/Subsystems/Subsystem/include
  ${CMAKE_SOURCE_DIR}/src/Application/Subsystems/Logger/include
  ${CMAKE_SOURCE_DIR}/src/Application/Subsystems/EventDispatcher/include
)

target_link_libraries(VideoCapture PUBLIC ${OpenCV_LIBS})

target_link_libraries(VideoCapture PRIVATE Logger)
target_link_libraries(VideoCapture PRIVATE EventDispatcher)
