#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace DisplayManager::Serialization
{
    class IOutputArchive
    {
    public:
        virtual ~IOutputArchive() = default;

        virtual IOutputArchive& operator<<(std::int64_t value) = 0;
        virtual IOutputArchive& operator<<(size_t value) = 0;
        virtual IOutputArchive& operator<<(double value) = 0;
        virtual IOutputArchive& operator<<(const std::string& value) = 0;
        virtual IOutputArchive& operator<<(const std::vector<uint8_t>& value) = 0;
    };
} // Serialization
// DisplayManager
