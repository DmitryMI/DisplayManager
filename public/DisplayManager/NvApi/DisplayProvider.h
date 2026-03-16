#pragma once

#include "DisplayManager/IDisplayProvider.h"
#include "DisplayManager/NvApi/Init.h"
#include "DisplayManager/NvApi/DisplayConfigPathInfo.h"
#include <unordered_map>

#include "Configuration.h"

namespace DisplayManager::NvApi
{
	class Display;

	class DisplayProvider : public IDisplayProvider
	{
	public:
		DisplayProvider();
		~DisplayProvider() override = default;

		std::vector<std::shared_ptr<IDisplay>> GetDisplays() override;
		void Refresh() override;

		static std::string GetDisplayName(NvU32 id);
		[[nodiscard]] std::optional<std::tuple<int, int>> GetDisplayCoordinates(NvU32 id) const;
		bool SetDisplayCoordinates(NvU32 id, int x, int y);
		[[nodiscard]] bool IsDisplayEnabled(NvU32 id) const;
		void SetDisplayEnabled(NvU32 id, bool enabledSet);
		[[nodiscard]] bool IsDisplayPrimary(NvU32 id) const;

	private:
		std::unordered_map<NvU32, std::shared_ptr<Display>> m_Displays;
		std::vector<NV_GPU_DISPLAYIDS> m_DisplayIds;
		Configuration m_Configuration;

		[[nodiscard]] auto FindPathByDisplayId(NvU32 id) const
		{
			return std::ranges::find_if(m_Configuration.GetPathInfos(),
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

		[[nodiscard]] std::tuple<int, int> FindTargetInfoIndexByDisplayId(NvU32 id) const
		{
			for (size_t pathInfoIndex = 0; pathInfoIndex < m_Configuration.GetPathInfos().size(); ++pathInfoIndex)
			{
				for (size_t targetInfoIndex = 0; targetInfoIndex < m_Configuration.GetPathInfos()[pathInfoIndex].TargetInfos.size(); targetInfoIndex++)
				{
					if (m_Configuration.GetPathInfos()[pathInfoIndex].TargetInfos[targetInfoIndex].displayId == id)
					{
						return std::make_tuple(static_cast<int>(pathInfoIndex), static_cast<int>(targetInfoIndex));
					}
				}
			}

			return std::make_tuple(-1, -1);
		}

		bool ApplyCurrentConfiguration();
		void RefreshInternal();

	public:
		std::unique_ptr<IConfiguration> DeserializeConfiguration(
			Serialization::IInputArchive& archive) const override;
		[[nodiscard]] const IConfiguration& GetActiveConfiguration() const override;
		bool ApplyConfiguration(const IConfiguration& configuration) override;
	};
}
