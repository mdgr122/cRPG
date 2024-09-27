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

# Add source to this project's executable.
add_executable(cRPG "cRPG/cRPG.cpp" "cRPG/cRPG.h" "cRPG/Game.cpp" "cRPG/Game.h" "cRPG/Logger.cpp" "cRPG/Logger.h" "cRPG/Colors.h" "cRPG/Console.cpp" "cRPG/Console.h" "cRPG/Button.h" "cRPG/Keys.h" "cRPG/Keyboard.cpp" "cRPG/Keyboard.h" "cRPG/Button.cpp" "cRPG/Button.h"  "cRPG/states/InterfaceState.h" "cRPG/states/StateMachine.h" "cRPG/states/StateMachine.cpp" "cRPG/states/GameState.h" "cRPG/states/GameState.cpp" "cRPG/Selector.h" "cRPG/Actor.cpp" "cRPG/Actor.h" "cRPG/Player.cpp" "cRPG/Player.h" "cRPG/Equipment.cpp" "cRPG/Equipment.h" "cRPG/Item.h"  "cRPG/Inventory.cpp" "cRPG/Inventory.h" "cRPG/Stats.cpp" "cRPG/Stats.h" "cRPG/Party.cpp" "cRPG/Party.h" "cRPG/utilities/crpg_utils.h" "cRPG/Potion.cpp" "cRPG/Potion.h" "cRPG/utilities/ItemCreator.h" "cRPG/states/GameMenuState.cpp" "cRPG/states/GameMenuState.h" "cRPG/utilities/Timer.cpp" "cRPG/utilities/Timer.h")

set_property(TARGET cRPG PROPERTY CXX_STANDARD 20)

# TODO: Add tests and install targets if needed.

# Add NOMINMAX to prevent Windows API from defining min/max macros.
target_compile_definitions(cRPG PRIVATE NOMINMAX)