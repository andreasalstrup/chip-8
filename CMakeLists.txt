cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(chip-8 VERSION 1.0)

# specify the c++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# add include directories
include_directories(./imgui ./imgui/backends)

# define the source files
set(SOURCES
        ./imgui/imgui.cpp
        ./imgui/imgui_demo.cpp
        ./imgui/imgui_draw.cpp
        ./imgui/imgui_tables.cpp
        ./imgui/imgui_widgets.cpp
        ./imgui/backends/imgui_impl_sdl2.cpp
        ./imgui/backends/imgui_impl_opengl3.cpp
)

file(GLOB_RECURSE EMULATOR_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/emulator/src/*.cpp")
file(GLOB_RECURSE PLATFORM_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/platform/src/*.cpp")

add_executable(chip-8 ${EMULATOR_SOURCES} ${PLATFORM_SOURCES} ${SOURCES})
target_include_directories(chip-8 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/emulator/include") #add header file
target_include_directories(chip-8 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/platform/include") #add header file
remove_definitions(-DUNICODE) #remove unicode

# add an executable
#add_executable(chip-8 ${SOURCES})

if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
        include_directories(/home/novem/dev/emsdk/upstream/emscripten/)

        set(CMAKE_EXECUTABLE_SUFFIX ".html")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/build/wasm)

#         set(cmake_cxx_flags "${cmake_cxx_flags} \
#             -s -s use_sdl=2 \
#             -s disable_exception_catching=1 \
#             -std=c++20")
#
#         set(cmake_exe_linker_flags "${cmake_exe_linker_flags} \
#     -s wasm=1 \
#     -s full_es3=1 \
#     -s use_webgl2=1 \
#     -s allow_memory_growth=1 \
#     -s assertions=1 \
#     --preload-file ${cmake_source_dir}/assets@/assets \
# ")


        # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
        # -s USE_SDL=2 \
        #     -s wasm=1 \
        #     -s full_es3=1 \
        #     -s use_webgl2=1 \
        #     -s allow_memory_growth=1 \
        #     -s assertions=1 \
        #     -s disable_exception_catching=1 \
        #     --shell-file ${cmake_current_source_dir}/shell_minimal.html \
        #     --preload-file ${cmake_current_source_dir}/assets@/assets")

         set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
        -s USE_SDL=2 \
        -s WASM=1 \
        -s FULL_ES3=1 \
        -s USE_WEBGL2=1 \
        -s ALLOW_MEMORY_GROWTH=1 \
        -s ASSERTIONS=1 \
        -s DISABLE_EXCEPTION_CATCHING=1 \
        -- no-heap-copy --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/assets@/assets"
    )

        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
        message(status "🔧 configuring webassembly build (emscripten)")
else()
        # find an link opengl
        find_package(OpenGL REQUIRED)

        # find and link sdl2
        find_package(PkgConfig REQUIRED)
        pkg_search_module(SDL2 REQUIRED sdl2)
        include_directories(${SDL2_INCLUDE_DIRS})

        # link libraries
        target_link_libraries(chip-8 ${SDL2_LIBRARIES} ${OPENGL_gl_LIBRARY})
endif()


# print a message after build
add_custom_command(TARGET chip-8 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "build complete for ${CMAKE_SYSTEM_NAME}"
)
