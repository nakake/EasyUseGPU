#include "D3D12CmdList.hpp"

namespace eug
{
	bool EUGD3D12CmdList::Init(const uint32_t FrameIndex)
	{
		if (!CreateD3D12CmdAllocator()) return false;
		return CreateD3D12CmdList(FrameIndex);
	}

	bool EUGD3D12CmdList::CreateD3D12CmdAllocator()
	{
		for (auto i = 0u; i < m_FrameCount; ++i) {
			HRESULT hr = m_pDevice->CreateCommandAllocator(
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				IID_PPV_ARGS(m_pCmdAllocator[i].GetAddressOf()));
			if (FAILED(hr))return false;
		}

		return true;
	}

	bool EUGD3D12CmdList::CreateD3D12CmdList(const uint32_t FrameIndex)
	{
		HRESULT hr = m_pDevice->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			m_pCmdAllocator[FrameIndex].Get(),
			nullptr,
			IID_PPV_ARGS(m_pCmdList.GetAddressOf()));

		return SUCCEEDED(hr);
	}
}