#include "D3D12RenderTargetView.hpp"

namespace eug
{
	D3D12_RESOURCE_BARRIER* EUGD3D12RenderTargetView::GetResouceBarrier(
		BARRIER_STETAS state, const uint32_t FrameIndex)
	{

		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = m_pRenderTarget[FrameIndex].Get();
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		switch (state)
		{
		case BARRIER_BEFORE:

			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
			
			break;

		case BARRIER_AFTER:

			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

			break;
		}

		return &barrier;
	}
}