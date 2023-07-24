cmake_minimum_required(VERSION 3.8)

project(socket LANGUAGES C)

add_library(socket STATIC)

target_sources(socket PRIVATE
    src/socket.c
)

target_include_directories(socket PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/../string/include
)

target_link_libraries(socket
)
