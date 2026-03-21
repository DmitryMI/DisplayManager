#include "DisplayManager/NvApi/DisplayConfigPathInfo.h"

#include <string>

namespace DisplayManager::NvApi
{
    NV_DISPLAYCONFIG_PATH_INFO DisplayConfigPathInfo::ToUnmanaged()
    {
        NV_DISPLAYCONFIG_PATH_INFO pathInfo{0};
        pathInfo.version = NV_DISPLAYCONFIG_PATH_INFO_VER;
        pathInfo.sourceId = SourceId;
        pathInfo.targetInfoCount = TargetInfos.size();
        pathInfo.targetInfo = TargetInfos.data();
        if (TargetInfos.size() == TargetInfosDetails.size())
        {
            pathInfo.targetInfo->details = TargetInfosDetails.data();
        }
        pathInfo.sourceModeInfo = SourceModeInfos.data();
        pathInfo.IsNonNVIDIAAdapter = IsNonNVIDIAAdapter;
        // pathInfo.pOSAdapterID
        return pathInfo;
    }

    bool DisplayConfigPathInfo::operator==(const DisplayConfigPathInfo& rhs) const
    {
        if (SourceId != rhs.SourceId) return false;
        if (IsNonNVIDIAAdapter != rhs.IsNonNVIDIAAdapter) return false;

        if (TargetInfos.size() != rhs.TargetInfos.size()) return false;
        if (TargetInfosDetails.size() != rhs.TargetInfosDetails.size()) return false;
        for (size_t i = 0; i < TargetInfos.size(); ++i)
        {
            const auto& myInfo = TargetInfos[i];
            const auto& otherInfo = rhs.TargetInfos[i];
            if (myInfo.targetId != otherInfo.targetId) return false;
            if (myInfo.displayId != otherInfo.displayId) return false;
            const auto& myInfoDetails = myInfo.details;
            const auto& otherInfoDetails = otherInfo.details;
            if (myInfoDetails->version != otherInfoDetails->version) return false;
            if (myInfoDetails->rotation != otherInfoDetails->rotation) return false;
            if (myInfoDetails->scaling != otherInfoDetails->scaling) return false;
            if (myInfoDetails->refreshRate1K != otherInfoDetails->refreshRate1K) return false;
            if (myInfoDetails->interlaced != otherInfoDetails->interlaced) return false;
            if (myInfoDetails->primary != otherInfoDetails->primary) return false;
#ifdef NV_PAN_AND_SCAN_DEFINED
            if (myInfoDetails->isPanAndScanTarget != otherInfoDetails->isPanAndScanTarget) return false;
#endif
            if (myInfoDetails->disableVirtualModeSupport != otherInfoDetails->disableVirtualModeSupport) return false;
            if (myInfoDetails->isPreferredUnscaledTarget != otherInfoDetails->isPreferredUnscaledTarget) return false;
            if (myInfoDetails->connector != otherInfoDetails->connector) return false;
            if (myInfoDetails->tvFormat != otherInfoDetails->tvFormat) return false;
            if (myInfoDetails->timingOverride != otherInfoDetails->timingOverride) return false;
            const auto& myTiming = myInfoDetails->timing;
            const auto& otherTiming = otherInfoDetails->timing;
            if (myTiming.HVisible != otherTiming.HVisible) return false;
            if (myTiming.HBorder != otherTiming.HBorder) return false;
            if (myTiming.HFrontPorch != otherTiming.HFrontPorch) return false;
            if (myTiming.HSyncWidth != otherTiming.HSyncWidth) return false;
            if (myTiming.HTotal != otherTiming.HTotal) return false;
            if (myTiming.HSyncPol != otherTiming.HSyncPol) return false;
            if (myTiming.VVisible != otherTiming.VVisible) return false;
            if (myTiming.VBorder != otherTiming.VBorder) return false;
            if (myTiming.VFrontPorch != otherTiming.VFrontPorch) return false;
            if (myTiming.VSyncWidth != otherTiming.VSyncWidth) return false;
            if (myTiming.VTotal != otherTiming.VTotal) return false;
            if (myTiming.VSyncPol != otherTiming.VSyncPol) return false;
            if (myTiming.interlaced != otherTiming.interlaced) return false;
            if (myTiming.pclk != otherTiming.pclk) return false;
            const auto& myEtc = myTiming.etc;
            const auto& otherEtc = otherTiming.etc;
            if (myEtc.flag != otherEtc.flag) return false;
            if (myEtc.rr != otherEtc.rr) return false;
            if (myEtc.rrx1k != otherEtc.rrx1k) return false;
            if (myEtc.aspect != otherEtc.aspect) return false;
            if (myEtc.rep != otherEtc.rep) return false;
            if (myEtc.status != otherEtc.status) return false;
            std::string myEtcName = std::string(reinterpret_cast<const char*>(myEtc.name), sizeof(myEtc.name));
            std::string otherEtcName = std::string(reinterpret_cast<const char*>(otherEtc.name), sizeof(otherEtc.name));
            if (myEtcName != otherEtcName) return false;
        }

        if (SourceModeInfos.size() != rhs.SourceModeInfos.size()) return false;
        for (size_t i = 0; i < SourceModeInfos.size(); ++i)
        {
            const auto& mySourceMode = SourceModeInfos[i];
            const auto& otherSourceMode = rhs.SourceModeInfos[i];
            if (mySourceMode.resolution.width != otherSourceMode.resolution.width) return false;
            if (mySourceMode.resolution.height != otherSourceMode.resolution.height) return false;
            if (mySourceMode.resolution.colorDepth != otherSourceMode.resolution.colorDepth) return false;
            if (mySourceMode.colorFormat != otherSourceMode.colorFormat) return false;
            if (mySourceMode.position.x != otherSourceMode.position.x) return false;
            if (mySourceMode.position.y != otherSourceMode.position.y) return false;
            if (mySourceMode.spanningOrientation != otherSourceMode.spanningOrientation) return false;
            if (mySourceMode.bGDIPrimary != otherSourceMode.bGDIPrimary) return false;
            if (mySourceMode.bSLIFocus != otherSourceMode.bSLIFocus) return false;

        }

        if (pOSAdapterID.has_value() != rhs.pOSAdapterID.has_value()) return false;
        // FIXME pOsAdapterId not checked.

        return true;
    }

    bool DisplayConfigPathInfo::operator!=(const DisplayConfigPathInfo& rhs) const
    {
        return !(*this == rhs);
    }
}
