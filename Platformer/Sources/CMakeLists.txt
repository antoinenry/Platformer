
cmake_minimum_required(VERSION 2.6)

project(Plateformer)

file(
GLOB
source_files
*.cpp *.hpp shader* *.c *.h
)

SET(CMAKE_BUILD_TYPE Release)
SET(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter -D_REENTRANT -fPIC" )
SET(CMAKE_CXX_COMPILER "g++")



add_executable(	
   executable
   ${source_files}
   )	

target_link_libraries(executable "-lm -lglut -lGLEW -lGLU -lGL -ldl")


