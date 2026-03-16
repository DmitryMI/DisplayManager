cmake_minimum_required(VERSION 3.20)

add_library(
        DisplayManager
        "DisplayManager/Factory.cpp"
        "DisplayManager/NvApi/DisplayProvider.cpp"
        "DisplayManager/NvApi/Init.cpp"
        "DisplayManager/NvApi/Display.cpp"
        "DisplayManager/NvApi/DisplayConfigPathInfo.cpp"
        "DisplayManager/NvApi/Configuration.cpp"
        "DisplayManager/Serialization/BoostInputArchive.cpp"
        "DisplayManager/Serialization/BoostOutputArchive.cpp"
)

set_property(TARGET DisplayManager PROPERTY CXX_STANDARD 23)
target_include_directories(DisplayManager PUBLIC "../public")
target_link_libraries(DisplayManager PUBLIC nvapi)

find_package(Boost REQUIRED COMPONENTS serialization)
target_link_libraries(DisplayManager PUBLIC Boost::serialization)