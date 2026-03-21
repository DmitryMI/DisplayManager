#pragma once
#include <vector>
#include "DisplayManager/IConfiguration.h"
#include "DisplayManager/NvApi/DisplayConfigPathInfo.h"

namespace DisplayManager::NvApi
{
    class DisplayConfigPathInfo;

    class Configuration : public IConfiguration
    {
    public:
        void Serialize(Serialization::IOutputArchive& archive) const override;
        void Deserialize(Serialization::IInputArchive& archive) override;

        const std::vector<DisplayConfigPathInfo>& GetPathInfos() const;
        std::vector<DisplayConfigPathInfo>& GetPathInfos();

        bool operator==(const Configuration& rhs) const;
        bool operator!=(const Configuration& rhs) const;

    private:
        std::vector<DisplayConfigPathInfo> m_DisplayConfigs;

        static void Serialize(Serialization::IOutputArchive& archive, const NV_DISPLAYCONFIG_PATH_ADVANCED_TARGET_INFO& targetInfoDetails);
        static void Serialize(Serialization::IOutputArchive& archive, const NV_TIMING& timingOverride);
        static void Serialize(Serialization::IOutputArchive& archive, const NV_TIMINGEXT& etc);

        static void Deserialize(Serialization::IInputArchive& archive, NV_DISPLAYCONFIG_PATH_ADVANCED_TARGET_INFO& targetInfoDetails);
        static void Deserialize(Serialization::IInputArchive& archive, NV_TIMING& timingOverride);
        static void Deserialize(Serialization::IInputArchive& archive, NV_TIMINGEXT& etc);
    };
}
