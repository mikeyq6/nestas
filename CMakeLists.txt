include(FetchContent)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ compiler version: ${CMAKE_CXX_COMPILER_VERSION}")

project(nestas)

option(USE_VENDORED_SDL "Download SDL2 from GitHub" ON)

if (USE_VENDORED_SDL)
    message(STATUS "Using vendored SDL")

    set(FETCHCONTENT_QUIET FALSE)

    FetchContent_Declare(
            SDL2
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG release-2.32.10
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(SDL2)

    FetchContent_Declare(
            SDL2_image
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
            GIT_TAG release-2.8.8
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(SDL2_image)

endif()

set(SDL2_IMAGE_INSTALL OFF)
set(SDL2_DISABLE_INSTALL OFF)

add_executable(nestas
    nestas.cpp
    Emulator.cpp
    CPU.cpp
    Display.cpp
    DisplayFactory.cpp
    SDLDisplay.cpp
    SharedData.cpp
    PPU.cpp
    MapperFactory.cpp
    mappers/Mapper.cpp
    mappers/INESMapper206.cpp
    mappers/MMC1.cpp
    mappers/MMC2.cpp
    mappers/NROM.cpp
    mappers/UxROM.cpp
)

target_link_libraries(nestas SDL2::SDL2 SDL2_image::SDL2_image)

target_compile_options(nestas PRIVATE -Wall -Werror -pthread)

add_definitions(-DDEBUG=1)