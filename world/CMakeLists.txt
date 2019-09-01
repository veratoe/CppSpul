project(wubbel)

cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 11)

file(GLOB sources *.cpp *.h)

set (SFML "/usr/local/include/SFML-2.5.1")

link_directories(${SFML}/lib)
add_executable(main ${sources})


target_include_directories(main PUBLIC ${SFML}/include)
target_link_libraries(main sfml-graphics sfml-window sfml-system)
