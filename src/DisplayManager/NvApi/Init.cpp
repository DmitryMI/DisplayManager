#include "DisplayManager/NvApi/Init.h"
#include <stdexcept>
#include <string>

namespace DisplayManager::NvApi
{
	Init Init::Instance;
	Init::Init()
	{
		if (const NvAPI_Status status = NvAPI_Initialize(); status != NVAPI_OK)
		{
			throw std::runtime_error("Failed to initialize NVAPI: " + std::to_string(status));
		}
	}

	Init::~Init()
	{
		NvAPI_Unload();
	}
}
