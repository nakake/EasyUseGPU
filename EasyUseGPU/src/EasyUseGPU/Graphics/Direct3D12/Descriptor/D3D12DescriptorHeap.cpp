#include "D3D12DescriptorHeap.hpp"

namespace eug
{

	bool EUGD3D12DescriptorHeap::Init(int DiscriptorNum)
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc;
		desc.NodeMask = 0;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.NumDescriptors = m_FrameCount;

		if (!Create(&desc)) {
			return false;
		}

		return true;
	}

	bool EUGD3D12DescriptorHeap::Create(const D3D12_DESCRIPTOR_HEAP_DESC* pDesc) 
	{	
		HRESULT hr = m_pDevice->CreateDescriptorHeap(pDesc, IID_PPV_ARGS(m_pHeap.GetAddressOf()));

		if (FAILED(hr)) {
			return false;
		}

		m_pHandle->HandleCPU = m_pHeap->GetCPUDescriptorHandleForHeapStart();
		m_IncrementSize = m_pDevice->GetDescriptorHandleIncrementSize(pDesc->Type);

		return true;
	}



}