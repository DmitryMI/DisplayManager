//
// Created by DmitriyPC on 16.03.2026.
//

#include "DisplayManager/Serialization/BoostOutputArchive.h"

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

}
