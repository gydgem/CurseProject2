cmake_minimum_required(VERSION 3.22)
project(user)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(user main.cpp)

target_link_libraries(user
        PRIVATE
        Boost::boost
        Boost::system
        core
)