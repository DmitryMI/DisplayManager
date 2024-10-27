cmake_minimum_required (VERSION 3.24)

include(FetchContent)

function(link_boost to_target boost_libs)
	set(BOOST_INCLUDE_LIBRARIES ${boost_libs})
	set(Boost_USE_STATIC_LIBS ON)
	set(BOOST_ENABLE_CMAKE ON)
	FetchContent_Declare(
		  Boost
		  URL https://github.com/boostorg/boost/releases/download/boost-1.86.0/boost-1.86.0-cmake.7z
		  USES_TERMINAL_DOWNLOAD TRUE
		  DOWNLOAD_NO_EXTRACT FALSE
		  OVERRIDE_FIND_PACKAGE
		)
	FetchContent_MakeAvailable(Boost)
	find_package(Boost 1.86 REQUIRED COMPONENTS ${boost_libs})

	foreach(boost_lib IN LISTS boost_libs)
		message(STATUS "Linking ${to_target} to Boost::${boost_lib}")
		target_link_libraries(${to_target} PUBLIC Boost::${boost_lib})
	endforeach()
endfunction()