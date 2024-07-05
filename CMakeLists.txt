cmake_minimum_required(VERSION 3.22.1)

set(PROJECT_TESOROLABERINTO "tesoro_laberinto")

project(${PROJECT_TESOROLABERINTO} VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

set(SRC ./src)
set(ASSIMP_DIR ./include/assimp)

# Add GLFW and Assimp libraries
add_subdirectory(include/glfw)
add_subdirectory(include/assimp)

set(SRC_FILES
    ${SRC}/glad.c
    ${SRC}/formas.cpp
    ${SRC}/modelo.cpp
    ${SRC}/main.cpp
    ${SRC}/escena_juego.cpp
    ${SRC}/menu_escena.cpp
    ${SRC}/creditos.cpp
    ${SRC}/constructor.cpp
    ${SRC}/instrucciones.cpp
    ${SRC}/textura.cpp
    ${SRC}/miscelaneos.cpp
)

set(LINK_LIB
    glfw
    OpenGL::GL
    assimp
)

include_directories(${INCLUDE_DIR})
add_executable(${PROJECT_TESOROLABERINTO} ${SRC_FILES})

if (assimp_FOUND)
    set(ASSIMP_LIBRARY "assimp")
    add_library(${ASSIMP_LIBRARY} SHARED IMPORTED)
    set_target_properties(${ASSIMP_LIBRARY} PROPERTIES IMPORTED_LOCATION "${ASSIMP_LIBRARY_DIRS}/libassimp.so")
endif(assimp_FOUND)

if(WIN32)
    set_target_properties(${PROJECT_NAME}
    PROPERTIES
        LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
        LINK_FLAGS_RELEASE "/SUBSYSTEM:windows /ENTRY:mainCRTStartup"
        LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:windows /ENTRY:mainCRTStartup"
        LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:windows /ENTRY:mainCRTStartup"
    )
endif(WIN32)

target_link_libraries(${PROJECT_TESOROLABERINTO} ${LINK_LIB})
