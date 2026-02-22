cmake_minimum_required(VERSION 3.29)
project(Geoguess)

set(CMAKE_CXX_STANDARD 20)

set(SFML_ROOT "C:/Libraries/SFML-2.6.2")
set(SFML_DIR "${SFML_ROOT}/lib/cmake/SFML")

find_package(SFML 2.6 COMPONENTS system window graphics audio REQUIRED)

add_executable(Geoguess main.cpp)

target_link_libraries(Geoguess sfml-system sfml-window sfml-graphics sfml-audio)