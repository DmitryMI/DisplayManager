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

		static std::string GetDisplayName(NvU32 id);
		std::optional<std::tuple<int, int>> GetDisplayCoordinates(NvU32 id) const;
		bool IsDisplayEnabled(NvU32 id) const;
		void SetDisplayEnabled(NvU32 id, bool enabledSet);

	private:
		std::unordered_map<NvU32, std::shared_ptr<Display>> m_Displays;
		std::vector<NV_GPU_DISPLAYIDS> m_DisplayIds;
		std::vector<DisplayConfigPathInfo> m_DisplayConfigs;

		auto FindPathByDisplayId(NvU32 id) const
		{
			return std::ranges::find_if(m_DisplayConfigs,
			                            [id](const DisplayConfigPathInfo& pathInfoEntry)
			                            {
				                            for (const auto & TargetInfo : pathInfoEntry.TargetInfos)
				                            {
					                            if (TargetInfo.displayId == id)
					                            {
						                            return true;
					                            }
				                            }
				                            return false;
			                            }
			);
		}

		void RefreshInternal();
	};
}