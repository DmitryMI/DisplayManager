#include <boost/program_options.hpp>
#include <spdlog/spdlog.h>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "DisplayManager/Factory.h"
#include "DisplayManager/IConfiguration.h"
#include "DisplayManager/IDisplayProvider.h"
#include "DisplayManager/Serialization/BoostInputArchive.h"
#include "DisplayManager/Serialization/BoostOutputArchive.h"

namespace po = boost::program_options;

using CommandArgs = std::vector<std::string>;
using CommandHandler = std::function<bool(
    DisplayManager::IDisplayProvider&,
    std::vector<std::shared_ptr<DisplayManager::IDisplay>>&,
    const CommandArgs&)>;

po::options_description CreateOptionsDescription()
{
    po::options_description desc("Allowed options");

    desc.add_options()
        ("help", "produce help message")
        ("command", po::value<std::string>(), "Command")
        ("args", po::value<std::vector<std::string>>(), "Command arguments")
        ("log-level", po::value<std::string>()->default_value("info"), "Log level");

    return desc;
}

void SetupLogging(const std::string& logLevel)
{
    spdlog::set_level(spdlog::level::from_str(logLevel));
    spdlog::set_pattern("[%Y-%b-%d %T.%e] [%l] %v");
}

bool CommandEnumerate(
    DisplayManager::IDisplayProvider&,
    const std::vector<std::shared_ptr<DisplayManager::IDisplay>>& displays,
    const CommandArgs&)
{
    for (const auto& display : displays)
    {
        std::string coordinatesStr = "(N/A)";
        const auto& coordinates = display->GetCoordinates();

        if (coordinates.has_value())
        {
            coordinatesStr =
                "(" + std::to_string(std::get<0>(*coordinates)) +
                ", " + std::to_string(std::get<1>(*coordinates)) + ")";
        }

        spdlog::info("Display: {}, Enabled: {}, Primary: {}, Coordinates: {}",
                     display->GetName(),
                     display->IsEnabled(),
                     display->IsPrimary(),
                     coordinatesStr);
    }

    return true;
}

bool CommandEnable(
    DisplayManager::IDisplayProvider&,
    const std::vector<std::shared_ptr<DisplayManager::IDisplay>>& displays,
    const CommandArgs& args)
{
    bool somethingDone = false;

    for (const auto& displayName : args)
    {
        bool displayFound = false;

        for (const auto& display : displays)
        {
            if (display->GetName() == displayName)
            {
                display->SetEnabled(true);
                spdlog::info("Display {}, Set Enabled: true", display->GetName());
                displayFound = true;
                somethingDone = true;
                break;
            }
        }

        if (!displayFound)
            spdlog::error("Display with name {} was not found", displayName);
    }

    return somethingDone;
}

bool CommandDisable(
    DisplayManager::IDisplayProvider&,
    const std::vector<std::shared_ptr<DisplayManager::IDisplay>>& displays,
    const CommandArgs& args)
{
    bool somethingDone = false;

    for (const auto& displayName : args)
    {
        bool displayFound = false;

        for (const auto& display : displays)
        {
            const auto name = display->GetName();
            if (name == displayName)
            {
                display->SetEnabled(false);
                spdlog::info("Display {}, Set Enabled: false", display->GetName());
                displayFound = true;
                somethingDone = true;
                break;
            }
        }

        if (!displayFound)
        {
            spdlog::error("Display with name {} was not found", displayName);
        }
    }

    return somethingDone;
}

bool CommandSave(
    const DisplayManager::IDisplayProvider& provider,
    std::vector<std::shared_ptr<DisplayManager::IDisplay>>&,
    const CommandArgs& args)
{
    if (args.empty())
    {
        spdlog::error("Save requires a file path");
        return false;
    }

    const auto& config = provider.GetActiveConfiguration();

    const std::filesystem::path savePath = std::filesystem::absolute(args[0]);

    spdlog::info("Saving configuration to {}", savePath.string());

    DisplayManager::Serialization::BoostOutputArchive archive(savePath);
    config.Serialize(archive);

    return true;
}

bool CommandLoad(
    DisplayManager::IDisplayProvider& provider,
    std::vector<std::shared_ptr<DisplayManager::IDisplay>>&,
    const CommandArgs& args)
{
    if (args.empty())
    {
        spdlog::error("Load requires a file path");
        return false;
    }

    const std::filesystem::path loadPath = std::filesystem::absolute(args[0]);

    spdlog::info("Loading configuration from {}", loadPath.string());

    if (!std::filesystem::exists(loadPath))
    {
        spdlog::error("Configuration file does not exist");
        return false;
    }

    DisplayManager::Serialization::BoostInputArchive archive(loadPath);
    const auto config = provider.DeserializeConfiguration(archive);

    if (!provider.ApplyConfiguration(*config))
    {
        spdlog::error("Failed to apply configuration");
        return false;
    }

    return true;
}

int main(int argc, char** argv)
{
    auto displayProvider = std::unique_ptr<DisplayManager::IDisplayProvider>(
        DisplayManager::Factory::GetDisplayProvider());

    auto displays = displayProvider->GetDisplays();

    auto optionsDesc = CreateOptionsDescription();

    po::positional_options_description pos;
    pos.add("command", 1);
    pos.add("args", -1);

    po::variables_map vm;

    po::store(
        po::command_line_parser(argc, argv)
            .options(optionsDesc)
            .positional(pos)
            .run(),
        vm);

    po::notify(vm);

    if (vm.contains("help") || !vm.contains("command"))
    {
        std::cout << optionsDesc << "\n";
        return 0;
    }

    SetupLogging(vm["log-level"].as<std::string>());
    spdlog::info("Log level set to {}", vm["log-level"].as<std::string>());

    std::string command = vm["command"].as<std::string>();

    CommandArgs args;
    if (vm.contains("args"))
        args = vm["args"].as<std::vector<std::string>>();

    std::unordered_map<std::string, CommandHandler> commands =
    {
        {"enumerate", CommandEnumerate},
        {"enable", CommandEnable},
        {"disable", CommandDisable},
        {"save", CommandSave},
        {"load", CommandLoad},
    };

    auto it = commands.find(command);

    if (it == commands.end())
    {
        spdlog::error("Unknown command: {}", command);
        return 1;
    }

    bool result = it->second(*displayProvider, displays, args);

    return result ? 0 : 1;
}
