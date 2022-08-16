#pragma once

#include <EasyUseGPU/Direct3D12/Direct3D12.hpp>
#include <EasyUseGPU/Windows/Windows.hpp>
#include <memory>
#include <vector>

namespace eug
{
	enum DESCRIPTOR_NUMS
	{
		EUG_DESCRIPTOR_RTV,
		EUG_DESCRIPTOR_DSV,		
		EUG_DESCRIPTOR_SAMPLER,	
		EUG_DESCRIPTOR_CSU		//CBV_SRV_UAV,
	};

	class DescriptorHandle
	{
	public:
		D3D12_CPU_DESCRIPTOR_HANDLE HandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE HandleGPU;

		bool HasCPU() const
		{ return HandleCPU.ptr != 0; }

		bool HasGPU() const
		{ return HandleGPU.ptr != 0; }
	};

	class EUGD3D12DescriptorHeap
	{
	public:
		EUGD3D12DescriptorHeap(ID3D12Device* pDevice, uint32_t FrameCount)
			:m_pDevice(pDevice), m_FrameCount(FrameCount)
		{
			m_IncrementSize = 0;
			m_pHandle.reset(new DescriptorHandle());
		}

		~EUGD3D12DescriptorHeap(){}

		bool Init(int DiscriptorNum);

		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle()
		{
			return m_pHandle->HandleCPU;
		}

		void AddCPUHandlePtr()
		{
			m_pHandle->HandleCPU.ptr += m_IncrementSize;
		}

	private:
		uint32_t m_IncrementSize;
		ComPtr<ID3D12DescriptorHeap> m_pHeap;
		std::unique_ptr<DescriptorHandle> m_pHandle;

		ID3D12Device* m_pDevice = nullptr;
		const uint32_t m_FrameCount;
		
		bool Create(const D3D12_DESCRIPTOR_HEAP_DESC* pDesc);
	};

}