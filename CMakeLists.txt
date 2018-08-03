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

option(FORCE_INCLUDED_THIRD_PARTY "Don't use system libraries, instead use those in third_party/" OFF)
if (NOT EMSCRIPTEN)
    option(VULKAN_SUPPORT "Include the vulkan renderer" ON)
endif()

file(GLOB SOURCE_FILES
    include/Camera.h
    include/Debug.h
    include/Engine.h
    include/Game.h
    include/Input.h
    include/Logging.h
    include/Rendering.h
    include/Scene.h
    include/Scripting.h

    src/Debug.cc
    src/Engine.cc
    src/Game.cc
    src/Input.cc
    src/Logging.cc
    src/Rendering.cc
    src/Scripting.cc

    src/private/Manager.h
    src/private/Camera/CameraImpl.h
    src/private/Camera/CameraImpl.cc
    src/private/Command/Command.h
    src/private/Command/ReusableCommandQueue.h
    src/private/Command/ReusableCommandQueue.cc
    src/private/Engine/EngineManager.h
    src/private/Engine/EngineManager.cc
    src/private/Game/GameManager.h
    src/private/Game/GameManager.cc
    src/private/Input/InputManager.h
    src/private/Input/InputManager.cc
    src/private/Logging/LoggingManager.h
    src/private/Logging/LoggingManager.cc
    src/private/Rendering/GLES/CommandQueueGles.h
    src/private/Rendering/GLES/RenderingManagerGles.h
    src/private/Rendering/GLES/RenderingManagerGles.cc
    src/private/Rendering/RenderingManager.h
    src/private/Rendering/RenderingManager.cc
    src/private/Scene/SceneImpl.h
    src/private/Scene/SceneImpl.cc
    src/private/Scripting/FunctionTables.h
    src/private/Scripting/ScriptingUtils.h
    src/private/Scripting/ScriptingManager.h
    src/private/Scripting/ScriptingManager.cc
)

file(GLOB VULKAN_SOURCE_FILES
    src/private/Rendering/Vulkan/RenderingManagerVulkan.h
    src/private/Rendering/Vulkan/RenderingManagerVulkan.cc
)

file(GLOB LUA_MAIN_FILE main.lua)

list(APPEND SOURCE_FILES ${LUA_MAIN_FILE})

if (EMSCRIPTEN)
    list(APPEND SOURCE_FILES ${EMSCRIPTEN_SOURCE_FILES})
endif()

if (VULKAN_SUPPORT)
    list(APPEND SOURCE_FILES ${VULKAN_SOURCE_FILES})
endif()

add_executable(SCarterEngine ${SOURCE_FILES} main.cc)

if (EMSCRIPTEN OR FORCE_INCLUDED_THIRD_PARTY)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/lua-5.3.5)
    add_dependencies(SCarterEngine lua)
    set(LUA_LIBRARIES $<TARGET_FILE:lua>)
    set(LUA_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/lua-5.3.5/src)
else()
    find_package(Lua REQUIRED)
    set(OPENGL_LIBRARIES OpenGL::GL)
endif()

if (NOT EMSCRIPTEN)
    find_package(SDL2 REQUIRED)
    set(SDL2_LIBRARIES SDL2::SDL2)
endif()

if (VULKAN_SUPPORT)
    find_package(Vulkan REQUIRED)
    set(VULKAN_LIBRARIES Vulkan::Vulkan)
endif()

find_package(OpenGL REQUIRED)

set(SCarterEngine_DEP_INCLUDE_DIRS
    include/
    ${LUA_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIRS}
)
set(SCarterEngine_DEP_LIBRARIES
    c++
    ${LUA_LIBRARIES}
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${VULKAN_LIBRARIES}
)

set_target_properties(SCarterEngine PROPERTIES COMPILE_FLAGS "-stdlib=libc++")  # Compatibility with emscripten
target_include_directories(SCarterEngine PRIVATE ${SCarterEngine_DEP_INCLUDE_DIRS})
target_link_libraries(SCarterEngine PRIVATE ${SCarterEngine_DEP_LIBRARIES})
get_filename_component(LUA_MAIN_FILE_BASENAME ${LUA_MAIN_FILE} NAME)
if (NOT CMAKE_CURRENT_BINARY_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${LUA_MAIN_FILE_BASENAME}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LUA_MAIN_FILE} ${CMAKE_BINARY_DIR}
        COMMENT "Copying Lua File: ${LUA_MAIN_FILE_BASENAME}"
        MAIN_DEPENDENCY ${LUA_MAIN_FILE}
    )
endif()

define_file_basename_for_sources(SCarterEngine)

if (EMSCRIPTEN)
    list(APPEND EMSCRIPTEN_OPTIONS "WASM=1")
    list(APPEND EMSCRIPTEN_OPTIONS "ERROR_ON_UNDEFINED_SYMBOLS=1")
    list(APPEND EMSCRIPTEN_OPTIONS "USE_SDL=2")
    list(APPEND EMSCRIPTEN_OPTIONS "USE_WEBGL2=1")
    list(APPEND PRELOAD_FILES "${LUA_MAIN_FILE_BASENAME}")
    foreach(OPT ${EMSCRIPTEN_OPTIONS})
        set(EMSCRIPTEN_OPTION_STR "${EMSCRIPTEN_OPTION_STR} -s ${OPT}")
    endforeach()
    foreach(FILE in ${PRELOAD_FILES})
        get_filename_component(FILE_BASENAME "${FILE}" NAME)
        set(PRELOAD_FILE_STR "${PRELOAD_FILE_STR} --preload-file ${FILE_BASENAME}")
    endforeach()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 ${EMSCRIPTEN_OPTION_STR}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${EMSCRIPTEN_OPTION_STR} --emrun ${PRELOAD_FILE_STR}")
    # Force emscripten to generate html file for us. In the future this will not be necessary.
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set_target_properties(SCarterEngine PROPERTIES OUTPUT_NAME "index")
else()
    if (VULKAN_SUPPORT)
        target_compile_definitions(SCarterEngine PUBLIC SCE_SUPPORTS_VULKAN)
    endif()

    enable_testing()
    find_package(GTest REQUIRED)

    file(GLOB TEST_FILES
        tests/*.cc
    )
    file(GLOB TEST_LUA_FILES
        tests/*.lua
    )

    add_executable(SCarterEngineTest ${SOURCE_FILES} ${TEST_FILES} ${TEST_LUA_FILES})
    if(FORCE_INCLUDED_THIRD_PARTY)
        add_dependencies(SCarterEngineTest lua)
    endif()

    # Does not link due to GTest but useful for testing compile
    # set_target_properties(SCarterEngineTest PROPERTIES COMPILE_FLAGS "-stdlib=libc++")

    target_include_directories(SCarterEngineTest PRIVATE ${GTEST_INCLUDE_DIRS} ${SCarterEngine_DEP_INCLUDE_DIRS})
    target_link_libraries(SCarterEngineTest ${GTEST_BOTH_LIBRARIES} ${SCarterEngine_DEP_LIBRARIES})

    foreach(TEST_LUA_FILE ${TEST_LUA_FILES})
        get_filename_component(LUA_BASENAME ${TEST_LUA_FILE} NAME)
        add_custom_command(
            OUTPUT ${CMAKE_BINARY_DIR}/${LUA_BASENAME}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TEST_LUA_FILE} ${CMAKE_BINARY_DIR}
            COMMENT "Copying Lua File: ${LUA_BASENAME}"
            MAIN_DEPENDENCY ${TEST_LUA_FILE}
        )
    endforeach()
    gtest_add_tests(SCarterEngineTest "" ${TEST_FILES})

    define_file_basename_for_sources(SCarterEngineTest)
endif()
