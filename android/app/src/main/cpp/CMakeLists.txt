cmake_minimum_required(VERSION 3.22)

project(Orange LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui")

add_library(imgui STATIC
    ${IMGUI_DIR}/src/imgui.cpp
    ${IMGUI_DIR}/src/imgui_draw.cpp
    ${IMGUI_DIR}/src/imgui_tables.cpp
    ${IMGUI_DIR}/src/imgui_widgets.cpp)

target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}/include
    ${IMGUI_DIR}/backends)

target_compile_options(imgui PRIVATE -fno-exceptions -Wall)

file(GLOB_RECURSE ORANGE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(Orange SHARED ${ORANGE_SOURCES})

target_include_directories(Orange PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    ${IMGUI_DIR}/include
    ${IMGUI_DIR}/backends)

target_compile_options(Orange PRIVATE -fvisibility=hidden -Wall -Wextra -Wno-unused-parameter)

target_link_libraries(Orange PRIVATE imgui android log dl GLESv3 EGL z)

find_library(IMGUI_PREBUILT NAMES ImGui libImGui
    PATHS "${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}"
    NO_DEFAULT_PATH)
if (IMGUI_PREBUILT)
    target_link_libraries(Orange PRIVATE ${IMGUI_PREBUILT})
endif()
