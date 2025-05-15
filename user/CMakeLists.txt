project(user)

add_executable(user
        main.cpp
)

target_include_directories(user PRIVATE
        ${CMAKE_SOURCE_DIR}/core
)

target_link_libraries(user PRIVATE
        core
        Boost::filesystem
        Boost::system
        Boost::process
)