
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
        return DisplayProvider::GetDisplayName(m_DisplayId);
    }

    bool Display::IsPrimary() const
    {
        return false;
    }

    bool Display::IsEnabled() const
    {
        return m_DisplayProvider->IsDisplayEnabled(m_DisplayId);
    }

    void Display::SetEnabled(bool enabled)
    {
        m_DisplayProvider->SetDisplayEnabled(m_DisplayId, enabled);
    }

    std::optional<std::tuple<int, int>> Display::GetCoordinates() const
    {
        return m_DisplayProvider->GetDisplayCoordinates(m_DisplayId);
    }

    void Display::SetCoordinates(int x, int y)
    {
    }
}
