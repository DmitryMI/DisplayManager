#include "DisplayManager/NvApi/Init.h"
#include <windows.h>
#include "DisplayManager/NvApi/targetver.h"
#include "nvapi.h"
#include <stdexcept>

namespace DisplayManager::NvApi
{
	Init Init::Instance;
	Init::Init()
	{
		NvAPI_Status status = NvAPI_Initialize();
		if (status != NVAPI_OK)
		{
			throw std::runtime_error("Failed to initialize NVAPI");
		}
	}

	Init::~Init()
	{
		NvAPI_Unload();
	}
}