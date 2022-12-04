#pragma once

#include <EasyUseGPU/Direct3D12/Direct3D12.hpp>
#include <EasyUseGPU/Windows/Windows.hpp>

#include <EasyUseGPU/DirectX12/Direct3D12/CmdList/D3D12CmdList.hpp>
#include <EasyUseGPU/DirectX12/Direct3D12/Fence/D3D12Fence.hpp>

namespace eug
{
	class EUGD3D12CmdQueue
	{

	public:

		EUGD3D12CmdQueue(ID3D12Device* pDevice):m_pDevice(pDevice){}
		~EUGD3D12CmdQueue(){}

		bool Init(
			D3D12_COMMAND_LIST_TYPE type, D3D12_COMMAND_QUEUE_PRIORITY priority,
			D3D12_COMMAND_QUEUE_FLAGS flag = D3D12_COMMAND_QUEUE_FLAG_NONE, int32_t NodeMask = 0
		) 
		{
			D3D12_COMMAND_QUEUE_DESC desc;
			desc.Type = type;
			desc.Priority = priority;
			desc.Flags = flag;
			desc.NodeMask = NodeMask;

			return CreateD3D12CmdQueue(desc);
		}

		ID3D12CommandQueue* GetCmdQueue()
		{
			return m_pCmdQueue.Get();
		}

		void ExecuteCmd(EUGD3D12CmdList* pCmdList)
		{
			ID3D12CommandList* ppCmdList[] = { pCmdList->GetCmdList() };
			m_pCmdQueue->ExecuteCommandLists(1, ppCmdList);
 		}

		void Signal(EUGD3D12Fence* pFence, uint32_t FrameIndex)
		{
			const auto currentValue = pFence->GetFenceCount(FrameIndex);
			m_pCmdQueue->Signal(pFence->GetFence(), currentValue);
		}

	private:
		
		ComPtr<ID3D12CommandQueue> m_pCmdQueue = nullptr;
		ID3D12Device* m_pDevice;

		bool CreateD3D12CmdQueue(D3D12_COMMAND_QUEUE_DESC desc)
		{
			auto hr = m_pDevice
				->CreateCommandQueue(&desc, IID_PPV_ARGS(m_pCmdQueue.GetAddressOf()));
			return SUCCEEDED(hr);
		}

	};

}
