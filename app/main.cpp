#include <boost/program_options.hpp>
#include <spdlog/spdlog.h>

namespace po = boost::program_options;

po::options_description CreateOptionsDescription()
{
	po::options_description desc("Allowed options");

	desc.add_options()
		("help", "produce help message")
		("service-install", "install .service file. Use --config to specify path to config file.")
		("service-uninstall", "uninstall .service file")
		("config", po::value<std::string>(), "path to config file")
		;

	return desc;
}

int main()
{	
	spdlog::info("Display Manager started");
	po::options_description optionsDesc = CreateOptionsDescription();

	return 0;
}
