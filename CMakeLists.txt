cmake_minimum_required(VERSION 3.10)
project(SCarterEngine)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_VERBOSE_MAKEFILE TRUE)

function(define_file_basename_for_sources targetname)
    get_target_property(source_files "${targetname}" SOURCES)
    foreach(sourcefile ${source_files})
        # Get source file's current list of compile definitions.
        get_property(defs SOURCE "${sourcefile}"
            PROPERTY COMPILE_DEFINITIONS)
        # Add the FILE_BASENAME=filename compile definition to the list.
        get_filename_component(basename "${sourcefile}" NAME)
        list(APPEND defs "FILE_BASENAME=\"${basename}\"")
        # Set the updated compile definitions on the source file.
        set_property(
            SOURCE "${sourcefile}"
            PROPERTY COMPILE_DEFINITIONS ${defs})
    endforeach()
endfunction()

file(GLOB SOURCE_FILES
    include/Debug.h

    src/Debug.cc
    include/Logging.h

    src/Debug.cc
    src/Logging.cc

    src/private/Manager.h
    src/private/Logging/LoggingManager.h
    src/private/Logging/LoggingManager.cc
)

add_executable(SCarterEngine ${SOURCE_FILES} main.cc)

set(SCarterEngine_DEP_INCLUDE_DIRS
    include/
)
set(SCarterEngine_DEP_LIBRARIES
    c++
)

set_target_properties(SCarterEngine PROPERTIES COMPILE_FLAGS "-stdlib=libc++")  # Compatibility with emscripten
target_include_directories(SCarterEngine PRIVATE ${SCarterEngine_DEP_INCLUDE_DIRS})
target_link_libraries(SCarterEngine PRIVATE ${SCarterEngine_DEP_LIBRARIES})

define_file_basename_for_sources(SCarterEngine)

if (EMSCRIPTEN)
    list(APPEND EMSCRIPTEN_OPTIONS "WASM=1")
    foreach(OPT ${EMSCRIPTEN_OPTIONS})
        set(EMSCRIPTEN_OPTION_STR "${EMSCRIPTEN_OPTION_STR} -s ${OPT}")
    endforeach()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 ${EMSCRIPTEN_OPTION_STR}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${EMSCRIPTEN_OPTION_STR} --emrun")
    # Force emscripten to generate html file for us. In the future this will not be necessary.
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set_target_properties(SCarterEngine PROPERTIES OUTPUT_NAME "index")
else()
    enable_testing()
    find_package(GTest REQUIRED)

    file(GLOB TEST_FILES
        tests/*.cc
    )

    add_executable(SCarterEngineTest ${SOURCE_FILES} ${TEST_FILES})
    target_include_directories(SCarterEngineTest PRIVATE ${GTEST_INCLUDE_DIRS} ${SCarterEngine_DEP_INCLUDE_DIRS})
    target_link_libraries(SCarterEngineTest ${GTEST_BOTH_LIBRARIES} ${SCarterEngine_DEP_LIBRARIES})

    gtest_add_tests(SCarterEngineTest "" ${TEST_FILES})

    define_file_basename_for_sources(SCarterEngineTest)
endif()
