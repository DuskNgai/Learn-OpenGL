cmake_minimum_required(VERSION 3.24)

# Set C++ compiler flags before `project`
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_NAME "LearnOpenGL")
project(${PROJECT_NAME} LANGUAGES C CXX)

# Compiler flags
if(MSVC)
    add_compile_definitions("_CRT_SECURE_NO_WARNINGS")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive-")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP24")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wfatal-errors -Wall -Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter -Wno-unused-variable")
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")

# Dependencies and third libraries
add_subdirectory("Dusk")

list(APPEND LEARN_OPENGL_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/Dusk/include"
)

list(APPEND LEARN_OPENGL_LIBRARIES
    dusk
)

# The executable file
aux_source_directory("src" SOURCE_FILES)
aux_source_directory("src/light" SOURCE_FILES)
aux_source_directory("src/material" SOURCE_FILES)
add_executable(learngl ${SOURCE_FILES})
target_include_directories(learngl PRIVATE ${LEARN_OPENGL_INCLUDES})
target_link_libraries(learngl PRIVATE ${LEARN_OPENGL_LIBRARIES})
