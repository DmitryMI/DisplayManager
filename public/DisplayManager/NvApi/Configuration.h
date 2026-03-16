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

    private:
        std::vector<DisplayConfigPathInfo> m_DisplayConfigs;
    };
}
