cmake_minimum_required(VERSION 3.8)

project(socket LANGUAGES C)

add_library(socket STATIC)

if (MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DWINDOWS")
else (MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLINUX")
endif (MSVC)

target_sources(socket PRIVATE
    src/socket.c
)

target_include_directories(socket PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(socket
)
