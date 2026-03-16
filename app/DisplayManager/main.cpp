#include <boost/program_options.hpp>
#include <spdlog/spdlog.h>
#include <iostream>
#include <string>
#include "DisplayManager/IDisplayProvider.h"
#include "DisplayManager/Factory.h"
#include <spdlog/spdlog.h>

namespace po = boost::program_options;

po::options_description CreateOptionsDescription()
{
	po::options_description desc("Allowed options");

	desc.add_options()
		("help", "produce help message")
		("enable", po::value<std::vector<std::string>>()->multitoken(), "Enable display")
		("disable", po::value<std::vector<std::string>>()->multitoken(), "Disable display")
		("enumerate", "Enumerate displays")
		("log-level", po::value<std::string>()->default_value("info"), "Log level")
		;

	return desc;
}

void SetupLogging(std::string logLevel)
{
	spdlog::set_level(spdlog::level::from_str(logLevel));
	spdlog::set_pattern("[%Y-%b-%d %T.%e] [%l] %v");
}

int main(int argc, char** argv)
{	
	auto displayProvider = std::unique_ptr<DisplayManager::IDisplayProvider>(DisplayManager::Factory::GetDisplayProvider());

	po::options_description optionsDesc = CreateOptionsDescription();
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, optionsDesc), vm);
	po::notify(vm);

	bool somethingDone = false;

	if (vm.count("help")) 
	{
		std::cout << optionsDesc << "\n";
		return 0;
	}

	SetupLogging(vm["log-level"].as<std::string>());
	spdlog::info("Log level set to {}", vm["log-level"].as<std::string>());

	if (vm.count("enumerate")) 
	{
		auto displays = displayProvider->GetDisplays();
		for (const auto& display : displays)
		{
			spdlog::info("Display: {}, Enabled: {}, Coordinates: ({}, {})", display->GetName(), display->IsEnabled(), std::get<0>(display->GetCoordinates()), std::get<1>(display->GetCoordinates()));
		
		}

		return 0;
	}

	if (vm.count("enable")) {
		std::cout << "Enable: ";
		for (const auto& display : vm["enable"].as<std::vector<std::string>>())
		{
			std::cout << display << " ";
		}
		std::cout << "\n";
		somethingDone = true;
	}

	if (vm.count("disable"))
		{
		std::cout << "Disable: ";
		for (const auto& display : vm["disable"].as<std::vector<std::string>>())
		{
			std::cout << display << " ";
		}
		std::cout << "\n";
		somethingDone = true;
	}

	if (!somethingDone)
	{
		std::cout << optionsDesc << "\n";
		return 1;
	}

	return 0;
}
