#pragma once

#include "DisplayManager/IDisplay.h"
#include <memory>
#include <vector>

namespace DisplayManager
{
	class IDisplayProvider
	{
	public:
		virtual ~IDisplayProvider() = default;
		virtual void Refresh() = 0;
		virtual std::vector<std::shared_ptr<IDisplay>> GetDisplays() = 0;
	};
}