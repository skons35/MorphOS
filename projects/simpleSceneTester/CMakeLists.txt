# CMakeList.txt : CMake project for simpleSceneTester, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

#Adding display of platform infos :
message(STATUS "Platform :"  ${CMAKE_HOST_SYSTEM_NAME})     # for instance : MorphOS, Linux, ...
#message(STATUS "System   :"  ${CMAKE_SYSTEM})              # for instance : MorphOS-3.13 , ...

# add Linux/Windows specific FreeGlut specific lookup
if(NOT "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "MorphOS")
	# add Windows & Linux lookup for Freeglut package :
	FIND_PACKAGE(GLUT REQUIRED)  # FindGLUT.cmake is going to look for NAMES : glut glut32 freeglut
	include_directories( ${GLUT_INCLUDE_DIR} )
else(NOT "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "MorphOS")
	# for Morphos, Tweaking gl glut related dependencies
	#target_compile_options(-noixemul)
endif(NOT "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "MorphOS")

set(EXTRA_LIBRARIES "")
# extra libs required for Linux
if("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Linux")
	set(EXTRA_LIBRARIES ${EXTRA_LIBRARIES} "GL" "GLU") #${DEP_LIBRARIES} GL GLU)
endif("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Linux")

#message(STATUS "GLUT_LIBRARIES : "  ${GLUT_LIBRARIES}) 
#message(STATUS "EXTRA_LIBRARIES : "  ${EXTRA_LIBRARIES})  

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/simpleSceneTester/code/dep/inc)

FILE(GLOB sources ${CMAKE_SOURCE_DIR}/simpleSceneTester/code/dep/src/*.cpp)
ADD_LIBRARY(sceneTestsLib ${sources})

# main prog : OBJ loader; including MTL and BMP loading if present
ADD_EXECUTABLE(sceneTester ${CMAKE_SOURCE_DIR}/simpleSceneTester/code/src/sceneTester.cpp)

if(NOT "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "MorphOS")
	target_link_libraries(sceneTester sceneTestsLib ${GLUT_LIBRARIES} ${EXTRA_LIBRARIES})
else(NOT "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "MorphOS")
	SET_TARGET_PROPERTIES(sceneTester PROPERTIES LINK_FLAGS -noixemul)
	target_link_libraries(sceneTester sceneTestsLib m Gl glut)
endif(NOT "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "MorphOS")

# Provides also some extra tests : dedicated code for file load of MTL and BMP
ADD_EXECUTABLE(mtlLoadTester ${CMAKE_SOURCE_DIR}/simpleSceneTester/code/src/mtlLoadTester.cpp)
target_link_libraries(mtlLoadTester sceneTestsLib) 

ADD_EXECUTABLE(bmpLoadTester ${CMAKE_SOURCE_DIR}/simpleSceneTester/code/src/bmpLoadTester.cpp)
target_link_libraries(bmpLoadTester sceneTestsLib) 

# install part related :

SET_PROPERTY(TARGET sceneTester
  PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  )
 
INSTALL(TARGETS sceneTestsLib DESTINATION lib)
INSTALL(TARGETS sceneTester DESTINATION bin)

# could also copy an EXISITNG 'run' folder into destination folder : ('run' must exist, or cmake error)
#INSTALL(DIRECTORY run DESTINATION ${CMAKE_BINARY_DIR}/run)
