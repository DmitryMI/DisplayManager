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
		("ip", po::value<std::string>()->default_value("10.10.0.1"), "listening IP")
		("port", po::value<uint16_t>()->default_value(10001), "listening port of Sensor Collector")
		("port-time", po::value<uint16_t>(), "listening port of Time Provider")
		("console-log-only", "allows to disable logging to file")
		("log-dir", po::value<std::string>(), "logging directory")
		("log-level", po::value<std::string>()->default_value("INFO"), "logging level")
		("force-mkdirs", "create missing directories")
		("registry-provider", po::value<std::string>(), "Sensor Registry driver name. Supported providers: csv, mysql")
		("registry-string", po::value<std::string>(), "Sensor Registry Connection String")
		("database-provider", po::value<std::string>(), "Sensor Database driver name. Supported providers: json, mysql, influxdb")
		("database-string", po::value<std::string>(), "Sensor Database Connection String passed to selected provider")
		("psk", po::value<std::string>(), "Pre-shared encryption key")
		("psk-path", po::value<std::string>(), "Path to a text file containing pre-shared encryption key")
		;

	return desc;
}

int main()
{	
	spdlog::info("Hello World!");
	po::options_description optionsDesc = CreateOptionsDescription();

	return 0;
}
