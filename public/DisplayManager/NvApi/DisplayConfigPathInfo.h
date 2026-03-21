#pragma once

#include <any>
#include <vector>

#include "DisplayManager/NvApi/Init.h"

namespace DisplayManager::NvApi
{
    class DisplayConfigPathInfo
    {
    public:
        DisplayConfigPathInfo() = default;

        DisplayConfigPathInfo(const DisplayConfigPathInfo& other)
            : SourceId(other.SourceId),
              IsNonNVIDIAAdapter(other.IsNonNVIDIAAdapter),
              TargetInfos(other.TargetInfos),
              TargetInfosDetails(other.TargetInfosDetails),
              SourceModeInfos(other.SourceModeInfos),
              pOSAdapterID(other.pOSAdapterID)
        {
            for (size_t i = 0; i < other.TargetInfos.size(); ++i)
            {
                TargetInfos[i].details = &TargetInfosDetails[i];
            }
        }

        DisplayConfigPathInfo& operator=(const DisplayConfigPathInfo& other)
        {
            if (this == &other)
                return *this;
            SourceId = other.SourceId;
            IsNonNVIDIAAdapter = other.IsNonNVIDIAAdapter;
            TargetInfos = other.TargetInfos;
            TargetInfosDetails = other.TargetInfosDetails;
            SourceModeInfos = other.SourceModeInfos;
            pOSAdapterID = other.pOSAdapterID;
            for (size_t i = 0; i < other.TargetInfos.size(); ++i)
            {
                TargetInfos[i].details = &TargetInfosDetails[i];
            }
            return *this;
        }

        NvU32 SourceId = 0;
        NvU32 IsNonNVIDIAAdapter = 0;
        std::vector<NV_DISPLAYCONFIG_PATH_TARGET_INFO> TargetInfos;
        std::vector<NV_DISPLAYCONFIG_PATH_ADVANCED_TARGET_INFO> TargetInfosDetails;
        std::vector<NV_DISPLAYCONFIG_SOURCE_MODE_INFO> SourceModeInfos;
        std::any pOSAdapterID;

        NV_DISPLAYCONFIG_PATH_INFO ToUnmanaged();

        bool operator==(const DisplayConfigPathInfo& rhs) const;
        bool operator!=(const DisplayConfigPathInfo& rhs) const;

    };
}