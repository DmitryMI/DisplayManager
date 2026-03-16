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
	auto displays = displayProvider->GetDisplays();

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
		for (const auto& display : displays)
		{
			std::string coordinatesStr = "(N/A)";
			const auto& coordinates = display->GetCoordinates();
			if (coordinates.has_value())
			{
				coordinatesStr = "(" + std::to_string(std::get<0>(coordinates.value())) + ", " + std::to_string(std::get<1>(coordinates.value())) + ")";
			}
			spdlog::info("Display: {}, Enabled: {}, Coordinates: {}", display->GetName(), display->IsEnabled(), coordinatesStr);
		
		}

		return 0;
	}

	if (vm.count("enable")) {
		for (const auto& displayName : vm["enable"].as<std::vector<std::string>>())
		{
			bool displayFound = false;
			for (auto& display : displays)
			{
				const auto name = display->GetName();
				if (name == displayName)
				{
					display->SetEnabled(true);
					spdlog::info("Display {}, Set Enabled: true", display->GetName());
					displayFound = true;
					break;
				}
			}
			if (!displayFound)
			{
				spdlog::error("Display with name {} was not found", displayName);
			}
		}
		somethingDone = true;
	}

	if (vm.count("disable"))
	{
		for (const auto& displayName : vm["disable"].as<std::vector<std::string>>())
		{
			bool displayFound = false;
			for (auto& display : displays)
			{
				const auto name = display->GetName();
				if (name == displayName)
				{
					display->SetEnabled(false);
					spdlog::info("Display {}, Set Enabled: false", display->GetName());
					displayFound = true;
					break;
				}
			}
			if (!displayFound)
			{
				spdlog::error("Display with name {} was not found", displayName);
			}
		}
		somethingDone = true;
	}

	if (!somethingDone)
	{
		std::cout << optionsDesc << "\n";
		return 1;
	}

	return 0;
}
