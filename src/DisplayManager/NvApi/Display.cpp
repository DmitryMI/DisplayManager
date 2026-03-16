
#include "DisplayManager/NvApi/Display.h"

#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "DisplayManager/NvApi/DisplayProvider.h"

namespace
{

}

namespace DisplayManager::NvApi
{
    Display::Display(DisplayProvider* provider, const NvPhysicalGpuHandle gpu, const NvU32 displayId) :
        m_DisplayProvider(provider),
        m_GpuHandle(gpu),
        m_DisplayId(displayId)
    {
    }

    std::string Display::GetName() const
    {
        return DisplayProvider::GetDisplayName(m_GpuHandle, m_DisplayId);
    }

    bool Display::IsEnabled() const
    {
        return false;
    }

    void Display::SetEnabled(bool enabled)
    {
    }

    std::tuple<int, int> Display::GetCoordinates() const
    {
        return std::make_tuple(0, 0);
    }

    void Display::SetCoordinates(int x, int y)
    {
    }
}
