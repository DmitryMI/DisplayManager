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
		virtual std::vector<std::shared_ptr<IDisplay>> GetDisplays() const = 0;
	};
}