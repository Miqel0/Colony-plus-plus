cmake_minimum_required(VERSION 3.15)
project(ColonyPlusPlus VERSION 1.0 LANGUAGES CXX RC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- ZMIENNE Z ŚCIEŻKAMI ---
set(SFML_DIR_CUSTOM "${CMAKE_SOURCE_DIR}/libs/SFML-3.0.2")
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/libs/imgui")

# --- ZBIERANIE PLIKÓW ŹRÓDŁOWYCH ---
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui-SFML.cpp
)

# --- TWORZENIE PLIKU WYKONYWALNEGO ---
add_executable(Colony++ ${SOURCES} ${IMGUI_SOURCES} src/resources.rc)

# --- NAGŁÓWKI (INCLUDE) ---
target_include_directories(Colony++ PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${SFML_DIR_CUSTOM}/include
    ${IMGUI_DIR}
)

# --- BIBLIOTEKI (.LIB) ---
target_link_directories(Colony++ PRIVATE 
    ${SFML_DIR_CUSTOM}/lib
)

target_link_libraries(Colony++ PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    opengl32
)

# Flagi kompilatora MSVC
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
set(SFML_BIN_DIR "C:/BIBLIOTEKI_CPP/SFML-3.0.2/bin")
file(GLOB SFML_DLLS "${SFML_BIN_DIR}/*.dll")
file(COPY ${SFML_DLLS} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")