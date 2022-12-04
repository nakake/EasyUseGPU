#include "DebugInterface.hpp"

namespace eug
{
	namespace Debug
	{
		bool EUGDebugInterface::Init()
		{
			HRESULT hr = D3D12GetDebugInterface(IID_PPV_ARGS(Debug.GetAddressOf()));
			if (FAILED(hr))return false;

			Debug->EnableDebugLayer();

			hr = m_pDevice->QueryInterface(IID_PPV_ARGS(&DebugInterface));
			if (FAILED(hr))return false;

			DebugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
			DebugInterface->Release();

			return true;
		}
	}
}