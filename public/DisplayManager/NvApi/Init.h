#pragma once

namespace DisplayManager::NvApi
{
	class Init
	{
	public:
		~Init();
		static const Init& GetInstance() { return Instance; }
		bool IsInitialized() const { return true; }

	private:
		static Init Instance;
		

		Init();
	};
}