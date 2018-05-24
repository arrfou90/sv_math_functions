cmake_minimum_required (VERSION 2.6 FATAL_ERROR)

project  (realsense_grabber)  
find_package(PCL REQUIRED)
find_package( OpenCV REQUIRED )
find_package(Boost COMPONENTS system filesystem program_options REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})
 include(CheckCXXCompilerFlag)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
 
ADD_EXECUTABLE  (realsense_grabber  main.cpp )

TARGET_LINK_LIBRARIES (realsense_grabber   ${PCL_LIBRARIES} ${PCL_LIBRARIES} ${Boost_LIBRARIES} ${OpenCV_LIBS}  /usr/local/lib/librealsense.so)


