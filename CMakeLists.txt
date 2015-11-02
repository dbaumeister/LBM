cmake_minimum_required(VERSION 3.2)
project(LBM)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(LBM ${SOURCE_FILES} vectormath/Vector.h gui/GUI.h gui/GUI.cpp scene/Grid.h vectormath/Vector.cpp scene/Grid.cpp general/Definitions.h lbm/SimpleD2Q9.h lbm/SimpleD2Q9.cpp lbm/SwapD2Q9.h lbm/SwapD2Q9.cpp lbm/BlockD2Q9.h lbm/BlockD2Q9.cpp lbm/D2Q9.h lbm/D2Q9.cpp)

add_subdirectory(GLFW/glfw-3.1.1)
include_directories(GLFW/glfw-3.1.1/include)
target_link_libraries(LBM  glfw ${GLFW_LIBRARIES})
target_link_libraries(LBM glfw ${OPENGL_glu_LIBRARY} ${GLFW_LIBRARIES})