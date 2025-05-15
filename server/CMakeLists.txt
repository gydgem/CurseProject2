project(server)

add_executable(server
        main.cpp
        Server.cpp Server.h
        Session.cpp Session.h
        Workspace.cpp Workspace.h
)

target_include_directories(server PRIVATE
        ${CMAKE_SOURCE_DIR}/core
)

target_link_libraries(server PRIVATE
        core
        Boost::filesystem
        Boost::system
        Boost::process
)