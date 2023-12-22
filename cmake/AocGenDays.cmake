function (AOC_GENERATE YEAR DAY)
    add_executable(aoc_${YEAR}_${DAY} ${DAY}/aoc_${YEAR}_${DAY}.c)
    target_include_directories(aoc_${YEAR}_${DAY} PRIVATE ${CMAKE_SOURCE_DIR}/lib/include)
    target_link_libraries(aoc_${YEAR}_${DAY} aoc)

    # NOTE(lucas): Since the input.txt files are stored in the same folder as the
    # source for each day, the working directory also needs to be in the same folder.
    # NOTE(lucas): Place the VS solution for each day of the year into a solution folder
    # (only affects organization in Visual Studio).
    set_target_properties(aoc_${YEAR}_${DAY} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${YEAR}/${DAY}
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${DAY}
    FOLDER ${YEAR})

    if (MSVC)
        # /W4 enables the highest level of warnings, and /WX turns warnings into errors
        # /Oi Replace functions with intrinsics where possible to improve performance
        # /wd removes specified warnings.
        # C4101 and C4189 are about unused parameters/variables and are usually not particularly useful.
        target_compile_options(aoc_${YEAR}_${DAY} PRIVATE
                               /Oi /WX /W4
                               /wd4101 /wd4189 /D_CRT_SECURE_NO_WARNINGS)

        if (CMAKE_BUILD_TYPE STREQUAL Debug)
            # In debug mode, enable complete debug information (/Zi) and disable optimization (/Od)
            # /RTCcsu enables run-time error checking
            target_compile_options(aoc_${YEAR}_${DAY} PRIVATE /Zi /Od /RTCcsu)
        endif()
    endif()
endfunction()

function(AOC_GENERATE_YEAR AOC_YEAR)
    set(DAY_DIRECTORIES 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25)
    set(custom_target_args)
    foreach (DIR ${DAY_DIRECTORIES})
        if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${DIR})
            AOC_GENERATE(${AOC_YEAR} ${DIR})
            list(APPEND custom_target_args COMMAND ${AOC_YEAR}/aoc_${AOC_YEAR}_${DIR})
            list(APPEND custom_target_args DEPENDS aoc_${AOC_YEAR}_${DIR})
        else()
            message("Skipping ${DIR}")
        endif()
    endforeach()
endfunction()
