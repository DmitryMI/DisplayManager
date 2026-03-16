//
// Created by DmitriyPC on 16.03.2026.
//

#pragma once
#include <fstream>
#include <filesystem>
#include <boost/archive/text_oarchive.hpp>
#include <string>
#include "DisplayManager/Serialization/IOutputArchive.h"

namespace DisplayManager::Serialization
{
    class BoostOutputArchive : public IOutputArchive
    {
    public:
        explicit BoostOutputArchive(const std::filesystem::path& path);

        IOutputArchive& operator<<(std::int64_t value) override;
        IOutputArchive& operator<<(size_t value) override;
        IOutputArchive& operator<<(double value) override;

    private:
        std::ofstream m_Stream;
        boost::archive::text_oarchive m_OutArchive;
    };
}
