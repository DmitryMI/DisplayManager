cmake_minimum_required (VERSION 3.20)

add_library(
	DisplayManager
	"DisplayManager/Factory.cpp"
	"DisplayManager/NvApi/DisplayProvider.cpp"
	"DisplayManager/NvApi/Init.cpp"
		DisplayManager/NvApi/Display.cpp
		../public/DisplayManager/NvApi/Display.h
)

set_property(TARGET DisplayManager PROPERTY CXX_STANDARD 23)
target_include_directories(DisplayManager PUBLIC "../public")
target_link_libraries(DisplayManager PUBLIC nvapi)