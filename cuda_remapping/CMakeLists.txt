cmake_minimum_required(VERSION 3.6)
project(cuda_remapping)

set(CMAKE_CXX_STANDARD 11)
find_package(OpenCV   REQUIRED )
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")

set(SOURCE_FILES main.cpp)
add_executable(cuda_remapping ${SOURCE_FILES})
target_link_libraries(cuda_remapping  ${OpenCV_LIBS}  )



cmake_minimum_required(VERSION 3.5.1)


