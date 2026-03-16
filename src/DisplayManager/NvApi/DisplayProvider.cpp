#include "DisplayManager/NvApi/DisplayProvider.h"
#include <algorithm>
#include <stdexcept>
#include <array>
#include <cassert>
#include <string>
#include <memory>
#include <ranges>
#include "DisplayManager/NvApi/Display.h"
#include <boost/algorithm/string.hpp>

namespace
{
    std::vector<NvPhysicalGpuHandle> EnumeratePhysicalGpus()
    {
        std::array<NvPhysicalGpuHandle, NVAPI_MAX_PHYSICAL_GPUS> hPhysicalGpu{nullptr};
        NvU32 physicalGpuCount = 0;
        if (const auto ret = NvAPI_EnumPhysicalGPUs(hPhysicalGpu.data(), &physicalGpuCount); ret != NVAPI_OK)
        {
            throw std::runtime_error("Failed to enumerate physical GPUs: " + std::to_string(ret));
        }
        return {hPhysicalGpu.begin(), hPhysicalGpu.begin() + physicalGpuCount};
    }

    std::vector<NV_GPU_DISPLAYIDS> GetConnectedDisplayIds(NvPhysicalGpuHandle gpuHandle)
    {
        assert(gpuHandle);

        NvU32 nDisplayIds = 0;
        auto ret = NvAPI_GPU_GetConnectedDisplayIds(gpuHandle, nullptr, &nDisplayIds, 0);
        if (ret != NVAPI_OK)
        {
            throw std::runtime_error("Failed to retrieve number of display IDs: " + std::to_string(ret));
        }
        if (!nDisplayIds)
        {
            return {};
        }

        std::vector<NV_GPU_DISPLAYIDS> displayIds(nDisplayIds);
        displayIds.resize(nDisplayIds);
        for (auto& displayId : displayIds)
        {
            displayId.version = NV_GPU_DISPLAYIDS_VER;
        }
        ret = NvAPI_GPU_GetConnectedDisplayIds(gpuHandle, displayIds.data(), &nDisplayIds, 0);
        if (ret != NVAPI_OK)
        {
            throw std::runtime_error("Failed to retrieve display IDs: " + std::to_string(ret));
        }

        return displayIds;
    }

    std::vector<DisplayManager::NvApi::DisplayConfigPathInfo> GetDisplayConfiguration()
    {
        NvU32 pathInfoCount = 0;
        auto ret = NvAPI_DISP_GetDisplayConfig(&pathInfoCount, nullptr);
        if (ret != NVAPI_OK)
        {
            throw std::runtime_error("Failed to get display config: " + std::to_string(ret));
        }

        std::vector<DisplayManager::NvApi::DisplayConfigPathInfo> pathInfosManaged(pathInfoCount);
        pathInfosManaged.resize(pathInfoCount);

        std::vector<NV_DISPLAYCONFIG_PATH_INFO> pathInfos(pathInfoCount);
        pathInfos.resize(pathInfoCount);
        for (auto& pathInfo : pathInfos)
        {
            pathInfo.version = NV_DISPLAYCONFIG_PATH_INFO_VER;
        }

        ret = NvAPI_DISP_GetDisplayConfig(&pathInfoCount, pathInfos.data());
        if (ret != NVAPI_OK)
        {
            throw std::runtime_error("Failed to get display config: " + std::to_string(ret));
        }

        for (NvU32 i = 0; i < pathInfos.size(); i++)
        {
            if (pathInfos[i].version == NV_DISPLAYCONFIG_PATH_INFO_VER1 || pathInfos[i].version ==
                NV_DISPLAYCONFIG_PATH_INFO_VER2)
            {
                // pathInfos[i].sourceModeInfo = (NV_DISPLAYCONFIG_SOURCE_MODE_INFO*) malloc(sizeof(NV_DISPLAYCONFIG_SOURCE_MODE_INFO));
                pathInfosManaged[i].SourceModeInfos.resize(1);
            }
            else
            {
#ifdef NV_DISPLAYCONFIG_PATH_INFO_VER3
                pathInfosManaged[i].SourceModeInfos.resize(pathInfos[i].sourceModeInfoCount);
#endif
            }
            pathInfos[i].sourceModeInfo = pathInfosManaged[i].SourceModeInfos.data();

            pathInfosManaged[i].TargetInfos.resize(pathInfos[i].targetInfoCount);
            pathInfos[i].targetInfo = pathInfosManaged[i].TargetInfos.data();
            pathInfosManaged[i].TargetInfosDetails.resize(pathInfos[i].targetInfoCount);

            for (NvU32 j = 0; j < pathInfos[i].targetInfoCount; j++)
            {
                pathInfos[i].targetInfo[j].details = &pathInfosManaged[i].TargetInfosDetails[j];
                pathInfos[i].targetInfo[j].details->version = NV_DISPLAYCONFIG_PATH_ADVANCED_TARGET_INFO_VER;
            }
        }

        ret = NvAPI_DISP_GetDisplayConfig(&pathInfoCount, pathInfos.data());
        if (ret != NVAPI_OK)
        {
            throw std::runtime_error("Failed to get display config: " + std::to_string(ret));
        }

        return pathInfosManaged;
    }

    NvDisplayHandle GetDisplayHandleFromId(NvPhysicalGpuHandle gpuHandle, NvU32 targetDisplayId)
    {
        NvU32 targetOutputId = 0;

        NvAPI_Status status = NvAPI_SYS_GetGpuAndOutputIdFromDisplayId(targetDisplayId, &gpuHandle, &targetOutputId);
        if (status != NVAPI_OK)
        {
            throw std::runtime_error("Failed to get GPU and Output ID: " + std::to_string(status));
            return nullptr;
        }

        // 2. Enumerate all active NvDisplayHandles
        NvDisplayHandle hDisplay = nullptr;
        for (NvU32 i = 0; NvAPI_EnumNvidiaDisplayHandle(i, &hDisplay) == NVAPI_OK; ++i)
        {
            // 3a. Get the Output ID for this specific display handle
            NvU32 currentOutputId = 0;
            status = NvAPI_GetAssociatedDisplayOutputId(hDisplay, &currentOutputId);
            if (status != NVAPI_OK) continue;

            // 3b. Get the Physical GPU(s) driving this display handle
            NvPhysicalGpuHandle currentGpus[NVAPI_MAX_PHYSICAL_GPUS] = {nullptr};
            NvU32 gpuCount = 0;
            status = NvAPI_GetPhysicalGPUsFromDisplay(hDisplay, currentGpus, &gpuCount);
            if (status != NVAPI_OK) continue;

            // 4. Check if both the Output ID and the Physical GPU match
            bool gpuMatches = false;
            for (NvU32 j = 0; j < gpuCount; ++j)
            {
                if (currentGpus[j] == gpuHandle)
                {
                    gpuMatches = true;
                    break;
                }
            }

            // If both the physical GPU and the connector output ID match, we found it.
            if (gpuMatches && currentOutputId == targetOutputId)
            {
                return hDisplay;
            }
        }

        // No matching display handle was found (display might be inactive/unattached)
        return nullptr;
    }

    NV_EDID GetDisplayEdid(NvU32 displayId)
    {
        // 1. Get the Physical GPU handle for this Display ID
        NvPhysicalGpuHandle hGpu = nullptr;
        NvU32 outputId = 0;
        if (const auto ret = NvAPI_SYS_GetGpuAndOutputIdFromDisplayId(displayId, &hGpu, &outputId); ret != NVAPI_OK)
        {
            throw std::runtime_error("NvAPI_SYS_GetGpuAndOutputIdFromDisplayId() failed: " + std::to_string(ret));
        }

        // 2. Query the EDID from NVAPI
        NV_EDID edidData = {0};
        edidData.version = NV_EDID_VER;
        if (const auto ret = NvAPI_GPU_GetEDID(hGpu, displayId, &edidData); ret != NVAPI_OK)
        {
            throw std::runtime_error("NvAPI_GPU_GetEDID() failed: " + std::to_string(ret));
        }
        return edidData;
    }

    std::string GetManufacturerFromEdid(const NV_EDID& edid)
    {
        // The Manufacturer ID is at offset 8 and 9
        const unsigned char b1 = edid.EDID_Data[8];
        const unsigned char b2 = edid.EDID_Data[9];

        // Bit-shifting to extract three 5-bit groups
        // Letter 1: Bits 6-2 of Byte 8
        const char c1 = ((b1 & 0x7C) >> 2) + 64;
        // Letter 2: Bits 1-0 of Byte 8 and Bits 7-5 of Byte 9
        const char c2 = (((b1 & 0x03) << 3) | ((b2 & 0xE0) >> 5)) + 64;
        // Letter 3: Bits 4-0 of Byte 9
        const char c3 = (b2 & 0x1F) + 64;

        std::string result;
        result += c1;
        result += c2;
        result += c3;

        return boost::trim_copy(result); // Returns strings like "SAM", "DEL", "VSC", etc.
    }

    std::string GetModelNameFromEdid(const NV_EDID& edid)
    {
        for (int i = 0; i < 4; i++)
        {
            const int offset = 54 + (i * 18);

            // Header for a 'Display Product Name' descriptor is 00 00 00 FC 00
            if (edid.EDID_Data[offset] == 0x00 &&
                edid.EDID_Data[offset + 1] == 0x00 &&
                edid.EDID_Data[offset + 3] == 0xFC)
            {
                std::string modelName;
                for (int j = 5; j < 18; j++)
                {
                    const char c = (char)edid.EDID_Data[offset + j];
                    if (c == 0x0A || c == 0x20 && modelName.empty()) continue; // Skip newlines/leading space
                    if (c == 0x0A) break; // Stop at newline
                    modelName += c;
                }
                return boost::trim_copy(modelName);
            }
        }

        throw std::runtime_error("Model Name Not Found in EDID");
    }

    NV_RESOLUTION GetNativeResolutionFromEdid(const NV_EDID& edid)
    {
        NV_RESOLUTION resolution = {0, 0, 32};

        // --- 1. Parse Resolution (Detailed Timing Descriptor at Offset 54) ---
        const unsigned char* dtd = &edid.EDID_Data[54];

        // Check if DTD is valid (Pixel Clock check)
        if (dtd[0] != 0 || dtd[1] != 0)
        {
            resolution.width = ((dtd[4] & 0xF0) << 4) | dtd[2];
            resolution.height = ((dtd[7] & 0xF0) << 4) | dtd[5];
        }
        return resolution;
    }
}

namespace DisplayManager::NvApi
{
    DisplayProvider::DisplayProvider()
    {
        // ReSharper disable once CppDFAConstantConditions
        if (!Init::GetInstance().IsInitialized())
        {
            // ReSharper disable once CppDFAUnreachableCode
            throw std::runtime_error("NVAPI is not initialized");
        }

        const auto configs = GetDisplayConfiguration();

        RefreshInternal();
    }


    std::vector<std::shared_ptr<IDisplay>> DisplayProvider::GetDisplays()
    {
        std::vector<std::shared_ptr<IDisplay>> result(m_Displays.size());
        std::ranges::transform(m_Displays, result.begin(),
                               [](const auto& pair)
                               {
                                   return std::dynamic_pointer_cast<IDisplay>(pair.second);
                               });
        return result;
    }

    void DisplayProvider::Refresh()
    {
        RefreshInternal();
    }

    std::string DisplayProvider::GetDisplayName(NvU32 id)
    {
        const auto edid = GetDisplayEdid(id);
        return GetManufacturerFromEdid(edid) + " " + GetModelNameFromEdid(edid);
    }

    std::optional<std::tuple<int, int>> DisplayProvider::GetDisplayCoordinates(NvU32 id) const
    {
        for (const auto& pathInfo : m_Configuration.GetPathInfos())
        {
            for (const auto& targetInfo : pathInfo.TargetInfos)
            {
                if (targetInfo.displayId == id)
                {
                    const auto position = pathInfo.SourceModeInfos[0].position;
                    return std::make_tuple(position.x, position.y);
                }
            }
        }

        return std::nullopt;
    }

    bool DisplayProvider::IsDisplayEnabled(NvU32 id) const
    {
        for (const auto& displayId : m_DisplayIds)
        {
            if (displayId.displayId == id)
            {
                return displayId.isActive;
            }
        }

        throw std::runtime_error("Failed to find DisplayID (struct) info for display id " + std::to_string(id));
    }

    void DisplayProvider::SetDisplayEnabled(NvU32 id, bool enabledSet)
    {
        const auto pathIter = FindPathByDisplayId(id);
        const bool currentlyEnabled = pathIter != m_Configuration.GetPathInfos().end();
        if (enabledSet == currentlyEnabled)
        {
            return;
        }

        if (enabledSet)
        {
            DisplayConfigPathInfo pathInfoNew;
            pathInfoNew.SourceId = 0;
            NV_DISPLAYCONFIG_PATH_TARGET_INFO targetInfoNew{0};
            targetInfoNew.displayId = id;
            pathInfoNew.TargetInfos = {targetInfoNew};

            NV_DISPLAYCONFIG_SOURCE_MODE_INFO sourceModeInfoNew{0};
            // sourceModeInfoNew.resolution = {3840, 2160, 32};
            const auto edid = GetDisplayEdid(id);
            sourceModeInfoNew.resolution = GetNativeResolutionFromEdid(edid);
            pathInfoNew.SourceModeInfos = {sourceModeInfoNew};

            m_Configuration.GetPathInfos().push_back(pathInfoNew);
        }
        else
        {
            if (pathIter->SourceModeInfos[0].bGDIPrimary)
            {
                throw std::runtime_error("Safety Fault: Cannot disable the Primary Display.");
            }

            m_Configuration.GetPathInfos().erase(pathIter);
        }

        std::vector<NV_DISPLAYCONFIG_PATH_INFO> pathInfos;
        pathInfos.reserve(m_Configuration.GetPathInfos().size());
        for (auto& managedPath : m_Configuration.GetPathInfos())
        {
            const NV_DISPLAYCONFIG_PATH_INFO unmanagedPathInfo = managedPath.ToUnmanaged();
            pathInfos.push_back(unmanagedPathInfo);
        }

        // Apply the new topology
        const auto status = NvAPI_DISP_SetDisplayConfig(static_cast<NvU32>(pathInfos.size()), pathInfos.data(), 0);

        if (status != NVAPI_OK)
        {
            throw std::runtime_error("NvAPI_DISP_SetDisplayConfig failed with code: " + std::to_string(status));
        }

        m_Configuration.GetPathInfos() = GetDisplayConfiguration();
    }

    // https://github.com/NVIDIA/nvapi/blob/main/Sample_Code/DisplayConfiguration/DisplayConfiguration.cpp
    void DisplayProvider::RefreshInternal()
    {
        m_DisplayIds.clear();
        for (const auto& gpuHandle : EnumeratePhysicalGpus())
        {
            for (const auto& displayId : GetConnectedDisplayIds(gpuHandle))
            {
                m_DisplayIds.push_back(displayId);
                if (m_Displays.contains(displayId.displayId))
                {
                    const auto& display = m_Displays[displayId.displayId];
                    display->m_DisplayId = displayId.displayId;
                    display->m_GpuHandle = gpuHandle;
                }
                else
                {
                    m_Displays[displayId.displayId] = std::make_shared<Display>(this, gpuHandle, displayId.displayId);
                }
            }
        }

        for (const auto displaysCopy = m_Displays; const auto& id : displaysCopy | std::views::keys)
        {
            auto iter = std::ranges::find_if(m_DisplayIds, [id](const auto& displayId)
            {
                return displayId.displayId == id;
            });
            if (iter != m_DisplayIds.end())
            {
                continue;
            }

            m_Displays.erase(iter->displayId);
        }

        m_Configuration.GetPathInfos() = GetDisplayConfiguration();
    }

    std::unique_ptr<IConfiguration> DisplayProvider::DeserializeConfiguration(
        Serialization::IInputArchive& archive) const
    {
        auto config = std::make_unique<Configuration>();
        config->Deserialize(archive);
        return config;
    }

    const IConfiguration& DisplayProvider::GetActiveConfiguration() const
    {
        return m_Configuration;
    }

    bool DisplayProvider::ApplyConfiguration(const IConfiguration& configuration)
    {
        const auto* configNew = dynamic_cast<const Configuration*>(&configuration);
        if (!configNew)
        {
            throw std::runtime_error("Configuration type mismatch");
        }

        m_Configuration.GetPathInfos() = configNew->GetPathInfos();

        std::vector<NV_DISPLAYCONFIG_PATH_INFO> pathInfos;
        pathInfos.reserve(m_Configuration.GetPathInfos().size());
        for (auto& managedPath : m_Configuration.GetPathInfos())
        {
            const NV_DISPLAYCONFIG_PATH_INFO unmanagedPathInfo = managedPath.ToUnmanaged();
            pathInfos.push_back(unmanagedPathInfo);
        }

        // Apply the new topology
        const auto status = NvAPI_DISP_SetDisplayConfig(static_cast<NvU32>(pathInfos.size()), pathInfos.data(), 0);

        m_Configuration.GetPathInfos() = GetDisplayConfiguration();
        return status == NVAPI_OK;
    }
}
