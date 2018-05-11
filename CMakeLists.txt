cmake_minimum_required(VERSION 3.10)
project(SCarter_Engine)

set(CMAKE_CXX_STANDARD 11)

file(GLOB HEADER_FILES
    GameStates/CubicGameState.hpp
    GL1_Primitives/GL1_Cube.hpp
    GL1_Primitives/GL1_Sphere.hpp
    GLConfigUtil.hpp
    Gui.hpp
    IKArm.hpp
    IKLine.hpp
    md5/Joint.hpp
    md5/Mesh.hpp
    Scenes/SceneBase.hpp
    Scenes/SceneCubicContainer.hpp
    Scenes/SceneGears.hpp
    Scenes/SceneIK.hpp
    Scenes/SceneMD5.hpp
    Scenes/SceneMorph.hpp
    Scenes/SceneParticleBase.hpp
    Scenes/SceneParticleFountain.hpp
    Scenes/SceneParticleRepelant.hpp
    md5/Triangle.hpp
    md5/Vertex.hpp
    md5/Weight.hpp
    # ../A1/interpolation.hpp
    math3d.hpp
)

file(GLOB SOURCE_FILES
    GameStates/CubicGameState.cpp
    GL1_Primitives/GL1_Cube.cpp
    GL1_Primitives/GL1_Sphere.cpp
    Gui.cpp
    IKArm.cpp
    IKLine.cpp
    md5/Joint.cpp
    md5/Mesh.cpp
    Scenes/SceneBase.cpp
    Scenes/SceneCubicContainer.cpp
    Scenes/SceneGears.cpp
    Scenes/SceneIK.cpp
    Scenes/SceneMD5.cpp
    Scenes/SceneMorph.cpp
    Scenes/SceneParticleBase.cpp
    Scenes/SceneParticleFountain.cpp
    Scenes/SceneParticleRepelant.cpp
    md5/Triangle.cpp
    md5/Vertex.cpp
    md5/Weight.cpp
    main.cpp
)

file(GLOB RESOURCE_FILES
    scarter_engine.glade
)

add_executable(SCarterEngine ${SOURCE_FILES} ${HEADER_FILES})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GTK2 REQUIRED gtk gtkmm glade)

target_include_directories(SCarterEngine PRIVATE
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
    ${GTK2_INCLUDE_DIRS}
    /usr/lib/gtkglextmm-1.2/include
    /usr/include/gtkglextmm-1.2
    /usr/lib/gtkglext-1.0/include
    /usr/include/gtkglext-1.0
)
target_link_libraries(SCarterEngine PRIVATE
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${GTK2_LIBRARIES}
    gtkglextmm-x11-1.2
    gdkglextmm-x11-1.2
)
add_custom_command(TARGET SCarterEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${RESOURCE_FILES} $<TARGET_FILE_DIR:SCarterEngine>)
