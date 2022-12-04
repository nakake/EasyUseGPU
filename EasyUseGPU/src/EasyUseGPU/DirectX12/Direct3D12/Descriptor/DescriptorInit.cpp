#include "D3D12DescriptorHeap.hpp"

namespace eug
{

	bool EUGD3D12DescriptorHeap::Init()
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc;
		
		desc.NodeMask = 0;

		switch (m_DescriptorType)
		{
		case D3D12_DESCRIPTOR_HEAP_TYPE_RTV:

			desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			desc.NumDescriptors = m_HandleSize;

			break;

		case D3D12_DESCRIPTOR_HEAP_TYPE_DSV:

			break;
		case D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER:

			break;
		case D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV:
			desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			desc.NumDescriptors = m_HandleSize;

			break;
		default:
			return false;
		}

		m_IncrementSize = m_pDevice->GetDescriptorHandleIncrementSize(m_DescriptorType);

		return CreateHeap(&desc);
	}

	bool EUGD3D12DescriptorHeap::CreateHeap(const D3D12_DESCRIPTOR_HEAP_DESC* pDesc)
	{
		HRESULT hr = m_pDevice->CreateDescriptorHeap(pDesc, IID_PPV_ARGS(m_pHeap.GetAddressOf()));

		if (FAILED(hr)) {
			return false;
		}

		return true;
	}



}