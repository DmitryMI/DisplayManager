#pragma once

#include "DisplayManager/IDisplayProvider.h"
#include "nvapi.h"

namespace DisplayManager::NvApi
{
	class DisplayProvider : public DisplayManager::IDisplayProvider
	{
	public:
		DisplayProvider();
		virtual ~DisplayProvider() = default;

		std::vector<std::shared_ptr<IDisplay>> GetDisplays() const override;
	};
}