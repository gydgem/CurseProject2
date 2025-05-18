project(client)

add_executable(client
        main.cpp
        Client.cpp
        Client.h
        UserCommandHandler.cpp
        UserCommandHandler.h
)

target_include_directories(client PRIVATE
        ${CMAKE_SOURCE_DIR}/core
)

target_link_libraries(client PRIVATE
        core
        Boost::system
)