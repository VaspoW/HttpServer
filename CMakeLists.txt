
cmake_minimum_required (VERSION 3.2.2)

project (MGCHmi)
add_definitions(-std=c++11)
include_directories (include/)
add_executable(app src/Main.cpp src/Configuration.cpp)
target_link_libraries(app pthread)


