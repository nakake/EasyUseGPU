#pragma once

#include <EasyUseGPU/Windows/Windows.hpp>
#include <EasyUseGPU/Direct3D12/Direct3D12.hpp>
#include <vector>
#include <memory>

#include <EasyUseGPU/DirectX12/Direct3D12/RenderTarget/D3D12RenderTargetView.hpp>

namespace eug
{
	class EUGD3D12CmdList
	{
	public:

		EUGD3D12CmdList(ID3D12Device* pDevice, uint32_t FrameCount)
			:m_pDevice(pDevice), m_FrameCount(FrameCount)
		{
			m_pCmdAllocator.resize(m_FrameCount);
		}
		
		~EUGD3D12CmdList(){}

		ID3D12GraphicsCommandList* GetCmdList() 
		{
			return m_pCmdList.Get();
		}

		ID3D12CommandAllocator* GetAllo(uint32_t index)
		{
			return m_pCmdAllocator[index].Get();
		}

		void CloseCmdList()
		{
			m_pCmdList->Close();
		}

		bool Init(const uint32_t FrameIndex);

		void SetBarrier(EUGD3D12RenderTargetView* pRendderTarget, const uint32_t FrameIndex);
		void ReleaseBarrier(EUGD3D12RenderTargetView* pRendderTarget, const uint32_t FrameIndex);

	private:

		const uint32_t m_FrameCount;

		std::vector<ComPtr<ID3D12CommandAllocator>> m_pCmdAllocator;
		ComPtr<ID3D12GraphicsCommandList> m_pCmdList;
		ID3D12Device* m_pDevice;

		float m_ClearColor[4] = { 0.25f, 0.25f, 0.25f, 1.0f };

		bool CreateD3D12CmdAllocator();
		bool CreateD3D12CmdList(const uint32_t FrameIndex);
	};
}