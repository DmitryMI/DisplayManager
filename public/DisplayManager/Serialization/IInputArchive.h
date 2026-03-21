//
// Created by DmitriyPC on 16.03.2026.
//

#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace DisplayManager::Serialization
{
    class IInputArchive
    {
    public:
        virtual ~IInputArchive() = default;

        virtual IInputArchive& operator>>(std::int64_t& value) = 0;
        virtual IInputArchive& operator>>(size_t& value) = 0;
        virtual IInputArchive& operator>>(double& value) = 0;
        virtual IInputArchive& operator>>(std::string& value) = 0;
        virtual IInputArchive& operator>>(std::vector<uint8_t>& value) = 0;
    };
} // Serialization
// DisplayManager
