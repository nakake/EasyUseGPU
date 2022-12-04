#pragma once
#include <memory>
#include <EasyUseGPU/Window/Window.hpp>
#include <EasyUseGPU/Graphics/GraphicsEngine.hpp>
#include <EasyUseGPU/Parallel/Engine/ParallelEngine.hpp>
#include <EasyUseGPU/DirectX12/DebugInterface/DebugInterface.hpp>

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

		void testrender() {
			//m_pGraphicsEngine->Render();
		}
	
		void StartRender()
		{
			m_pGraphicsEngine->StartRender();
			m_EndRenderFlag = true;
		}

		void EndRender()
		{
			if (m_EndRenderFlag) {
				m_pGraphicsEngine->EndRender();
			}
		}

		void ComputeShader()
		{
			m_pParaEngine->Run();
		}

		void mulMatrix(vector<int>& data1, vector<int>& data2)
		{
			//あとで計算結果の行列の大きさを計算する仕組みを作る
			//行列計算ができるかも確認
			m_pParaEngine->MulMatrix(data1, data2, 4);
		}

	private:

		unique_ptr<Window> m_Window = nullptr;
		unique_ptr<EUGGraphicsEngine> m_pGraphicsEngine = nullptr;
		unique_ptr<EUGD3D12Device> m_pDevice = nullptr;
		unique_ptr<EUGParallelEngine> m_pParaEngine = nullptr;
		unique_ptr<Debug::EUGDebugInterface> m_pDebug = nullptr;

		bool m_EndRenderFlag = false;
		
		bool GraphicsInit()
		{
			m_pGraphicsEngine.reset(new EUGGraphicsEngine(
				m_Window->GetWidth(), m_Window->GetHeight(), m_Window->GetWindow(), m_pDevice->GetDevice()));
			return m_pGraphicsEngine->Init();
		}

	};

	inline extern unique_ptr<EasyUseGPUEngine> engine = nullptr;
}

