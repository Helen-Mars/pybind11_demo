cmake_minimum_required(VERSION 3.4...3.18)
project(example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set (Python_ROOT_DIR "D:/ProgramX86/anaconda")
set(SOURCE_FILES 
    src/example.cpp
    src/example_class.cpp
    src/type1.cpp
    src/main.cpp
)

set(HEADERS_FILES 
    include/example.h
)


add_subdirectory(pybind11-master)
include_directories(include)

# find_package(pybind11-master)

# add_executable(example ${SOURCE_FILES} ${HEADERS_FILES})

pybind11_add_module(example ${SOURCE_FILES} ${HEADERS_FILES})

target_link_libraries(example PRIVATE pybind11::embed)




