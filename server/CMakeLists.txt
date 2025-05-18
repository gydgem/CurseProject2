project(server)

add_executable(server
        main.cpp
        Server.cpp
        Server.h
        Session.cpp
        Session.h
        Workspace.cpp
        Workspace.h
        ZoneManager.cpp
        ZoneManager.h
        command_handlers/ServerCommandHandler.cpp
        command_handlers/ServerCommandHandler.h
        command_handlers/SessionCommandHandler.cpp
        command_handlers/SessionCommandHandler.h
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