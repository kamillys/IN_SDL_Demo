project(SDL_Input_Demo)
cmake_minimum_required(VERSION 2.8)

# Supported build types:
#  Debug
#  Release
#  RelWithDebInfo
#
# to set build type: -DCMAKE_BUILD_TYPE=...

find_package(PkgConfig)

#set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(PROJECT_SOURCES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Sources")

#Add system-related configure #unused
if(WIN32)
    # Windows specific code
    # set(OperatingSystem "Windows")
elseif(APPLE)
    # Mac OS X specific code
    # set(OperatingSystem "Mac OS X")
    message( FATAL_ERROR "Apple platform is not yet supported. CMake will exit." )
elseif(UNIX)
    # Linux specific code
    # set(OperatingSystem "Linux")
else()
    message( FATAL_ERROR "Unknown platform '${CMAKE_SYSTEM_NAME}'! CMake will exit." )
endif()

# Enable C++11
list( APPEND CMAKE_CXX_FLAGS "-std=c++0x" )

pkg_check_modules(SDL2 REQUIRED sdl2 gl glu glew)
set(INCLUDEDIRS ${INCLUDEDIRS} ${SDL2_INCLUDE_DIRS})
set(PROJECTLIBS ${PROJECTLIBS} ${SDL2_LIBRARIES} )

#if(USE_PTHREADS)
#    set(PROJECTLIBS ${PROJECTLIBS} pthread)
#endif(USE_PTHREADS)

#Add include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Sources)
include_directories(${INCLUDEDIRS})

##Project sources
file(GLOB_RECURSE SRC_LIST ${PROJECT_SOURCES_PATH}/*)

#Executable
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${PROJECTLIBS})
