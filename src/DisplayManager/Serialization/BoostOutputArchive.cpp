#include "DisplayManager/Serialization/BoostOutputArchive.h"
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

namespace DisplayManager::Serialization
{
    BoostOutputArchive::BoostOutputArchive(const std::filesystem::path& path):
        m_Stream(path),
        m_OutArchive(m_Stream)
    {
    }

    IOutputArchive& BoostOutputArchive::operator<<(std::int64_t value)
    {
        m_OutArchive << value;
        return *this;
    }

    IOutputArchive& BoostOutputArchive::operator<<(size_t value)
    {
        m_OutArchive << value;
        return *this;
    }

    IOutputArchive& BoostOutputArchive::operator<<(double value)
    {
        m_OutArchive << value;
        return *this;
    }

    IOutputArchive& BoostOutputArchive::operator<<(const std::string& value)
    {
        m_OutArchive << std::string(value);
        return *this;
    }

    IOutputArchive& BoostOutputArchive::operator<<(const std::vector<uint8_t>& value)
    {
        m_OutArchive << value;
        return *this;
    }
}
