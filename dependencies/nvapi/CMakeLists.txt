cmake_minimum_required(VERSION 3.20)

project(NvApiExample LANGUAGES CXX)

include(FetchContent)

# Fetch NVAPI repository
FetchContent_Declare(
    nvapi_repo
    GIT_REPOSITORY https://github.com/NVIDIA/nvapi.git
    GIT_TAG main
)

FetchContent_MakeAvailable(nvapi_repo)

# Determine architecture folder
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(NVAPI_LIB_DIR "${nvapi_repo_SOURCE_DIR}/amd64")
else()
    set(NVAPI_LIB_DIR "${nvapi_repo_SOURCE_DIR}/x86")
endif()

# Create a static library target that wraps the NVAPI import library
add_library(nvapi STATIC IMPORTED GLOBAL)

set_target_properties(nvapi PROPERTIES
    IMPORTED_LOCATION "${NVAPI_LIB_DIR}/nvapi64.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${nvapi_repo_SOURCE_DIR}"
)

# Adjust library name for 32-bit
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    set_target_properties(nvapi PROPERTIES
        IMPORTED_LOCATION "${NVAPI_LIB_DIR}/nvapi.lib"
    )
endif()