
add_executable (DisplayManager.Cli "main.cpp")

set_property(TARGET DisplayManager.Cli PROPERTY CXX_STANDARD 23)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(DisplayManager.Cli PRIVATE spdlog::spdlog)

find_package(Boost REQUIRED COMPONENTS program_options)
target_link_libraries(DisplayManager.Cli PRIVATE Boost::program_options)

target_link_libraries(DisplayManager.Cli PRIVATE DisplayManager) 