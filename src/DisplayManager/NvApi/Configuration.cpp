#include "DisplayManager/NvApi/Configuration.h"

#include <cassert>

#include "DisplayManager/Serialization/IOutputArchive.h"
#include "DisplayManager/Serialization/IInputArchive.h"

namespace DisplayManager::NvApi
{
    void Configuration::Serialize(Serialization::IOutputArchive& archive) const
    {
        archive << m_DisplayConfigs.size();
        for (const auto& displayConfig : m_DisplayConfigs)
        {
            archive << static_cast<std::size_t>(displayConfig.SourceId);

            archive << displayConfig.TargetInfos.size();
            assert(displayConfig.TargetInfos.size() == displayConfig.TargetInfosDetails.size());
            for (size_t i = 0; i < displayConfig.TargetInfos.size(); i++)
            {
                const auto& targetInfo = displayConfig.TargetInfos[i];
                // const auto& targetInfoDetails = displayConfig.TargetInfosDetails[i];
                archive << static_cast<std::int64_t>(targetInfo.displayId);
                // archive << targetInfoDetails.
                archive << static_cast<std::int64_t>(targetInfo.targetId);
            }

            assert(displayConfig.SourceModeInfos.size() == 1);
            archive << static_cast<std::size_t>(displayConfig.SourceModeInfos[0].resolution.width);
            archive << static_cast<std::size_t>(displayConfig.SourceModeInfos[0].resolution.height);
            archive << static_cast<std::size_t>(displayConfig.SourceModeInfos[0].resolution.colorDepth);
            archive << static_cast<std::size_t>(displayConfig.SourceModeInfos[0].colorFormat);
            archive << static_cast<std::int64_t>(displayConfig.SourceModeInfos[0].position.x);
            archive << static_cast<std::int64_t>(displayConfig.SourceModeInfos[0].position.y);
            archive << static_cast<std::size_t>(displayConfig.SourceModeInfos[0].spanningOrientation);
            archive << static_cast<std::size_t>(displayConfig.SourceModeInfos[0].bGDIPrimary);
            archive << static_cast<std::size_t>(displayConfig.SourceModeInfos[0].bSLIFocus);
        }
    }

    void Configuration::Deserialize(Serialization::IInputArchive& archive)
    {
        m_DisplayConfigs.clear();

        size_t pathCount = 0;
        archive >> pathCount;

        m_DisplayConfigs.resize(pathCount);
        for (size_t i = 0; i < pathCount; i++)
        {
            auto& displayConfig = m_DisplayConfigs[i];
            size_t sourceId = 0;
            archive >> sourceId;
            displayConfig.SourceId = sourceId;

            size_t targetInfosCount = 0;
            archive >> targetInfosCount;
            for (size_t j = 0; j < targetInfosCount; j++)
            {
                NV_DISPLAYCONFIG_PATH_TARGET_INFO targetInfo{0};
                std::int64_t displayId = 0;
                std::int64_t targetId = 0;
                archive >> displayId;
                archive >> targetId;
                targetInfo.displayId = displayId;
                targetInfo.targetId = targetId;
                displayConfig.TargetInfos.push_back(targetInfo);
            }

            size_t resolutionWidth = 0;
            archive >> resolutionWidth;
            size_t resolutionHeight = 0;
            archive >> resolutionHeight;
            size_t resolutionColorDepth = 0;
            archive >> resolutionColorDepth;
            size_t colorFormat = 0;
            archive >> colorFormat;
            size_t positionX = 0;
            archive >> positionX;
            size_t positionY = 0;
            archive >> positionY;
            size_t spanningOrientation = 0;
            archive >> spanningOrientation;
            size_t bGDIPrimary = 0;
            archive >> bGDIPrimary;
            size_t bSLIFocus = 0;
            archive >> bSLIFocus;
            displayConfig.SourceModeInfos.resize(1);
            displayConfig.SourceModeInfos[0].resolution.width = resolutionWidth;
            displayConfig.SourceModeInfos[0].resolution.height = resolutionHeight;
            displayConfig.SourceModeInfos[0].resolution.colorDepth = resolutionColorDepth;
            displayConfig.SourceModeInfos[0].colorFormat = static_cast<NV_FORMAT>(colorFormat);
            displayConfig.SourceModeInfos[0].position.x = positionX;
            displayConfig.SourceModeInfos[0].position.y = positionY;
            displayConfig.SourceModeInfos[0].spanningOrientation = static_cast<NV_DISPLAYCONFIG_SPANNING_ORIENTATION>(spanningOrientation);
            displayConfig.SourceModeInfos[0].bGDIPrimary = bGDIPrimary;
            displayConfig.SourceModeInfos[0].bSLIFocus = bSLIFocus;
        }
    }

    const std::vector<DisplayConfigPathInfo>& Configuration::GetPathInfos() const
    {
        return m_DisplayConfigs;
    }

    std::vector<DisplayConfigPathInfo>& Configuration::GetPathInfos()
    {
        return m_DisplayConfigs;
    }
}