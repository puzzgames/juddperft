cmake_minimum_required(VERSION 3.20)
project(juddperft)

set(CMAKE_CXX_STANDARD 14)

add_executable(juddperft Juddperft.cpp diagnostics.cpp engine.cpp fen.cpp
        hashtable.cpp movegen.cpp search.cpp timemanage.cpp winboard.cpp)

target_link_libraries(juddperft pthread atomic)
