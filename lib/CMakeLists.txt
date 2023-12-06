add_library(aoc)
target_sources(aoc PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/src/aoc_file.c
    ${CMAKE_SOURCE_DIR}/lib/src/aoc_str.c)
target_include_directories(aoc PRIVATE ${CMAKE_SOURCE_DIR}/lib/include)
