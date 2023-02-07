#include "EasyUseGPUEngine.hpp"

namespace eug 
{
	EasyUseGPUEngine::EasyUseGPUEngine()
	{


#if defined(DEBUG) || defined(_DEBUG)
		{
			ComPtr<ID3D12Debug> debug;

			HRESULT hr = D3D12GetDebugInterface(IID_PPV_ARGS(debug.GetAddressOf()));

			if (SUCCEEDED(hr)) debug->EnableDebugLayer();
		}
#endif
		m_Window.reset(new Window);
		m_Window->init();
		m_pDevice.reset(new EUGD3D12Device());
		m_pDevice->Init();

		ComPtr<ID3D12DebugDevice> debugInterface = nullptr;
		auto hr = m_pDevice->GetDevice()->QueryInterface(debugInterface.GetAddressOf());
		if (SUCCEEDED(hr))
		{
			debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
		}

		m_pParaEngine.reset(new EUGParallelEngine(m_pDevice->GetDevice()));
		m_pParaEngine->Init();
	}

	EasyUseGPUEngine::~EasyUseGPUEngine()
	{
	}

}

