#pragma once
#include "DisplayManager/Serialization/IInputArchive.h"
#include <boost/archive/text_iarchive.hpp>
#include <fstream>
#include <filesystem>
#include <string>

namespace DisplayManager::Serialization
{
    class BoostInputArchive : public IInputArchive
    {
    public:
        explicit BoostInputArchive(const std::filesystem::path& path);

        IInputArchive& operator>>(std::int64_t& value) override;
        IInputArchive& operator>>(size_t& value) override;
        IInputArchive& operator>>(double& value) override;
        IInputArchive& operator>>(std::string& value) override;
        IInputArchive& operator>>(std::vector<uint8_t>& value) override;

    private:
        std::ifstream m_Stream;
        boost::archive::text_iarchive m_InArchive;
    };
}
