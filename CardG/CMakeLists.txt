# CMakeList.txt : CMake project for CardG, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (CardG "CardG.cpp" "CardG.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CardG PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
# Add custom FindSDL2 module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

# Find SDL2 using custom FindSDL2.cmake
find_package(SDL2 REQUIRED)

# Link SDL2 to the target
target_include_directories(CardG PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(CardG PRIVATE ${SDL2_LIBRARIES})

add_custom_command(TARGET CardG POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/vcpkg/installed/x64-windows/debug/bin/SDL2d.dll"  # Replace with actual path if needed
        $<TARGET_FILE_DIR:CardG>)
