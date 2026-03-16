//
// Created by DmitriyPC on 16.03.2026.
//

#pragma once

namespace DisplayManager
{
    namespace Serialization
    {
        class IInputArchive;
        class IOutputArchive;
    }

    class IConfiguration
    {
    public:
        virtual ~IConfiguration() = default;
        virtual void Serialize(Serialization::IOutputArchive& archive) const = 0;
        virtual void Deserialize(Serialization::IInputArchive& archive) = 0;
    };
} // DisplayManager