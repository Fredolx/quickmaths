cmake_minimum_required(VERSION 3.14)
project(quickmaths)

set(CMAKE_CXX_STANDARD 20) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

link_directories($ENV{HOME}/.local/lib)

add_library(quickmaths SHARED
    src/quickmaths.cpp
    src/ai.cpp
)

target_include_directories(quickmaths PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/exprtk
    ${PROJECT_SOURCE_DIR}/llama/include
    ${PROJECT_SOURCE_DIR}/ggml/include
)

target_link_libraries(quickmaths PRIVATE 
    ggml-base
    ggml-blas
    ggml-cpu
    ggml-metal
    ggml
    llama
    llava_shared
    openblas
)

add_executable(quickmaths_test
    ${PROJECT_SOURCE_DIR}/tests/test.cpp
)

target_include_directories(quickmaths_test PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(quickmaths_test PRIVATE
    quickmaths
)