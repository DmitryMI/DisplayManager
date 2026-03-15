#include "DisplayManager/NvApi/DisplayProvider.h"
#include "DisplayManager/NvApi/Init.h"
#include <stdexcept>

namespace DisplayManager::NvApi
{
	DisplayProvider::DisplayProvider()
	{
		if (!DisplayManager::NvApi::Init::GetInstance().IsInitialized())
		{
			throw std::runtime_error("NVAPI is not initialized");
		}
		

	}

	// https://github.com/NVIDIA/nvapi/blob/main/Sample_Code/DisplayConfiguration/DisplayConfiguration.cpp
	std::vector<std::shared_ptr<IDisplay>> DisplayProvider::GetDisplays() const
	{
		return {};
	}
}