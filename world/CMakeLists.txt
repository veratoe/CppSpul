project(wubbel)

cmake_minimum_required(VERSION 3.0.0)

file(GLOB sources *.cpp *.h)

set (SFML "C:\\msys32\\spul\\SFML")

link_directories(${SFML}/lib)
add_executable(main ${sources})


target_include_directories(main PUBLIC ${SFML}/include)
target_link_libraries(main sfml-graphics sfml-window sfml-system)