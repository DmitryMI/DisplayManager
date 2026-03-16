#pragma once

#include <any>
#include <vector>
#include "DisplayManager/NvApi/Init.h"

namespace DisplayManager::NvApi
{
    struct DisplayConfigPathInfo
    {
        NvU32 SourceId;
        NvU32 IsNonNVIDIAAdapter;
        std::vector<NV_DISPLAYCONFIG_PATH_TARGET_INFO> TargetInfos;
        std::vector<NV_DISPLAYCONFIG_PATH_ADVANCED_TARGET_INFO> TargetInfosDetails;
        std::vector<NV_DISPLAYCONFIG_SOURCE_MODE_INFO> SourceModeInfos;
        std::any pOSAdapterID;
    };
}