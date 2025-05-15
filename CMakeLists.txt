cmake_minimum_required(VERSION 3.22)
project(network_app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Найти Boost для всего проекта
find_package(Boost 1.87.0 REQUIRED COMPONENTS filesystem system process)

add_subdirectory(core)
add_subdirectory(server)
add_subdirectory(user)