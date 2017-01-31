cmake_minimum_required (VERSION 2.8)
cmake_policy(VERSION 2.8)

project (oglapp)

# GLEW
set(glew-cmake_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(glew-cmake_BUILD_MULTI_CONTEXT OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glew)
include_directories(lib/glew/include lib/glew/src)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)
include_directories(lib/glfw/include)

# GLM
include_directories(lib/glm)

# SOIL
add_subdirectory(lib/soil)
include_directories(lib/soil/inc/SOIL)

file(GLOB src_files
    ${PROJECT_SOURCE_DIR}/src/*
    ${PROJECT_SOURCE_DIR}/src/common/*)

add_executable(app ${src_files})
target_link_libraries(app ${GLFW_LIBRARIES} libglew_static glfw soil)
