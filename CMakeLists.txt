cmake_minimum_required(VERSION 3.2)
project(Graphics)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(Graphics ${SOURCE_FILES} VectorMath/Vector.h GUI/GUI.h GUI/GUI.cpp FluidSolver/FluidSolver.h FluidSolver/FluidSolver.cpp Scene/Grid.h Scene/Scene.h VectorMath/Vector.cpp Scene/Grid.cpp Scene/Scene.cpp General/Definitions.h)

add_subdirectory(GLFW/glfw-3.1.1)
include_directories(GLFW/glfw-3.1.1/include)
target_link_libraries(Graphics glfw ${GLFW_LIBRARIES})
target_link_libraries(Graphics glfw ${OPENGL_glu_LIBRARY} ${GLFW_LIBRARIES})