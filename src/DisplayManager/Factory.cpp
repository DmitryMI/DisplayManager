#include "DisplayManager/Factory.h"
#include "DisplayManager/NvApi/DisplayProvider.h"

namespace DisplayManager
{
	IDisplayProvider* Factory::GetDisplayProvider()
	{
		return new NvApi::DisplayProvider();
	}
}