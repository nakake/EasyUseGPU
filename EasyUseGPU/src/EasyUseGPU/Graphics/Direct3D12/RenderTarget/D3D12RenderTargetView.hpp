#pragma once

#include <EasyUseGPU/Direct3D12/Direct3D12.hpp>
#include <EasyUseGPU/Windows/Windows.hpp>

#include <EasyUseGPU/Graphics/Direct3D12/Descriptor/D3D12DescriptorHeap.hpp>
#include <memory>
#include <vector>

namespace eug
{
	class EUGD3D12RenderTargetView
	{
	public:
		EUGD3D12RenderTargetView(IDXGISwapChain3* pSwapChain, ID3D12Device* pDevice, uint32_t FrameCount)
			:m_pSwapChain(pSwapChain), m_pDevice(pDevice), m_FrameCount(FrameCount)
		{
			m_pRenderTarget.resize(FrameCount);
		}

		~EUGD3D12RenderTargetView(){}

		bool Init() {
			return CreateRTV();
		}

	private:
		IDXGISwapChain3* m_pSwapChain = nullptr;
		ID3D12Device* m_pDevice = nullptr;
		uint32_t m_FrameCount = 0;

		std::unique_ptr<EUGD3D12DescriptorHeap> m_pHeapRTV = nullptr;
		std::vector<ComPtr<ID3D12Resource>> m_pRenderTarget;

		bool CreateRTV() 
		{
			D3D12_RENDER_TARGET_VIEW_DESC desc = {};
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = 0;
			desc.Texture2D.PlaneSlice = 0;

			m_pHeapRTV.reset(new EUGD3D12DescriptorHeap(m_pDevice, m_FrameCount));
			m_pHeapRTV->Init(EUG_DESCRIPTOR_RTV);

			for (auto i = 0u; i < m_FrameCount; ++i) 
			{
				HRESULT hr = m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(m_pRenderTarget[i].GetAddressOf()));
				if (FAILED(hr)) return false;

				m_pDevice->CreateRenderTargetView(m_pRenderTarget[i].Get(), &desc, m_pHeapRTV->GetCPUHandle());

				m_pHeapRTV->AddCPUHandlePtr();
			}

			return true;
		}
	};
}