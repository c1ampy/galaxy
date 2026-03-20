cmake_minimum_required(VERSION 3.20)
project(galaxy LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(galaxy
    src/main.cpp
    src/render.cpp
    src/control.c
    src/high_score_save_load.c
    src/list.c
    src/object.c
)

target_include_directories(galaxy PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

if(NOT WIN32)
    message(FATAL_ERROR "This project currently targets Windows (EasyX + Win32).")
endif()

set(EASYX_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/easyx")

if(NOT EXISTS "${EASYX_ROOT}/include")
    message(FATAL_ERROR "EasyX include directory not found at ${EASYX_ROOT}/include.")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(_easyx_lib_arch_dir "lib64")
else()
    set(_easyx_lib_arch_dir "lib32")
endif()

if(MINGW)
    set(_easyx_search_paths
        "${EASYX_ROOT}/${_easyx_lib_arch_dir}"
        "${EASYX_ROOT}/lib"
    )
    find_library(EASYX_LIBRARY
        NAMES easyxw easyx
        PATHS ${_easyx_search_paths}
    )
elseif(MSVC)
    set(_easyx_search_paths
        "${EASYX_ROOT}/${_easyx_lib_arch_dir}"
        "${EASYX_ROOT}/lib"
    )
    find_library(EASYX_LIBRARY
        NAMES EasyXw easyxw EasyX easyx
        PATHS ${_easyx_search_paths}
    )
else()
    message(FATAL_ERROR "Unsupported toolchain. Use MSVC or MinGW on Windows.")
endif()

if(NOT EASYX_LIBRARY)
    if(MSVC)
        message(FATAL_ERROR "EasyX .lib not found under ${EASYX_ROOT}. MSVC requires EasyXw.lib/EasyX.lib; your EasyX package only has .a (MinGW). Use MinGW build or install MSVC EasyX libs.")
    else()
        message(FATAL_ERROR "EasyX library not found under ${EASYX_ROOT}.")
    endif()
endif()

target_include_directories(galaxy PRIVATE "${EASYX_ROOT}/include")

target_link_libraries(galaxy PRIVATE
    "${EASYX_LIBRARY}"
    winmm
    gdi32
    user32
    imm32
    msimg32
)

target_compile_definitions(galaxy PRIVATE UNICODE _UNICODE)

set(ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
add_custom_command(TARGET galaxy POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
    "${ASSETS_DIR}"
    "$<TARGET_FILE_DIR:galaxy>/assets"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
    "${ASSETS_DIR}/high_score.dat"
    "$<TARGET_FILE_DIR:galaxy>/high_score.dat"
)
