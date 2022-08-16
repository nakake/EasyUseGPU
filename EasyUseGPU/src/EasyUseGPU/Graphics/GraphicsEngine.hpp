#pragma once

#include <EasyUseGPU/Graphics/Direct3D12/Device/D3D12Device.hpp>
#include <EasyUseGPU/Graphics/Direct3D12/CmdQueue/D3D12CmdQueue.hpp>
#include <EasyUseGPU/Graphics/Direct3D12/CmdList/D3D12CmdList.hpp>
#include <EasyUseGPU/Graphics/Direct3D12/RenderTarget/D3D12RenderTargetView.hpp>
#include <EasyUseGPU/Graphics/Direct3D12/Fence/D3D12Fence.hpp>

#include <EasyUseGPU/Graphics/DXGI/Factory/DXGIFactorys.hpp>
#include <EasyUseGPU/Graphics/DXGI/SwapChain/DXGISwapChain.hpp>

#include <memory>

namespace eug
{
	class EUGD3D12Device;
	class EUGD3D12CmdQueue;
	class EUGDXGIFactory;
	class EUGDXGISwapChain;
	class EUGD3D12CmdList;
	class EUGD3D12RenderTargetView;
	class EUGD3D12Fence;

	class EUGGraphicsEngine
	{
	public:

		EUGGraphicsEngine(uint32_t WndWidth, uint32_t WndHeight, HWND Window)
			:m_WndWidth(WndWidth), m_WndHeight(WndHeight), m_Window(Window)
		{
			m_FrameCount = 2;
			m_FrameIndex = 0;
		}
		
		~EUGGraphicsEngine(){}

		bool Init()
		{
			return InitObjects();
		}

	private:
		uint32_t m_FrameCount;
		uint32_t m_FrameIndex;

		uint32_t m_WndWidth;
		uint32_t m_WndHeight;
		HWND m_Window = nullptr;

		std::unique_ptr<EUGD3D12Device> m_pDevice;
		std::unique_ptr<EUGD3D12CmdQueue> m_pCmdQueue;
		std::unique_ptr<EUGDXGIFactory> m_pFactory;
		std::unique_ptr<EUGDXGISwapChain> m_pSwapChain;
		std::unique_ptr<EUGD3D12CmdList> m_pCmdList;
		std::unique_ptr<EUGD3D12RenderTargetView> m_pRTV;
		std::unique_ptr<EUGD3D12Fence> m_pFence;
		

		bool InitObjects();
	};

}