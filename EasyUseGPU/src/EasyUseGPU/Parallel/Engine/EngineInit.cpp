#include "ParallelEngine.hpp"

namespace eug
{

	//bool EUGParallelEngine::CreateList()
	//{
	//	m_pComputeCommandList.reset(new ParaCmdList(m_pDevice->GetDevice(), m_pPipeline->GetPipeline()));
	//	if(!m_pComputeCommandList->Init(D3D12_COMMAND_LIST_TYPE_COMPUTE)) return false;
	//	m_pGraphicCommandList.reset(new ParaCmdList(m_pDevice->GetDevice(), m_pPipeline->GetPipeline()));
	//	return m_pGraphicCommandList->Init(D3D12_COMMAND_LIST_TYPE_DIRECT);
	//}

	//bool EUGParallelEngine::CreateDescriptor()
	//{
	//	m_pDescriptor.reset(new EUGD3D12DescriptorHeap(m_pDevice->GetDevice(),D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV ,2));
	//	return m_pDescriptor->Init();
	//}

	//bool EUGParallelEngine::CreateQueue()
	//{
	//	m_pCommandQueue.reset(new EUGD3D12CmdQueue(m_pDevice->GetDevice()));
	//	return m_pCommandQueue->Init(D3D12_COMMAND_LIST_TYPE_COMPUTE, D3D12_COMMAND_QUEUE_PRIORITY_NORMAL);
	//}

	//void EUGParallelEngine::CreateResource()
	//{
	//	m_pUAV.reset(new EUGResouceUAV(m_pDevice->GetDevice(), m_pDescriptor.get()));
	//	m_pSRV.reset(new EUGResourceSRV(m_pDevice->GetDevice(), m_pDescriptor.get()));
	//}

	//bool EUGParallelEngine::CreatePipeline()
	//{
	//	m_pPipeline.reset(new ParaPipeline(m_pDevice->GetDevice(), m_pRootSignature.get()));
	//	return m_pPipeline->Init();
	//}

	//bool EUGParallelEngine::CreateRootSignature() 
	//{
	//	m_pRootSignature.reset(new ParaRootSignature(m_pDevice->GetDevice()));
	//	return m_pRootSignature->Init();
	//}

	bool EUGParallelEngine::InitPara()
	{
		D3D12_COMMAND_QUEUE_DESC desc = {};
		desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 0;

		HRESULT hr = m_pDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(m_pQueue.GetAddressOf()));
		if (FAILED(hr))
		{
			return false;
		}

		hr = m_pDevice->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(m_pAllocator.GetAddressOf()));

		hr = m_pDevice->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			m_pAllocator.Get(),
			nullptr,
			IID_PPV_ARGS(m_pCmdList.GetAddressOf()));
		if (FAILED(hr))
		{
			return false;
		}

		hr = m_pDevice->CreateFence(
			0,
			D3D12_FENCE_FLAG_NONE,
			IID_PPV_ARGS(m_pFence.GetAddressOf()));
		if (FAILED(hr))
		{
			return false;
		}

		D3D12_DESCRIPTOR_HEAP_DESC Heapdesc = {};
		Heapdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		Heapdesc.NumDescriptors = 2;
		Heapdesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		Heapdesc.NodeMask = 0;

		hr = m_pDevice->CreateDescriptorHeap(
			&Heapdesc,
			IID_PPV_ARGS(m_pResourceHeap.GetAddressOf()));
		if (FAILED(hr))
		{
			return false;
		}

		return true;
	}
}