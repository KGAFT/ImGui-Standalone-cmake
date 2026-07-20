cmake_minimum_required(VERSION 3.20)

project(dx11renderbackend
    VERSION 1.0
    LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/W4 /permissive-)
endif()


file(GLOB_RECURSE SOURCES src/cxx/*.cpp)

add_library(dx11renderbackend STATIC ${SOURCES})


target_link_libraries(dx11renderbackend
        PUBLIC
        d3d11
        dxgi
        d3dcompiler
        dxguid
        user32
        gdi32
        winmm
        ntdll.lib
        userenv.lib
        ws2_32.lib
)

set_target_properties(dx11renderbackend PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin
)

target_include_directories(dx11renderbackend
        PRIVATE
        src/cxx/ImGui
        src/cxx
)

target_compile_definitions(dx11renderbackend
        PRIVATE
        UNICODE
        _UNICODE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
)
