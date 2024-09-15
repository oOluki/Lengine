cmake_minimum_required(VERSION 3.27.0)
project(Lengine VERSION 0.2.2 LANGUAGES C CXX)


# Add your source files
set(SOURCE_FILES
    src/Lengine.c
)


# Add executable and specify source files
add_executable(Lengine ${SOURCE_FILES})

if(WIN32)
    target_link_libraries(Lengine ws2_32) # Link Windows libraries, e.g., ws2_32
elseif(UNIX AND NOT APPLE)
    target_link_libraries(Lengine dl)     # Link against dlfcn for Linux
elseif(APPLE)
    target_link_libraries(Lengine dl)     # Link against dlfcn for macOS
endif()

# Option to enable or disable the usage of external libraries
option(USE_EXTERNAL_LIBS "Use external libraries (SDL2, SDL2_image, SDL2_ttf)" ON)

# Initialize flag for finding all libraries
set(ALL_LIBRARIES_FOUND TRUE)

# Find SDL2
if(USE_EXTERNAL_LIBS)
  find_package(SDL2 QUIET)
  if(NOT SDL2_FOUND)
    message(WARNING "SDL2 not found, examples won't be build")
    set(ALL_LIBRARIES_FOUND FALSE)
  else()
    message(STATUS "SDL2 found")

    add_library(simple_example SHARED
      plugins/plugin.c
    )

    add_library(config SHARED
      plugins/config.c
    )
    target_link_libraries(config SDL2)
    target_compile_definitions(config PRIVATE LE_SDL2_FOUND)
    target_link_libraries(simple_example SDL2)
  endif()
endif()

# Find SDL_image
if(USE_EXTERNAL_LIBS AND ALL_LIBRARIES_FOUND)
  find_package(SDL2_image QUIET)
  if(NOT SDL2_image_FOUND)
    message(WARNING "SDL2_image not found, map_designer example won't be built")
    set(ALL_LIBRARIES_FOUND FALSE)
  else()
    message(STATUS "SDL2_image found")
  endif()
endif()

# Find SDL_ttf
if(USE_EXTERNAL_LIBS AND ALL_LIBRARIES_FOUND)
  find_package(SDL2_ttf QUIET)
  if(NOT SDL2_ttf_FOUND)
    message(WARNING "SDL2_ttf not found, map_designer example won't be built")
    set(ALL_LIBRARIES_FOUND FALSE)
  else()
    message(STATUS "SDL2_ttf found")
    add_library(map_designer SHARED
      plugins/map\ designer/map_designer.c
    )
    target_link_libraries(config SDL2_image SDL2_ttf)
    target_compile_definitions(config PRIVATE LE_IMG_TTF_FOUND)
    target_link_libraries(map_designer SDL2 SDL2_image SDL2_ttf)
  endif()
endif()


set(CMAKE_GENERATOR "Ninja")
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
