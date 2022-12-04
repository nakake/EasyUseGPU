#include "EasyUseGPUEngine.hpp"

namespace eug 
{
	EasyUseGPUEngine::EasyUseGPUEngine()
	{
		m_Window.reset(new Window);
		m_Window->init();
		m_pDevice.reset(new EUGD3D12Device());
		m_pDevice->Init();

		//ID3D12DebugDevice* debugInterface;
		//auto hr = m_pDevice->GetDevice()->QueryInterface(&debugInterface);
		//if (SUCCEEDED(hr))
		//{
		//	debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
		//	debugInterface->Release();
		//}

		m_pParaEngine.reset(new EUGParallelEngine(m_pDevice->GetDevice()));
		m_pParaEngine->Init();
	}

	EasyUseGPUEngine::~EasyUseGPUEngine()
	{
	}

}

