#include "DisplayManager/NvApi/Configuration.h"
#include <cassert>
#include "DisplayManager/Serialization/IOutputArchive.h"
#include "DisplayManager/Serialization/IInputArchive.h"

#define min_c min
#undef min
#include <algorithm>

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
                archive << static_cast<std::int64_t>(targetInfo.displayId);
                Serialize(archive, displayConfig.TargetInfosDetails[i]);
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
                targetInfo.displayId = displayId;

                NV_DISPLAYCONFIG_PATH_ADVANCED_TARGET_INFO targetInfoDetails{0};
                Deserialize(archive, targetInfoDetails);
                displayConfig.TargetInfosDetails.push_back(targetInfoDetails);
                targetInfo.details = &displayConfig.TargetInfosDetails[displayConfig.TargetInfosDetails.size() - 1];

                archive >> targetId;
                targetInfo.targetId = targetId;

                displayConfig.TargetInfos.push_back(targetInfo);
            }

            displayConfig.SourceModeInfos.resize(1);
            auto& sourceMode = displayConfig.SourceModeInfos[0];
            std::size_t t;

            archive >> t; sourceMode.resolution.width = static_cast<NvU32>(t);
            archive >> t; sourceMode.resolution.height = static_cast<NvU32>(t);
            archive >> t; sourceMode.resolution.colorDepth = static_cast<NvU32>(t);
            archive >> t; sourceMode.colorFormat = static_cast<NV_FORMAT>(t);
            archive >> t; sourceMode.position.x = static_cast<NvS32>(t);
            archive >> t; sourceMode.position.y = static_cast<NvS32>(t);
            archive >> t; sourceMode.spanningOrientation = static_cast<NV_DISPLAYCONFIG_SPANNING_ORIENTATION>(t);
            archive >> t; sourceMode.bGDIPrimary = static_cast<NvU32>(t);
            archive >> t; sourceMode.bSLIFocus = static_cast<NvU32>(t);
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

    bool Configuration::operator==(const Configuration& rhs) const
    {
        if (m_DisplayConfigs.size() != rhs.m_DisplayConfigs.size())
        {
            return false;
        }

        for (size_t i = 0; i < m_DisplayConfigs.size(); i++)
        {
            if (m_DisplayConfigs[i] != rhs.m_DisplayConfigs[i])
            {
                return false;
            }
        }

        return true;
    }

    bool Configuration::operator!=(const Configuration& rhs) const
    {
        return !(*this == rhs);
    }

    void Configuration::Serialize(Serialization::IOutputArchive& archive,
                                  const NV_DISPLAYCONFIG_PATH_ADVANCED_TARGET_INFO& targetInfoDetails)
    {
        archive << static_cast<std::int64_t>(targetInfoDetails.rotation);
        archive << static_cast<std::int64_t>(targetInfoDetails.scaling);
        archive << static_cast<std::int64_t>(targetInfoDetails.refreshRate1K);
        archive << static_cast<std::size_t>(targetInfoDetails.interlaced);
        archive << static_cast<std::size_t>(targetInfoDetails.primary);
#ifdef NV_PAN_AND_SCAN_DEFINED
        archive << static_cast<std::size_t>(targetInfoDetails.isPanAndScanTarget);
#else
        archive << static_cast<std::size_t>(0);
#endif
        archive << static_cast<std::size_t>(targetInfoDetails.disableVirtualModeSupport);
        archive << static_cast<std::size_t>(targetInfoDetails.isPreferredUnscaledTarget);
        archive << static_cast<std::size_t>(targetInfoDetails.connector);
        archive << static_cast<std::size_t>(targetInfoDetails.tvFormat);
        archive << static_cast<std::size_t>(targetInfoDetails.timingOverride);
        Serialize(archive, targetInfoDetails.timing);
    }

    void Configuration::Serialize(Serialization::IOutputArchive& archive, const NV_TIMING& timingOverride)
    {
        archive << static_cast<std::size_t>(timingOverride.HVisible);
        archive << static_cast<std::size_t>(timingOverride.HBorder);
        archive << static_cast<std::size_t>(timingOverride.HFrontPorch);
        archive << static_cast<std::size_t>(timingOverride.HSyncWidth);
        archive << static_cast<std::size_t>(timingOverride.HTotal);
        archive << static_cast<std::size_t>(timingOverride.HSyncPol);
        archive << static_cast<std::size_t>(timingOverride.VVisible);
        archive << static_cast<std::size_t>(timingOverride.VBorder);
        archive << static_cast<std::size_t>(timingOverride.VFrontPorch);
        archive << static_cast<std::size_t>(timingOverride.VSyncWidth);
        archive << static_cast<std::size_t>(timingOverride.VTotal);
        archive << static_cast<std::size_t>(timingOverride.VSyncPol);
        archive << static_cast<std::size_t>(timingOverride.interlaced);
        archive << static_cast<std::size_t>(timingOverride.pclk);
        Serialize(archive, timingOverride.etc);
    }

    void Configuration::Serialize(Serialization::IOutputArchive& archive, const NV_TIMINGEXT& etc)
    {
        archive << static_cast<std::size_t>(etc.flag);
        archive << static_cast<std::size_t>(etc.rr);
        archive << static_cast<std::size_t>(etc.rrx1k);
        archive << static_cast<std::size_t>(etc.aspect);
        archive << static_cast<std::size_t>(etc.rep);
        archive << static_cast<std::size_t>(etc.status);
        std::vector<uint8_t> buffer(sizeof(etc.name));
        memcpy(buffer.data(), etc.name, sizeof(etc.name));
        archive << buffer;
    }

    void Configuration::Deserialize(Serialization::IInputArchive& archive,
                                    NV_DISPLAYCONFIG_PATH_ADVANCED_TARGET_INFO& targetInfoDetails)
    {
        targetInfoDetails.version = NV_DISPLAYCONFIG_PATH_ADVANCED_TARGET_INFO_VER;

        std::int64_t temp64;
        std::size_t  tempSize;

        archive >> temp64; targetInfoDetails.rotation = static_cast<NV_ROTATE>(temp64);
        archive >> temp64; targetInfoDetails.scaling = static_cast<NV_SCALING>(temp64);
        archive >> temp64; targetInfoDetails.refreshRate1K = static_cast<NvU32>(temp64);

        archive >> tempSize; targetInfoDetails.interlaced = static_cast<NvU32>(tempSize);
        archive >> tempSize; targetInfoDetails.primary = static_cast<NvU32>(tempSize);

#ifdef NV_PAN_AND_SCAN_DEFINED
        archive >> tempSize; targetInfoDetails.isPanAndScanTarget = static_cast<NvU32>(tempSize);
#else
        archive >> tempSize;
#endif

        archive >> tempSize; targetInfoDetails.disableVirtualModeSupport = static_cast<NvU32>(tempSize);
        archive >> tempSize; targetInfoDetails.isPreferredUnscaledTarget = static_cast<NvU32>(tempSize);
        archive >> tempSize; targetInfoDetails.connector = static_cast<NV_GPU_CONNECTOR_TYPE>(tempSize);
        archive >> tempSize; targetInfoDetails.tvFormat = static_cast<NV_DISPLAY_TV_FORMAT>(tempSize);
        archive >> tempSize; targetInfoDetails.timingOverride = static_cast<NV_TIMING_OVERRIDE>(tempSize);

        Deserialize(archive, targetInfoDetails.timing);
    }

    void Configuration::Deserialize(Serialization::IInputArchive& archive, NV_TIMING& timingOverride)
    {
        std::size_t t;

        archive >> t; timingOverride.HVisible = static_cast<NvU32>(t);
        archive >> t; timingOverride.HBorder = static_cast<NvU32>(t);
        archive >> t; timingOverride.HFrontPorch = static_cast<NvU32>(t);
        archive >> t; timingOverride.HSyncWidth = static_cast<NvU32>(t);
        archive >> t; timingOverride.HTotal = static_cast<NvU32>(t);
        archive >> t; timingOverride.HSyncPol = static_cast<NvU32>(t);
        archive >> t; timingOverride.VVisible = static_cast<NvU32>(t);
        archive >> t; timingOverride.VBorder = static_cast<NvU32>(t);
        archive >> t; timingOverride.VFrontPorch = static_cast<NvU32>(t);
        archive >> t; timingOverride.VSyncWidth = static_cast<NvU32>(t);
        archive >> t; timingOverride.VTotal = static_cast<NvU32>(t);
        archive >> t; timingOverride.VSyncPol = static_cast<NvU32>(t);
        archive >> t; timingOverride.interlaced = static_cast<NvU32>(t);
        archive >> t; timingOverride.pclk = static_cast<NvU32>(t);

        Deserialize(archive, timingOverride.etc);
    }

    void Configuration::Deserialize(Serialization::IInputArchive& archive, NV_TIMINGEXT& etc)
    {
        std::size_t t;

        archive >> t; etc.flag = static_cast<NvU32>(t);
        archive >> t; etc.rr = static_cast<NvU32>(t);
        archive >> t; etc.rrx1k = static_cast<NvU32>(t);
        archive >> t; etc.aspect = static_cast<NvU32>(t);
        archive >> t; etc.rep = static_cast<NvU32>(t);
        archive >> t; etc.status = static_cast<NvU32>(t);

        std::vector<uint8_t> nameBytes;
        archive >> nameBytes;

        std::ranges::fill(etc.name, 0);

        std::copy_n(nameBytes.begin(),
                    std::min(nameBytes.size(), sizeof(etc.name)),
                    etc.name);
    }
}

#define min min_c
#undef min_c