# CMakeList.txt : CMake project for cRPG, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("cRPG")

# ===================================================================
# Dependencies
# ===================================================================

# Add the tinyxml2 library from its source files
add_library(tinyxml2 STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/external/tinyxml2/tinyxml2.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/tinyxml2//tinyxml2.h"
)

# Specify the include directories for tinyxml2
target_include_directories(tinyxml2 PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/external/tinyxml2"
)

# ===================================================================
# Main Executable
# ===================================================================

# Automatically find all .cpp files inside the cRPG directory,
# excluding the ones in the libs folder.
file(GLOB_RECURSE CRPG_SOURCES
    "cRPG.cpp"
    "include/cRPG/*.h"
    "src/*.cpp"
)

# Add the main executable target using the automatically found sources
add_executable(cRPG ${CRPG_SOURCES})

# Add the cRPG directory to the include path.
# This lets you use #include "Game.h" instead of #include "cRPG/Game.h".
target_include_directories(cRPG PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/cRPG"
)

# Specify the C++ standard
set_property(TARGET cRPG PROPERTY CXX_STANDARD 20)

# Link the tinyxml2 library to the executable
target_link_libraries(cRPG PRIVATE tinyxml2)

# Add NOMINMAX to prevent Windows API from defining min/max macros.
target_compile_definitions(cRPG PRIVATE NOMINMAX)


# TODO: Add tests and install targets if needed.
# Custom Command for Asset XML files -- Comes after the add_executable
add_custom_command(
    TARGET cRPG POST_BUILD
    # Command to run: CMake's built-in copy tool
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    # Assets Folder
    "${CMAKE_SOURCE_DIR}/assets"
    # The destination: an "assets" folder inside the same directory where cRPG.exe is created.
    "$<TARGET_FILE_DIR:cRPG>/assets"
    # A message that prints during the build
    COMMENT "Copying assets folder to build directory..."
)