cmake_minimum_required(VERSION 3.20)

project(DisplayManager.Test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DisplayManager.Test.Sources
        "DisplayManager/NvApi/ConfigurationTest.cpp"
)

enable_testing()

add_executable(DisplayManager.Test ${DisplayManager.Test.Sources} )
target_link_libraries(DisplayManager.Test PRIVATE DisplayManager)
target_include_directories(DisplayManager.Test PRIVATE "./")

find_package(spdlog REQUIRED)
target_link_libraries(DisplayManager.Test PRIVATE spdlog::spdlog)

find_package(gtest REQUIRED)
target_link_libraries(DisplayManager.Test PRIVATE GTest::gtest_main GTest::gmock)

include(GoogleTest)
gtest_discover_tests(DisplayManager.Test)