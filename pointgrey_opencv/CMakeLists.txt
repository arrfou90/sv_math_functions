project(stereo_vision C CXX)
cmake_minimum_required(VERSION 2.8.3)
find_package(OpenCV  REQUIRED )
 

include_directories(${PROJECT_SOURCE_DIR}/include) 
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/lib)
include_directories ( sgm/)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")
add_definitions(-Dlinux=1) 
add_definitions(-Wdeprecated-declarations)
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
set(CMAKE_VERBOSE_MAKEFILE on)
 
set(SOURCES include/myObserver.h
            include/myObserver.cpp
            include/iitstereocam.h
            include/iitstereocam.cpp)
set(HEADERS include/VimbaCPP/Include/AncillaryData.h
            include/VimbaCPP/Include/BasicLockable.h
            include/VimbaCPP/Include/Camera.h
            include/VimbaCPP/Include/Camera.hpp
            include/VimbaCPP/Include/EnumEntry.h
            include/VimbaCPP/Include/EnumEntry.hpp
            include/VimbaCPP/Include/FeatureContainer.h
            include/VimbaCPP/Include/FeatureContainer.hpp
            include/VimbaCPP/Include/Feature.h
            include/VimbaCPP/Include/Feature.hpp
            include/VimbaCPP/Include/FileLogger.h
            include/VimbaCPP/Include/Frame.h
            include/VimbaCPP/Include/ICameraFactory.h
            include/VimbaCPP/Include/ICameraListObserver.h
            include/VimbaCPP/Include/IFeatureObserver.h
            include/VimbaCPP/Include/IFrameObserver.h
            include/VimbaCPP/Include/IInterfaceListObserver.h
            include/VimbaCPP/Include/Interface.h
            include/VimbaCPP/Include/Interface.hpp
            include/VimbaCPP/Include/IRegisterDevice.h
            include/VimbaCPP/Include/LoggerDefines.h
            include/VimbaCPP/Include/Mutex.h
            include/VimbaCPP/Include/SharedPointerDefines.h
            include/VimbaCPP/Include/SharedPointer.h
            include/VimbaCPP/Include/SharedPointer_impl.h
            include/VimbaCPP/Include/UserLoggerDefines.h
            include/VimbaCPP/Include/UserSharedPointerDefines.h
            include/VimbaCPP/Include/VimbaCPPCommon.h
            include/VimbaCPP/Include/VimbaCPP.h
            include/VimbaCPP/Include/VimbaSystem.h
            include/VimbaCPP/Include/VimbaSystem.hpp
            )

  add_executable(stereo_vision_disparity_online src/disparityTestOnline.cpp
                                               ${SOURCES}
                                               ${HEADERS})  
  target_link_libraries(stereo_vision_disparity_online ${PROJECT_SOURCE_DIR}/lib/libVimbaC.so
                                                      ${PROJECT_SOURCE_DIR}/lib/libVimbaCPP.so
                                                      ${OpenCV_LIBS} 
)
 
