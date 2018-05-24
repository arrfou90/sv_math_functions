project(stereo_vision C CXX)
cmake_minimum_required(VERSION 2.8.3)
find_package(OpenCV REQUIRED )
 
add_executable(image_filters main.cpp )  
target_link_libraries(image_filters ${OpenCV_LIBS})

