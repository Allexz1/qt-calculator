cmake_minimum_required(VERSION 3.21)
project(qtcalculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

enable_testing()

add_subdirectory(engine)
add_subdirectory(app)
add_subdirectory(tests)
