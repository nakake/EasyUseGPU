#pragma once

#include "GraphicsEngine.hpp"

namespace eug
{
	bool EUGGraphicsEngine::InitObjects()
	{
		m_pDevice.reset(new EUGD3D12Device());
		m_pDevice->Init();
		
		m_pCmdQueue.reset(new EUGD3D12CmdQueue(m_pDevice->GetDevice()));
		m_pCmdQueue->Init();
		
		m_pFactory.reset(new EUGDXGIFactory());
		m_pFactory->Init();
		
		m_pSwapChain.reset(new EUGDXGISwapChain(
			m_pDevice->GetDevice(), m_pFactory->GetFactory4(), m_pCmdQueue->GetCmdQueue()));
		m_pSwapChain->Init(
			m_WndHeight, m_WndWidth,m_FrameCount, m_Window);
	
		m_FrameIndex = m_pSwapChain->GetFrameIndex();
		
		m_pCmdList.reset(new EUGD3D12CmdList(m_pDevice->GetDevice(), m_FrameCount, m_FrameIndex));
		m_pCmdList->Init();
		
		m_pRTV.reset(new EUGD3D12RenderTargetView(
			m_pSwapChain->GetSwapChain(), m_pDevice->GetDevice(), m_FrameCount));
		m_pRTV->Init();
		
		m_pFence.reset(new EUGD3D12Fence(m_pDevice->GetDevice(), m_FrameCount, m_FrameIndex));
		m_pFence->Init();

		m_pCmdList->CloseCmdList();

		return true;
	}
}