project (main_project)
cmake_minimum_required(VERSION 2.8.9)

file(GLOB sources *.cpp *.h)

set(CMAKE_CXX_COMPILER /usr/bin/g++)
set (SFML /home/wubmans/Downloads/SFML-2.5.1/)

link_directories(${SFML}/lib)
add_executable(main ${sources})

target_include_directories(main PUBLIC ${SFML}/include)

target_link_libraries(main sfml-graphics sfml-window sfml-system)
