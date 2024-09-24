# CMakeList.txt : CMake project for cRPG, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("cRPG")

# Add source to this project's executable.
add_executable(cRPG "cRPG/cRPG.cpp" "cRPG/cRPG.h" "cRPG/Game.cpp" "cRPG/Game.h" "cRPG/Logger.h")

# Set C++20 standard explicitly
set_target_properties(cRPG PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

# TODO: Add tests and install targets if needed.
