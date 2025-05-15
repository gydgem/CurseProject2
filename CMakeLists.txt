cmake_minimum_required(VERSION 3.22)
project(network_app)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED COMPONENTS system)

add_subdirectory(core)

add_subdirectory(server)
add_subdirectory(user)