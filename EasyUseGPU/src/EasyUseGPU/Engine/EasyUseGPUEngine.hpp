#pragma once
#include <memory>
#include <EasyUseGPU/Window/CWindow.hpp>

namespace eug
{ 
	using namespace std;
	
	enum ENGINE_MEMBERS{
			EUG_ENGINE_WINDOW,
	};

	class EasyUseGPUEngine
	{
	public:
		
		EasyUseGPUEngine();
		~EasyUseGPUEngine();
		
		[[nodiscard]]
		CWindow* GetWindow() const
		{
			return m_Window.get();
		}

	private:

		unique_ptr<CWindow> m_Window = nullptr;
		
	};

	inline extern unique_ptr<EasyUseGPUEngine> engine = nullptr;
}

