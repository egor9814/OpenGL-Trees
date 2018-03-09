cmake_minimum_required(VERSION 3.8)
project(OpenGL_Trees)

set(CMAKE_CXX_STANDARD 17)


find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(glfw3 3 REQUIRED)
find_package(Threads REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

set(SOURCE_FILES
        main.cpp
        tree.cpp tree.hpp
        canvas.cpp canvas.hpp
        canvas_opengl.cpp canvas_opengl.hpp
        key_event.cpp key_event.hpp
        component.cpp component.hpp
        drawables.cpp drawables.hpp
        binary_tree.cpp binary_tree.hpp
        random.cpp random.hpp
        search_tree.cpp search_tree.hpp
        config.cpp config.hpp
        )
add_executable(trees ${SOURCE_FILES})

target_link_libraries(trees
        glfw
        ${OPENGL_LIBRARIES}
        ${FREETYPE_LIBRARIES}
        ${MATH_LIBRARY}
        ${GLEW_LIBRARY}
        ${GLUT_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT}
        )