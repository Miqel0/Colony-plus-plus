cmake_minimum_required(VERSION 3.15)

project(ColonyPlusPlus VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


if(WIN32)
    enable_language(RC) 
    set(SFML_DIR_CUSTOM "${CMAKE_SOURCE_DIR}/libs/SFML-3.0.2")
    set(OPENGL_LIB "opengl32")
elseif(UNIX)
    set(SFML_DIR_CUSTOM "${CMAKE_SOURCE_DIR}/libs/SFML-3.0.2-linux")
    set(OPENGL_LIB "GL") 
endif()

set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/libs/imgui")
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui-SFML.cpp
)


if(WIN32)
    add_executable(Colony++ ${SOURCES} ${IMGUI_SOURCES} src/resources.rc)
else()
    add_executable(Colony++ ${SOURCES} ${IMGUI_SOURCES})
endif()

target_include_directories(Colony++ BEFORE PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${SFML_DIR_CUSTOM}/include
    ${IMGUI_DIR}
)

target_link_directories(Colony++ BEFORE PRIVATE 
    ${SFML_DIR_CUSTOM}/lib
)

if(WIN32)
    target_link_libraries(Colony++ PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        ${OPENGL_LIB} 
    )
elseif(UNIX)
    target_link_libraries(Colony++ PRIVATE
        "${SFML_DIR_CUSTOM}/lib/libsfml-graphics.so"
        "${SFML_DIR_CUSTOM}/lib/libsfml-window.so"
        "${SFML_DIR_CUSTOM}/lib/libsfml-system.so"
        ${OPENGL_LIB}
    )
    
    set_target_properties(Colony++ PROPERTIES BUILD_RPATH "${SFML_DIR_CUSTOM}/lib")
endif()

if(MSVC)
    target_compile_options(Colony++ PRIVATE /EHsc /W3)
endif()

set_target_properties(Colony++ PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}"
)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/data" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

if(WIN32)
    set(SFML_BIN_DIR "${SFML_DIR_CUSTOM}/bin") 
    file(GLOB SFML_DLLS "${SFML_BIN_DIR}/*.dll")
    if(SFML_DLLS)
        file(COPY ${SFML_DLLS} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
    endif()
endif()