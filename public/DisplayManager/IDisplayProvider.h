#pragma once

#include "DisplayManager/IDisplay.h"
#include <memory>
#include <vector>

namespace DisplayManager
{
	class IConfiguration;
}

namespace DisplayManager
{
	namespace Serialization
	{
		class IInputArchive;
		class IOutputArchive;
	}

	class IDisplayProvider
	{
	public:
		virtual ~IDisplayProvider() = default;
		virtual void Refresh() = 0;
		virtual std::vector<std::shared_ptr<IDisplay>> GetDisplays() = 0;
		virtual std::unique_ptr<IConfiguration> DeserializeConfiguration(Serialization::IInputArchive& archive) const = 0;
		virtual const IConfiguration& GetActiveConfiguration() const = 0;
		virtual bool ApplyConfiguration(const IConfiguration& configuration) = 0;
	};
}
