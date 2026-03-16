#pragma once

#include <windows.h>
#include "DisplayManager/NvApi/targetver.h"
#include "nvapi.h"

namespace DisplayManager::NvApi
{
	class Init
	{
	public:
		~Init();
		static const Init& GetInstance() { return Instance; }
		// ReSharper disable once CppMemberFunctionMayBeStatic
		[[nodiscard]] bool IsInitialized() const { return true; }

	private:
		static Init Instance;
		

		Init();
	};
}