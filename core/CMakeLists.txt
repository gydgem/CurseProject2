project(core)

add_library(core STATIC
        base64.cpp
        base64.h
)

target_include_directories(core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(core PUBLIC
        Boost::boost  # Для хедеров Boost
)