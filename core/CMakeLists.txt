cmake_minimum_required(VERSION 3.22)
project(core)

add_library(core STATIC library.cpp)

# Наследуем настройки из корневого CMakeLists.txt
set_target_properties(core PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)