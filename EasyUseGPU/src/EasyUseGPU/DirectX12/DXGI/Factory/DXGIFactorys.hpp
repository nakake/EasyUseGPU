#pragma once

#include <EasyUseGPU/Direct3D12/Direct3D12.hpp>
#include <EasyUseGPU/Windows/Windows.hpp>

namespace eug
{
	class EUGDXGIFactory
	{
	public:
		EUGDXGIFactory(){}
		~EUGDXGIFactory(){}

		bool Init()
		{
			if (!Createfactory4()) return false;

			return true;
		}

		IDXGIFactory4* GetFactory4()
		{
			return m_pFactory4.Get();
		}

	private:
		ComPtr<IDXGIFactory4> m_pFactory4 = nullptr;

		bool Createfactory4()
		{
#ifdef _DEBUG
			HRESULT hr = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(m_pFactory4.GetAddressOf()));
#else
			HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(m_pFactory4.GetAddressOf()));
#endif
			return SUCCEEDED(hr);
		}
	};

}