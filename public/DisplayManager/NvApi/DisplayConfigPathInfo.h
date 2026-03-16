#pragma once

#include <any>
#include <vector>
#include "DisplayManager/NvApi/Init.h"

namespace DisplayManager::NvApi
{
    class DisplayConfigPathInfo
    {
    public:
        NvU32 SourceId = 0;
        NvU32 IsNonNVIDIAAdapter = 0;
        std::vector<NV_DISPLAYCONFIG_PATH_TARGET_INFO> TargetInfos;
        std::vector<NV_DISPLAYCONFIG_PATH_ADVANCED_TARGET_INFO> TargetInfosDetails;
        std::vector<NV_DISPLAYCONFIG_SOURCE_MODE_INFO> SourceModeInfos;
        std::any pOSAdapterID;

        NV_DISPLAYCONFIG_PATH_INFO ToUnmanaged();
    };
}