#pragma once
#include <memory>
#include <EasyUseGPU/Window/Window.hpp>
#include <EasyUseGPU/Graphics/GraphicsEngine.hpp>



namespace eug
{ 
	using namespace std;
	
	enum ENGINE_MEMBERS{
			EUG_ENGINE_WINDOW,
	};

	class EUGGraphicsEngine;

	class EasyUseGPUEngine
	{
	public:
		
		EasyUseGPUEngine();
		~EasyUseGPUEngine();
		
		[[nodiscard]]
		Window* GetWindow() const
		{
			return m_Window.get();
		}

		bool Init()
		{
			return GraphicsInit();
		}

	private:

		unique_ptr<Window> m_Window = nullptr;
		unique_ptr<EUGGraphicsEngine> m_pGraphicsEngine = nullptr;
		
		bool GraphicsInit()
		{
			m_pGraphicsEngine.reset(new EUGGraphicsEngine(
				m_Window->GetWidth(), m_Window->GetHeight(), m_Window->GetWindow()));
			return m_pGraphicsEngine->Init();
		}

	};

	inline extern unique_ptr<EasyUseGPUEngine> engine = nullptr;
}

