#include "DisplayManager/NvApi/DisplayConfigPathInfo.h"

namespace DisplayManager::NvApi
{
    NV_DISPLAYCONFIG_PATH_INFO DisplayConfigPathInfo::ToUnmanaged()
    {
        NV_DISPLAYCONFIG_PATH_INFO pathInfo {0};
        pathInfo.version = NV_DISPLAYCONFIG_PATH_INFO_VER;
        pathInfo.sourceId = SourceId;
        pathInfo.targetInfoCount = TargetInfos.size();
        pathInfo.targetInfo = TargetInfos.data();
        pathInfo.sourceModeInfo = SourceModeInfos.data();
        pathInfo.IsNonNVIDIAAdapter = IsNonNVIDIAAdapter;
        // pathInfo.pOSAdapterID
        return pathInfo;
    }
}