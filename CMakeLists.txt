# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name (just for identification)
project(assn3 LANGUAGES C)

# Set the C standard explicitly to C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Include directories for header files
include_directories(
        ${CMAKE_SOURCE_DIR}/assn3/assn-03-lagorg22
)

# Dummy target just for code suggestions (no actual executable)
add_library(dummy_target STATIC
        ${CMAKE_SOURCE_DIR}/assn3/assn-03-lagorg22/hashset.c
        ${CMAKE_SOURCE_DIR}/assn3/assn-03-lagorg22/streamtokenizer.c
        ${CMAKE_SOURCE_DIR}/assn3/assn-03-lagorg22/vector.c
        ${CMAKE_SOURCE_DIR}/assn3/assn-03-lagorg22/thesaurus-lookup.c
        ${CMAKE_SOURCE_DIR}/assn3/assn-03-lagorg22/bool.h
        ${CMAKE_SOURCE_DIR}/assn3/assn-03-lagorg22/hashset.h
        ${CMAKE_SOURCE_DIR}/assn3/assn-03-lagorg22/streamtokenizer.h
        ${CMAKE_SOURCE_DIR}/assn3/assn-03-lagorg22/vector.h
        assn3/assn-03-lagorg22/hashset.c
        "Paradigms Exams/2019/midterm_1/problems/rugby/rugby.c"
        "Paradigms Exams/2019/midterm_1/problems/rugby/tests/tests.c"
        "Paradigms Exams/2018/2018_midterm_3.c"
        practice/stack_gen.c
        practice/matrix.c
        practice/ppp.c
        practice/p2.c
        practice/mid.c
)
