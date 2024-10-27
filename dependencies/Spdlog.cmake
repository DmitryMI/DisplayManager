cmake_minimum_required (VERSION 3.24)

include(FetchContent)

function(link_spdlog to_target)
	set(BOOST_INCLUDE_LIBRARIES program_options)
	set(BOOST_ENABLE_CMAKE ON)
	FetchContent_Declare(
		spdlog
		GIT_REPOSITORY https://github.com/gabime/spdlog.git
		GIT_TAG v1.14.1
		USES_TERMINAL_DOWNLOAD TRUE
		DOWNLOAD_NO_EXTRACT FALSE
		OVERRIDE_FIND_PACKAGE
	)
	FetchContent_MakeAvailable(spdlog)
	find_package(Boost 1.86 REQUIRED COMPONENTS program_options)

	target_link_libraries(${to_target} PUBLIC PUBLIC spdlog::spdlog)
endfunction()