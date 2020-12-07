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
add_executable (day3_a "day3_a.cpp" "lib.cpp")
add_executable (day3_b "day3_b.cpp" "lib.cpp")
add_executable (day4_a "day4_a.cpp" "lib.cpp")
add_executable (day4_b "day4_b.cpp" "lib.cpp")
add_executable (day5_a "day5_a.cpp" "lib.cpp")
add_executable (day5_b "day5_b.cpp" "lib.cpp")
add_executable (day6 "day6.cpp" "lib.cpp")

