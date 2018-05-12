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
)

add_executable(SCarterEngine ${SOURCE_FILES} main.cc)

set(SCarterEngine_DEP_INCLUDE_DIRS
    include/
)
set(SCarterEngine_DEP_LIBRARIES
    c++
)

target_include_directories(SCarterEngine PRIVATE ${SCarterEngine_DEP_INCLUDE_DIRS})
target_link_libraries(SCarterEngine PRIVATE ${SCarterEngine_DEP_LIBRARIES})

define_file_basename_for_sources(SCarterEngine)
