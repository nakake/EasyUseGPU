#pragma once

#include <EasyUseGPU/Windows/Windows.hpp>
#include <EasyUseGPU/Direct3D12/Direct3D12.hpp>
#include <vector>

namespace eug
{
	class EUGD3D12CmdList
	{
	public:

		EUGD3D12CmdList(ID3D12Device* pDevice, uint32_t FrameCount, uint32_t FrameIndex)
			:m_pDevice(pDevice), m_FrameCount(FrameCount), m_FrameIndex(FrameIndex)
		{
			m_pCmdAllocator.resize(m_FrameCount);
		}
		
		~EUGD3D12CmdList(){}

		bool Init()
		{

			if (!CreateD3D12CmdAllocator()) return false;
			return CreateD3D12CmdList();

		}

		ID3D12GraphicsCommandList* GetCmdList() const
		{
			return m_pCmdList.Get();
		}

		void CloseCmdList()
		{
			m_pCmdList->Close();
		}

	private:

		const uint32_t m_FrameCount;
		const uint32_t m_FrameIndex;

		std::vector<ComPtr<ID3D12CommandAllocator>> m_pCmdAllocator;
		ComPtr<ID3D12GraphicsCommandList> m_pCmdList;
		ID3D12Device* m_pDevice;

		bool CreateD3D12CmdAllocator()
		{

			for (auto i = 0u; i < m_FrameCount; ++i) {
				HRESULT hr = m_pDevice->CreateCommandAllocator(
					D3D12_COMMAND_LIST_TYPE_DIRECT,
					IID_PPV_ARGS(m_pCmdAllocator[i].GetAddressOf()));
				if (FAILED(hr))return false;
			}

			return true;
		}

		bool CreateD3D12CmdList()
		{

			HRESULT hr = m_pDevice->CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				m_pCmdAllocator[m_FrameIndex].Get(),
				nullptr,
				IID_PPV_ARGS(m_pCmdList.GetAddressOf()));

			return SUCCEEDED(hr);
		}

	};
}