//
// Created by DmitriyPC on 16.03.2026.
//

#pragma once
#include <cstdint>
#include <string>

namespace DisplayManager::Serialization
{
    class IOutputArchive
    {
    public:
        virtual ~IOutputArchive() = default;

        virtual IOutputArchive& operator<<(std::int64_t value) = 0;
        virtual IOutputArchive& operator<<(size_t value) = 0;
        virtual IOutputArchive& operator<<(double value) = 0;
    };
} // Serialization
// DisplayManager
