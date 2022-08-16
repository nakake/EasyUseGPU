#pragma once

#include <EasyUseGPU/Windows/Windows.hpp>
#include <EasyUseGPU/Direct3D12/Direct3D12.hpp>

namespace eug
{

	class EUGDXGISwapChain
	{
	public:

		EUGDXGISwapChain(ID3D12Device* m_pDevice,IDXGIFactory4* m_pFactory, ID3D12CommandQueue* m_pCmdQueue)
			:m_pDevice(m_pDevice), 
			m_pFactory(m_pFactory), 
			m_pCmdQueue(m_pCmdQueue) 
		{}

		~EUGDXGISwapChain(){}

		bool Init(
			uint32_t height, uint32_t width, uint32_t FrameCount, 
			const HWND hWnd ,uint32_t refreshRate = 60
		)
		{
			DXGI_SWAP_CHAIN_DESC desc = { };

			desc.BufferDesc.Width = width;
			desc.BufferDesc.Height = height;
			desc.BufferDesc.RefreshRate.Numerator = refreshRate;
			desc.BufferDesc.RefreshRate.Denominator = 1;
			desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			desc.BufferCount = FrameCount;
			desc.OutputWindow = hWnd;
			desc.Windowed = TRUE;
			desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

			return CreateD3dD12SwapChain(desc);
		}

		IDXGISwapChain3* GetSwapChain()
		{
			return m_pSwapChain3.Get();
		}

		uint32_t GetFrameIndex()
		{
			return m_pSwapChain3->GetCurrentBackBufferIndex();
		}

	private:

		ComPtr<IDXGISwapChain3> m_pSwapChain3 = nullptr;
		ID3D12Device*			m_pDevice = nullptr;
		ID3D12CommandQueue*		m_pCmdQueue = nullptr;
		IDXGIFactory4*			m_pFactory = nullptr;

		bool CreateD3dD12SwapChain(DXGI_SWAP_CHAIN_DESC desc)
		{

			ComPtr<IDXGISwapChain> pSwapChain = nullptr;

			HRESULT hr = m_pFactory->CreateSwapChain(m_pCmdQueue, &desc, pSwapChain.GetAddressOf());
			if (FAILED(hr)) return false;

			hr = pSwapChain.As(&m_pSwapChain3);
			if (FAILED(hr)) return false;

			return true;

		}

	};
}