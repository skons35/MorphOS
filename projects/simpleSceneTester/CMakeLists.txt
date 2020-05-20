# CMakeList.txt : CMake project for simpleSceneTester, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)


FIND_PACKAGE(GLUT REQUIRED)  # FindGLUT.cmake is going to look for NAMES : glut glut32 freeglut

include_directories( ${GLUT_INCLUDE_DIR} )

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/simpleSceneTester/code/dep/inc)

FILE(GLOB sources ${CMAKE_SOURCE_DIR}/simpleSceneTester/code/dep/src/*.cpp)
ADD_LIBRARY(sceneTestsLib ${sources})

# main prog : OBJ loader; including MTL and BMP loading if present
ADD_EXECUTABLE(sceneTester ${CMAKE_SOURCE_DIR}/simpleSceneTester/code/src/sceneTester.cpp)
target_link_libraries(sceneTester sceneTestsLib  ${GLUT_LIBRARIES})

# Provides also some extra tests : dedicated code for file load of MTL and BMP
ADD_EXECUTABLE(mtlLoadTester ${CMAKE_SOURCE_DIR}/simpleSceneTester/code/src/mtlLoadTester.cpp)
target_link_libraries(mtlLoadTester sceneTestsLib) #  ${GLUT_LIBRARIES})

ADD_EXECUTABLE(bmpLoadTester ${CMAKE_SOURCE_DIR}/simpleSceneTester/code/src/bmpLoadTester.cpp)
target_link_libraries(bmpLoadTester sceneTestsLib) #  ${GLUT_LIBRARIES})

# install part related :

SET_PROPERTY(TARGET sceneTester
  PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  )
 
INSTALL(TARGETS sceneTestsLib DESTINATION lib)
INSTALL(TARGETS sceneTester DESTINATION bin)

# could also copy an EXISITNG 'run' folder into destination folder : ('run' must exist, or cmake error)
#INSTALL(DIRECTORY run DESTINATION ${CMAKE_BINARY_DIR}/run)