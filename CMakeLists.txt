cmake_minimum_required(VERSION 3.16)
project(sam C)

option(USE_SDL "Build with SDL2 live audio output" ON)

# Match the static CRT used by the x64-windows-static vcpkg triplet so that
# statically linked SDL2 and our executable agree on the runtime library.
# Harmless for dynamic/non-MSVC builds; only MSVC honors this property.
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

add_executable(sam
    src/reciter.c
    src/sam.c
    src/render.c
    src/main.c
    src/debug.c
)

target_compile_options(sam PRIVATE
    $<$<C_COMPILER_ID:GNU,Clang>:-Wall>
    $<$<C_COMPILER_ID:MSVC>:/W3>
)

if(USE_SDL)
    find_package(SDL2 REQUIRED)
    target_compile_definitions(sam PRIVATE USESDL)
    if(WIN32)
        target_compile_definitions(sam PRIVATE SDL_MAIN_HANDLED)
    endif()
    target_link_libraries(sam PRIVATE SDL2::SDL2)
endif()

add_custom_command(TARGET sam POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/dist
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:sam> ${CMAKE_SOURCE_DIR}/dist
    VERBATIM)
