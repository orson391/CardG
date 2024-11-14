# FindSDL2.cmake - Custom SDL2 Finder for CMake

# Define required SDL2 variables
find_path(SDL2_INCLUDE_DIR SDL2/SDL.h
          PATH_SUFFIXES SDL2 include
          DOC "C:\vcpkg\installed\x64-windows\include\SDL2")

find_library(SDL2_LIBRARY NAMES SDL2 SDL2-2.0 SDL2main
             PATH_SUFFIXES lib64 lib
             DOC "C:\vcpkg\installed\x64-windows\lib\SDL2.lib")

# Check if both include directory and library were found
if (SDL2_INCLUDE_DIR AND SDL2_LIBRARY)
    set(SDL2_FOUND TRUE)
else()
    set(SDL2_FOUND FALSE)
endif()

# Set the variables used by `target_include_directories` and `target_link_libraries`
if (SDL2_FOUND)
    set(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIR})
    set(SDL2_LIBRARIES ${SDL2_LIBRARY})
else()
    message(FATAL_ERROR "SDL2 not found. Please install SDL2 or specify SDL2 paths manually.")
endif()

# Make the variables visible to CMake
mark_as_advanced(SDL2_INCLUDE_DIR SDL2_LIBRARY)
