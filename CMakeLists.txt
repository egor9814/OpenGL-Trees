cmake_minimum_required(VERSION 3.8)
project(OpenGL_Trees)

set(CMAKE_CXX_STANDARD 17)


find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(glfw3 3 REQUIRED)
find_package(Threads REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
include_directories(glext/include)

add_library(glext SHARED
        glext/canvas.cpp glext/include/GLext/canvas.hpp
        glext/canvas_opengl.cpp glext/include/GLext/canvas_opengl.hpp
        glext/key_event.cpp glext/include/GLext/key_event.hpp
        glext/component.cpp glext/include/GLext/component.hpp
        glext/drawables.cpp glext/include/GLext/drawables.hpp
        glext/random.cpp glext/include/GLext/random.hpp
        )
target_link_libraries(glext
        glfw
        ${OPENGL_LIBRARIES}
        ${FREETYPE_LIBRARIES}
        ${MATH_LIBRARY}
        ${GLEW_LIBRARY}
        ${GLUT_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT}
        )
macro(use_glext module)
    target_link_libraries(${module} glext)
endmacro()


set(SOURCE_FILES
        tree/main.cpp
        tree/tree.cpp tree/tree.hpp
        tree/binary_tree.cpp tree/binary_tree.hpp
        tree/search_tree.cpp tree/search_tree.hpp
        tree/config.cpp tree/config.hpp
        )
add_executable(tree ${SOURCE_FILES})
use_glext(tree)


set(SOURCE_FILES_BT
        btree/main.cpp
        btree/balanced_tree.cpp btree/balanced_tree.hpp
        )
add_executable(btree ${SOURCE_FILES_BT})
use_glext(btree)