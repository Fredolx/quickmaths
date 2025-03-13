cmake_minimum_required(VERSION 3.14)
project(quickmaths)

set(CMAKE_CXX_STANDARD 20) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(ONNX_RUNTIME_DIR "onnx")
set(ONNX_GENAI_DIR "onnx-genai")
set(ONNX_GENAI_LIB "${PROJECT_SOURCE_DIR}/${ONNX_GENAI_DIR}/lib/libonnxruntime-genai.dylib")
set(ONNX_RUNTIME_LIB "${PROJECT_SOURCE_DIR}/onnx/lib/libonnxruntime.dylib")

include_directories(
    ${PROJECT_SOURCE_DIR}/include 
    ${ONNX_RUNTIME_DIR}/include
    ${ONNX_GENAI_DIR}/include
    ${PROJECT_SOURCE_DIR}/exprtk
)

add_library(lib_quickmaths SHARED
    src/quickmaths.cpp
    src/ai.cpp
    include/quickmaths.hpp
    include/ai.hpp
)

target_include_directories(lib_quickmaths PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${ONNX_RUNTIME_DIR}/include
    ${ONNX_GENAI_DIR}/include
    ${PROJECT_SOURCE_DIR}/exprtk
)

# Link the library and ONNX Runtime to the test executable
target_link_libraries(lib_quickmaths PRIVATE 
    ${ONNX_RUNTIME_LIB}
    ${ONNX_GENAI_LIB}
)