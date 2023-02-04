cmake_minimum_required(VERSION 3.3)
project(lab4)

set(CMAKE_CXX_STANDARD 11)

set(MAIN main.cpp)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})
include_directories(include)
include_directories(src)

add_subdirectory(glad/)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

set(SOURCES 
    "src/common.cpp"
    "src/cameras.cpp"
    "src/camera_free.cpp"
    "src/mesh.cpp"
    "src/shader.cpp"
    "src/app.cpp"
    )

add_executable(lab4 ${MAIN} ${SOURCES})
target_link_libraries(lab4 ${OPENGL_gl_LIBRARY} glfw)
target_link_libraries(lab4 glad ${CMAKE_DL_LIBS})