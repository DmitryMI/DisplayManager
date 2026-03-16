#pragma once

#include "DisplayManager/IDisplayProvider.h"
#include "DisplayManager/NvApi/Init.h"
#include "DisplayManager/NvApi/DisplayConfigPathInfo.h"
#include <unordered_map>

namespace DisplayManager::NvApi
{
	class Display;

	class DisplayProvider : public IDisplayProvider
	{
	public:
		DisplayProvider();
		virtual ~DisplayProvider() = default;

		std::vector<std::shared_ptr<IDisplay>> GetDisplays() override;
		void Refresh() override;

		static std::string GetDisplayName(NvPhysicalGpuHandle gpuHandle, NvU32 id);

	private:
		std::unordered_map<NvU32, std::shared_ptr<Display>> m_Displays;
		std::vector<NV_GPU_DISPLAYIDS> m_DisplayIds;
		std::vector<DisplayManager::NvApi::DisplayConfigPathInfo> m_DisplayConfigs;

		void RefreshInternal();
	};
}