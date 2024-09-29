cmake_minimum_required(VERSION 3.27.0)
project(Lengine VERSION 0.3.0 LANGUAGES C CXX)


# Add your source files
set(SOURCE_FILES
    src/Lengine.c
)


# Add executable and specify source files
add_executable(Lengine ${SOURCE_FILES})


add_library(test_plugin SHARED plugins/test.c)

target_compile_definitions(test_plugin PRIVATE LE_BUILDING_DLL)

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
    message("SDL2 not found, map_designer example won't be build")
    set(ALL_LIBRARIES_FOUND FALSE)
  endif()
endif()

# Find SDL_image
if(USE_EXTERNAL_LIBS AND ALL_LIBRARIES_FOUND)
  find_package(SDL2_image QUIET)
  if(NOT SDL2_image_FOUND)
    message("SDL2_image not found, map_designer example won't be built")
    set(ALL_LIBRARIES_FOUND FALSE)
  endif()
endif()

# Find SDL_ttf
if(USE_EXTERNAL_LIBS AND ALL_LIBRARIES_FOUND)
  find_package(SDL2_ttf QUIET)
  if(NOT SDL2_ttf_FOUND)
    message(WARNING "SDL2_ttf not found, map_designer example won't be built")
    set(ALL_LIBRARIES_FOUND FALSE)
  else()
    message(STATUS "SDL2 dependencies for map_designer example found, map_designer will be built as an example")

    add_library(map_designer SHARED
      plugins/map\ designer/map_designer.c
    )

    add_library(config SHARED
      plugins/config.c
    )

    target_link_libraries(config SDL2 SDL2_image SDL2_ttf)
    target_compile_definitions(config PRIVATE LE_SDL2_FOUND LE_IMG_TTF_FOUND LE_BUILDING_DLL)
    target_link_libraries(map_designer SDL2 SDL2_image SDL2_ttf)
    target_compile_definitions(map_designer PRIVATE LE_BUILDING_DLL)
  endif()
endif()

include(CTest)
enable_testing()


# Use a custom command to print the absolute path of the built library
add_custom_command(
    TARGET test_plugin
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Built shared library absolute path: $<TARGET_FILE:test_plugin>"
)

# Optionally, if you want to store the path in a variable:
set(TEST_PLUGIN_PATH $<TARGET_FILE:test_plugin>)


add_test(NAME Test COMMAND Lengine ${TEST_PLUGIN_PATH})

set_tests_properties(Test PROPERTIES TIMEOUT 5)


message("Built shared library absolute path: ${TEST_PLUGIN_PATH}")


