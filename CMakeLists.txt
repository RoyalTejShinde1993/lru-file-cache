cmake_minimum_required(VERSION 3.10)
project(lru_file_cache)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -O2)

add_executable(lru_file_cache lru_cache.cpp)
