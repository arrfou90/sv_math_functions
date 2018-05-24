cmake_minimum_required(VERSION 3.5.1)
project(depth_rgb_ToPointCloud)



set(CMAKE_CXX_STANDARD 11)
ADD_DEFINITIONS( -DUSE_OPENACC )
MESSAGE( "Using OpenACC" )


set(SOURCE_FILES main.cpp)
find_package(OpenCV   REQUIRED )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(PCL  REQUIRED COMPONENTS common io visualization)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})


add_executable(depth_rgb_ToPointCloud ${SOURCE_FILES})
target_link_libraries(depth_rgb_ToPointCloud  ${OpenCV_LIBS}  ${PCL_LIBRARIES} )



