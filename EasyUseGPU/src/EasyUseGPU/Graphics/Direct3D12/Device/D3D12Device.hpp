#pragma once

#include <EasyUseGPU/Direct3D12/Direct3D12.hpp>
#include <EasyUseGPU/Windows/Windows.hpp>

namespace eug 
{
	class EUGD3D12Device
	{
	public:

		EUGD3D12Device(){}
		~EUGD3D12Device(){}

		ID3D12Device* GetDevice() const
		{
			return m_pDevice.Get();
		}

		bool Init() 
		{
			return CreateD3D12Device();
		}


	private:
		
		ComPtr<ID3D12Device> m_pDevice = nullptr;

		bool CreateD3D12Device() 
		{

			HRESULT hr = D3D12CreateDevice
			(
				nullptr, 
				D3D_FEATURE_LEVEL_11_0, 
				IID_PPV_ARGS(m_pDevice.GetAddressOf())
			);

			return SUCCEEDED(hr);
		}
	};
}


