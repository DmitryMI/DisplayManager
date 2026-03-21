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

        [[nodiscard]] std::string GetName() const override;
        [[nodiscard]] bool IsPrimary() const override;
        [[nodiscard]] bool IsEnabled() const override;
        void SetEnabled(bool enabled) override;
        [[nodiscard]] std::optional<std::tuple<int, int>> GetCoordinates() const override;
        bool SetCoordinates(int x, int y) override;

    private:
        DisplayProvider* m_DisplayProvider;
        NvPhysicalGpuHandle m_GpuHandle;
        NvU32 m_DisplayId;

        friend class DisplayProvider;
    };
}