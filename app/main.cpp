#include <boost/program_options.hpp>
#include <spdlog/spdlog.h>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>

namespace po = boost::program_options;

po::options_description CreateOptionsDescription()
{
	po::options_description desc("Allowed options");

	desc.add_options()
		("help", "produce help message")
		("enable", po::value<std::vector<std::string>>()->multitoken(), "Enable display")
		("disable", po::value<std::vector<std::string>>()->multitoken(), "Disable display")
		("enumerate", "Enumerate displays")
		;

	return desc;
}

int main(int argc, char** argv)
{	
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

	if (vm.count("enumerate")) 
	{
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

	if (vm.count("disable")) {
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
