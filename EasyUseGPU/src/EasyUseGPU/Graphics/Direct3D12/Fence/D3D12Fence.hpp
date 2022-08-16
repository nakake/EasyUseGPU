#pragma once

#include <EasyUseGPU/Direct3D12/Direct3D12.hpp>
#include <EasyUseGPU/Windows/Windows.hpp>

#include <vector>

namespace eug
{
	class EUGD3D12Fence
	{
	public:
		EUGD3D12Fence(ID3D12Device* pDevice, uint32_t FrameCount, uint32_t FrameIndex)
			:m_pDevice(pDevice), m_FrameCount(FrameCount), m_FrameIndex(FrameIndex)
		{
			m_FenceCouter.resize(FrameCount);
		}

		~EUGD3D12Fence() {}

		bool Init()
		{
			return Createfence();
		}

	private:
		uint32_t m_FrameCount = 0;
		uint32_t m_FrameIndex = 0;
		ID3D12Device* m_pDevice = nullptr;
		
		ComPtr<ID3D12Fence> m_Fence = nullptr;
		HANDLE m_FenceEvent = nullptr;
		std::vector<uint64_t> m_FenceCouter;

		bool Createfence()
		{
			for(auto i = 0u; i < m_FrameCount; ++i)
			{ m_FenceCouter[i] = 0; }

			HRESULT hr = m_pDevice->CreateFence(
				m_FenceCouter[m_FrameIndex],
				D3D12_FENCE_FLAG_NONE,
				IID_PPV_ARGS(m_Fence.GetAddressOf()));

			if (FAILED(hr)) return false;

			m_FenceCouter[m_FrameIndex]++;

			m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

			if (m_FenceEvent == nullptr) return false;

			return true;
		}
	};

}