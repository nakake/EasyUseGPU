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

		bool EUGSetParaState(const EUG_PARALLEL_STATE* pParaState)
		{
			return m_pParaEngine->ParaSetParaState(pParaState);
		}

		bool EUGExecutionGPU(uint32_t x, uint32_t y, uint32_t z)
		{
			if (x <= 0 || y <= 0 || z <= 0) return false;
			return m_pParaEngine->ParaExecutionGPU(x, y ,z);
		}

	public:

		template<typename T>
		bool EUGSetUploadBufferData(const vector<T>& pUploadData)
		{
			return m_pParaEngine->ParaSetUploadBufferData(pUploadData);
		}

		template<typename T>
		bool EUGSetOutputBufferData(const vector<T>& pOutputData)
		{
			return m_pParaEngine->ParaSetOutputBufferData(pOutputData);
		}

		template<typename T>
		bool EUGGetBufferDataFromGPU(vector<T>& pOutputData, uint32_t DataIndex)
		{
			return m_pParaEngine->ParaGetBufferDataFromGPU(pOutputData, DataIndex);
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

