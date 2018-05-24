project(disToPCDoffline C CXX)
cmake_minimum_required(VERSION 2.8.3)
find_package(OpenCV  REQUIRED ) 
find_package(PCL  REQUIRED COMPONENTS common io visualization)

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})
 

add_executable(disToPCDoffline main.cpp )  
target_link_libraries(disToPCDoffline  ${OpenCV_LIBS} ${PCL_LIBRARIES}  )
 

