#pragma once

#include <EasyUseGPU/Direct3D12/Direct3D12.hpp>
#include <EasyUseGPU/Windows/Windows.hpp>


namespace eug
{
	class EUGD3D12CmdQueue
	{

	public:

		EUGD3D12CmdQueue(ID3D12Device* pDevice):m_pDevice(pDevice){}
		~EUGD3D12CmdQueue(){}

		bool Init() 
		{
			D3D12_COMMAND_QUEUE_DESC desc;
			desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
			desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			desc.NodeMask = 0;

			return CreateD3D12CmdQueue(desc);
		}

		ID3D12CommandQueue* GetCmdQueue()
		{
			return m_pCmdQueue.Get();
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
