#pragma once

#include <EasyUseGPU/Direct3D12/Direct3D12.hpp>
#include <EasyUseGPU/Windows/Windows.hpp>

namespace eug 
{
	class EUGD3D12Device
	{
	public:

		EUGD3D12Device() 
		{
			level = D3D_FEATURE_LEVEL_11_0;
		}

		~EUGD3D12Device(){}

		ComPtr<ID3D12Device> GetDevice() const
		{
			return m_pDevice;
		}

		bool Init() 
		{
			return CreateD3D12Device();
		}


	private:

		ComPtr<ID3D12Device> m_pDevice = nullptr;
		D3D_FEATURE_LEVEL level;

		bool CreateD3D12Device() 
		{
			D3D_FEATURE_LEVEL levels[] = 
			{
				D3D_FEATURE_LEVEL_12_1,
				D3D_FEATURE_LEVEL_12_0,
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0
			};
			
			/*
			 * 全てのレベルを試し、成功したときに終了する。
			 * 使用するコンピュータのスペックによって、最適なデバイスを生成するため全てのレベルを試す。
			 * 
			*/
			for (D3D_FEATURE_LEVEL& i : levels)
			{
				 auto hr = D3D12CreateDevice(nullptr, i, IID_PPV_ARGS(m_pDevice.GetAddressOf()));
				 if (SUCCEEDED(hr))
				 {
					 level = i;
					 return true;
				 }
			}

			m_pDevice->SetName(L"EUG_MAIN_DEVICE");

			return false;
		}
	};
}


