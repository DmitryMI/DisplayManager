#pragma once

#include "DisplayManager/IDisplayProvider.h"

namespace DisplayManager
{
	class Factory
	{
	public:
		static IDisplayProvider* GetDisplayProvider();
	};
}