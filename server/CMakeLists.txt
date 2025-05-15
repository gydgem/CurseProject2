cmake_minimum_required(VERSION 3.22)
project(server)

find_package(Boost  REQUIRED COMPONENTS system)

add_executable(server main.cpp
        Server.cpp
        Server.h
        Session.cpp
        Session.h
        RequestHandler.cpp
        RequestHandler.h)

target_link_libraries(server
        PRIVATE
        Boost::boost
        Boost::system
        core
)