#pragma once

#include "DisplayManager/NvApi/Init.h"
#include "DisplayManager/IDisplay.h"

namespace DisplayManager::NvApi
{
    class DisplayProvider;

    class Display : public IDisplay
    {
    public:
        Display(DisplayProvider* provider, NvPhysicalGpuHandle gpu, NvU32 displayId);

        std::string GetName() const override;
        bool IsPrimary() const override;
        bool IsEnabled() const override;
        void SetEnabled(bool enabled) override;
        std::optional<std::tuple<int, int>> GetCoordinates() const override;
        void SetCoordinates(int x, int y) override;

    private:
        DisplayProvider* m_DisplayProvider;
        NvPhysicalGpuHandle m_GpuHandle;
        NvU32 m_DisplayId;

        friend class DisplayProvider;
    };
}