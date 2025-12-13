cmake_minimum_required(VERSION 3.20)

project(DX11App
    VERSION 1.0
    LANGUAGES CXX
)

# ----------------------------
# C++ settings
# ----------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# MSVC-specific options
if (MSVC)
    add_compile_options(/W4 /permissive-)
endif()

# ----------------------------
# Source files
# ----------------------------
file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(DX11App WIN32 ${SOURCES})
add_library(DX11Lib SHARED ${SOURCES})

# ----------------------------
# Windows / DirectX libraries
# ----------------------------
target_link_libraries(DX11App
    PRIVATE
        d3d11
        dxgi
        d3dcompiler
        dxguid
        user32
        gdi32
        winmm
)

# ----------------------------
# Include directories
# ----------------------------
target_include_directories(DX11App
    PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src/ImGui
)

# ----------------------------
# Unicode & Windows defines
# ----------------------------
target_compile_definitions(DX11App
    PRIVATE
        UNICODE
        _UNICODE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
)

# ----------------------------
# Windows / DirectX libraries
# ----------------------------
target_link_libraries(DX11Lib
        PRIVATE
        d3d11
        dxgi
        d3dcompiler
        dxguid
        user32
        gdi32
        winmm
)

# ----------------------------
# Include directories
# ----------------------------
target_include_directories(DX11Lib
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ImGui
)

# ----------------------------
# Unicode & Windows defines
# ----------------------------
target_compile_definitions(DX11Lib
        PRIVATE
        UNICODE
        _UNICODE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
        _WINDLL
)