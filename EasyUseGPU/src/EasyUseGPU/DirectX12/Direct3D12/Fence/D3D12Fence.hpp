#pragma once

#include <EasyUseGPU/Direct3D12/Direct3D12.hpp>
#include <EasyUseGPU/Windows/Windows.hpp>

#include <vector>

namespace eug
{
	class EUGD3D12Fence
	{
	public:
		EUGD3D12Fence(ID3D12Device* pDevice, uint32_t FrameCount)
			:m_pDevice(pDevice), m_FrameCount(FrameCount)
		{
			m_FenceCounter.resize(FrameCount);
		}

		~EUGD3D12Fence() 
		{
			if (m_FenceEvent != nullptr)
			{
				CloseHandle(m_FenceEvent);
				m_FenceEvent = nullptr;
			}
		}

		bool Init(const uint32_t FrameIndex)
		{
			return Createfence(FrameIndex);
		}

		ID3D12Fence* GetFence()
		{
			return m_pFence.Get();
		}

		uint64_t GetFenceCount(uint32_t FrameIndex)
		{
			return m_FenceCounter[FrameIndex];
		}

		void Wait(uint32_t FrameIndex)
		{
			if (m_pFence->GetCompletedValue() < m_FenceCounter[FrameIndex])
			{
				m_pFence->SetEventOnCompletion(m_FenceCounter[FrameIndex], m_FenceEvent);
				WaitForSingleObjectEx(m_FenceEvent, INFINITE, FALSE);
			}

			m_FenceCounter[FrameIndex]++;
		}

		void WaitDiscard(uint32_t FrameIndex)
		{
			m_pFence->SetEventOnCompletion(m_FenceCounter[FrameIndex], m_FenceEvent);
			WaitForSingleObjectEx(m_FenceEvent, INFINITE, FALSE);
			m_FenceCounter[FrameIndex]++;
		}

	private:
		uint32_t m_FrameCount = 0;
		ID3D12Device* m_pDevice = nullptr;
		
		ComPtr<ID3D12Fence> m_pFence = nullptr;
		HANDLE m_FenceEvent = nullptr;
		std::vector<uint64_t> m_FenceCounter;

		bool Createfence(const uint32_t FrameIndex)
		{
			for(auto i = 0u; i < m_FrameCount; ++i)
			{ m_FenceCounter[i] = 0; }

			HRESULT hr = m_pDevice->CreateFence(
				m_FenceCounter[FrameIndex],
				D3D12_FENCE_FLAG_NONE,
				IID_PPV_ARGS(m_pFence.GetAddressOf()));

			if (FAILED(hr)) return false;

			m_FenceCounter[FrameIndex]++;

			m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

			if (m_FenceEvent == nullptr) return false;

			return true;
		}
	};

}