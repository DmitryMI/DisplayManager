﻿# CMakeList.txt : CMake project for DisplayManager, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("DisplayManager")

add_executable (DisplayManager "app/main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET DisplayManager PROPERTY CXX_STANDARD 20)
endif()

# Boost
include(${CMAKE_SOURCE_DIR}/dependencies/Boost.cmake)
set(BOOST_INCLUDE_LIBRARIES program_options property_tree)
link_boost(DisplayManager "${BOOST_INCLUDE_LIBRARIES}")

# spdlog
include(${CMAKE_SOURCE_DIR}/dependencies/Spdlog.cmake)
link_spdlog(DisplayManager)
