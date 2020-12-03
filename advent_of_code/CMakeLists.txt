# CMakeList.txt : CMake project for advent_of_code, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

# Add source to this project's executable.
add_executable (day1_a "day1_a.cpp" "lib.cpp")
add_executable (day1_b "day1_b.cpp" "lib.cpp")
add_executable (day2_a "day2_a.cpp" "lib.cpp")
add_executable (day2_b "day2_b.cpp" "lib.cpp")

