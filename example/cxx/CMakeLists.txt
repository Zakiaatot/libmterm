cmake_minimum_required(VERSION 3.2)
project(libmterm-example-cxx)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED 11)

link_directories(${PROJECT_SOURCE_DIR}/../../output/linux)

add_executable(test main.cpp)
target_link_libraries(test mterm)