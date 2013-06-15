cmake_minimum_required(VERSION 2.8)

project(voxelquest)



set(Boost_INCLUDE_DIR /usr/local/boost_1_53_0)
set(Boost_LIBRARY_DIR /usr/local/boost_1_53_0/stage/lib)
find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

find_library(POCO_FOUNDATION PocoFoundation REQUIRED)
find_library(POCO_NET PocoNet REQUIRED)
find_library(POCO_UTIL PocoUtil REQUIRED)

#include_directories ("${PROJECT_SOURCE_DIR}/CompiledSource")
#add_subdirectory (CompiledSource)

add_executable(voxelquest ${PROJECT_SOURCE_DIR}/CompiledSource/main.cpp)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries(voxelquest ${Boost_LIBRARIES} ${POCO_FOUNDATION} ${POCO_NET} ${POCO_UTIL} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )