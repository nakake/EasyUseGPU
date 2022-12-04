#pragma once

#include <EasyUseGPU/Direct3D12/Direct3D12.hpp>
#include <EasyUseGPU/Windows/Windows.hpp>
#include <memory>
#include <vector>

namespace eug
{
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
		EUGD3D12DescriptorHeap(ID3D12Device* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE DescriptorType, int32_t HandleSize)
			:m_pDevice(pDevice), m_DescriptorType(DescriptorType), m_HandleSize(HandleSize)
		{
			m_IncrementSize = 0;
			m_pHandles.reset(new DescriptorHandle());
		}

		~EUGD3D12DescriptorHeap(){}

		bool Init();

		void GetCPUHandle(D3D12_CPU_DESCRIPTOR_HANDLE* handle)
		{
			*handle = m_pHeap->GetCPUDescriptorHandleForHeapStart();
			if(refeCount != 0) handle->ptr += m_IncrementSize;
		}

		ID3D12DescriptorHeap* GetDescriptorHeap()
		{
			return m_pHeap.Get();
		}

	private:
		uint32_t m_IncrementSize;
		ComPtr<ID3D12DescriptorHeap> m_pHeap;
		std::unique_ptr<DescriptorHandle> m_pHandles;
		const D3D12_DESCRIPTOR_HEAP_TYPE m_DescriptorType;

		ID3D12Device* m_pDevice = nullptr;
		const int32_t m_HandleSize;
		const int32_t refeCount = 0;
		
		bool CreateHeap(const D3D12_DESCRIPTOR_HEAP_DESC* pDesc);
	};

}