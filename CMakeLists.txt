cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCMAKE_OSX_ARCHITECTURES=arm64")
project(NBodySimulator)
set(CMAKE_CXX_STANDARD 23)

if (APPLE)
    set(CMAKE_CXX_FLAGS "-framework Cocoa -framework IOKit -framework OpenGL -framework CoreVideo")
    add_compile_definitions(GL_SILENCE_DEPRECATION)
endif ()

include_directories(
        ${PROJECT_SOURCE_DIR}/include)
link_directories(
        ${PROJECT_SOURCE_DIR}/lib)

set(SOURCES
        src/NBodySimulator.h
        src/shader.h
        src/triangle.h
        src/rectangle.h
        src/shape.h
        src/camera.h
)
add_executable(NBodySimulator src/main.cpp
        src/sphere.h
        src/world.h
        src/world.h)

target_link_libraries(NBodySimulator
        libglfw3.a)