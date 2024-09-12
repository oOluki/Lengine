cmake_minimum_required(VERSION 3.27.0)
project(Lengine VERSION 0.2.2 LANGUAGES C CXX)


# Add your source files
set(SOURCE_FILES
    src/Lengine.c
)

add_library(simple_example SHARED
    plugins/plugin.c
)

add_library(map_designer SHARED
    plugins/map\ designer/map_designer.c
)


find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL2 REQUIRED)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)


# Add executable and specify source files
add_executable(Lengine ${SOURCE_FILES})

target_link_libraries(Lengine stdc++fs)

target_link_libraries(Lengine
    ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES}
)

set(CMAKE_GENERATOR "Ninja")
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
