cmake_minimum_required(VERSION 4.1)
project(lavarand_ecdsa C)

find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(JPEG REQUIRED)
find_package(Stb REQUIRED)

set(CMAKE_C_STANDARD 11)

add_executable(lavarand_ecdsa main.c
        hsv.h
        image_processing.h
        image_processing.c
        gaussian_blur.c
        gaussian_blur.h
        hsv.c
        binary_processing.c
        binary_processing.h
        ecdsa.c
        ecdsa.h
        camera_processing.c
        camera_processing.h
)

target_link_libraries(lavarand_ecdsa PRIVATE SDL3::SDL3 SDL3_image::SDL3_image JPEG::JPEG)
target_include_directories(lavarand_ecdsa PRIVATE ${Stb_INCLUDE_DIR})