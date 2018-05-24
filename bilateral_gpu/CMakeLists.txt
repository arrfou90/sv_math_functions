project(bilateral_gpu C CXX)
cmake_minimum_required(VERSION 2.8.3)
find_package(OpenCV REQUIRED )


find_package(CUDA REQUIRED)
 include_directories("${CUDA_INCLUDE_DIRS}")
include_directories(${PROJECT_SOURCE_DIR}/src) 
include_directories("${CMAKE_SOURCE_DIR}/src" 
)
  
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")
add_definitions(-Dlinux=1) 
#add_definitions(-Wdeprecated-declarations)
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
  endif(COMMAND cmake_policy)

set(CUDA_USE_STATIC_CUDA_RUNTIME OFF) 
set(CMAKE_EXPORT_COMPILE_COMMANDS 1) 
set(CMAKE_VERBOSE_MAKEFILE On)

 #	LDFLAGS="-Wl,-rpath,$(CUDA_INSTALL_PATH)/lib64"
 #	CFLAGS += -DCUDA_SIFTGPU_ENABLED -I$(CUDA_INC_PATH) -L$(CUDA_LIB_PATH) $(LDFLAGS)
set(
    CUDA_NVCC_FLAGS
    ${CUDA_NVCC_FLAGS};
    -O3 -lineinfo  
                  -gencode=arch=compute_53,code=sm_53
                  -gencode=arch=compute_35,code=sm_35
                   --default-stream per-thread
    ) 
    
SET(LIB_TYPE STATIC) 
CUDA_ADD_LIBRARY(bilateral_gpuLib ${LIB_TYPE}  src/median_gpu.cu src/bilateral_gpu.cu  )
 
  add_executable(bilateral_gpu src/main.cpp  )  
 
  target_link_libraries(bilateral_gpu  ${OpenCV_LIBS} ${CUDA_LIBRARY} ${CUDA_CUDART_LIBRARY} bilateral_gpuLib )
 
