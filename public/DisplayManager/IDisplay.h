#pragma once

#include <optional>
#include <string>
#include <utility>

namespace DisplayManager
{
	class IDisplay
	{
	public:
		virtual ~IDisplay() = default;
		virtual std::string GetName() const = 0;
		virtual bool IsPrimary() const = 0;
		virtual bool IsEnabled() const = 0;
		virtual void SetEnabled(bool enabled) = 0;
		virtual std::optional<std::tuple<int, int>> GetCoordinates() const = 0;
		virtual void SetCoordinates(int x, int y) = 0;
	};
}