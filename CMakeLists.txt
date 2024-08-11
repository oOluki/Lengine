cmake_minimum_required(VERSION 3.27.0)
project(Lengine VERSION 0.2.1 LANGUAGES C CXX)


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
pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(GLEW REQUIRED glew)
pkg_check_modules(GLFW REQUIRED glfw3)


include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_GFX})


include_directories(${GLEW_INCLUDE_DIRS})


include_directories(${GLFW_INCLUDE_DIRS})


# Add executable and specify source files
add_executable(Lengine ${SOURCE_FILES})

target_link_libraries(Lengine stdc++fs)

target_link_libraries(Lengine
    ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_GFX_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${GLFW_LIBRARIES}
)

set(CMAKE_GENERATOR "Ninja")
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
