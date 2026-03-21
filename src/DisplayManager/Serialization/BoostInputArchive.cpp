#include "DisplayManager/Serialization/BoostInputArchive.h"
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

namespace DisplayManager::Serialization
{
    BoostInputArchive::BoostInputArchive(const std::filesystem::path& path):
        m_Stream(path),
        m_InArchive(m_Stream)
    {

    }

    IInputArchive& BoostInputArchive::operator>>(std::int64_t& value)
    {
        m_InArchive >> value;
        return *this;
    }

    IInputArchive& BoostInputArchive::operator>>(size_t& value)
    {
        m_InArchive >> value;
        return *this;
    }

    IInputArchive& BoostInputArchive::operator>>(double& value)
    {
        m_InArchive >> value;
        return *this;
    }

    IInputArchive& BoostInputArchive::operator>>(std::string& value)
    {
        m_InArchive >> value;
        return *this;
    }

    IInputArchive& BoostInputArchive::operator>>(std::vector<uint8_t>& value)
    {
        m_InArchive >> value;
        return *this;
    }
}
