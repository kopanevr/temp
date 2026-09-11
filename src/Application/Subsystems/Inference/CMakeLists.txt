add_library(Inference SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/Inference.cpp)

target_include_directories(
  Inference PUBLIC
  ${CMAKE_SOURCE_DIR}/thirdParty/onnxruntime-linux-x64-1.29.1/include
)

target_include_directories(
  Inference PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/src/Application/Subsystems/Subsystem/include
  ${CMAKE_SOURCE_DIR}/src/Application/Subsystems/Logger/include
  ${CMAKE_SOURCE_DIR}/src/Application/Subsystems/EventDispatcher/include
)

target_link_libraries(Inference PRIVATE ${CMAKE_SOURCE_DIR}/thirdParty/onnxruntime-linux-x64-1.29.1/lib/libonnxruntime.so)

target_link_libraries(Inference PRIVATE Logger)
target_link_libraries(Inference PRIVATE EventDispatcher)

if (USER_OPTION_ENABLE_PROFILING)
  target_compile_definitions(Inference PRIVATE USER_OPTION_ENABLE_PROFILING)
endif()
