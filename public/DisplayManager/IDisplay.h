#pragma once

#include <string_view>
#include <utility>

namespace DisplayManager
{
	class IDisplay
	{
	public:
		virtual ~IDisplay() = default;
		virtual std::string_view GetName() const = 0;
		virtual bool IsEnabled() const = 0;
		virtual void SetEnabled(bool enabled) = 0;
		virtual std::tuple<int, int> GetCoordinates() const = 0;
		virtual void SetCoordinates(int x, int y) = 0;
	};
}