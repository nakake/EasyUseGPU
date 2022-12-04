#include "D3D12CmdList.hpp"

namespace eug
{
	void EUGD3D12CmdList::SetBarrier(EUGD3D12RenderTargetView* pRendderTarget, const uint32_t FrameIndex)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE Handle = pRendderTarget->GetHandle();

		m_pCmdAllocator[FrameIndex]->Reset();
		m_pCmdList->Reset(m_pCmdAllocator[FrameIndex].Get(), nullptr);

		m_pCmdList->ResourceBarrier(1, pRendderTarget->GetResouceBarrier(BARRIER_BEFORE, FrameIndex));

		m_pCmdList->OMSetRenderTargets(1, &Handle, FALSE, nullptr);

		m_pCmdList->ClearRenderTargetView(Handle, m_ClearColor, 0, nullptr);
 	}

	void EUGD3D12CmdList::ReleaseBarrier(EUGD3D12RenderTargetView* pRendderTarget, const uint32_t FrameIndex)
	{
		m_pCmdList->ResourceBarrier(1, pRendderTarget->GetResouceBarrier(BARRIER_AFTER, FrameIndex));

		CloseCmdList();
	}
}