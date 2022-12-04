#pragma once

#include <EasyUseGPU/Windows/Windows.hpp>
#include <EasyUseGPU/Direct3D12/Direct3D12.hpp>

#include <EasyUseGPU/DirectX12/Direct3D12/Device/D3D12Device.hpp>

namespace eug 
{
	class ParaCmdList
	{
	public:
		ParaCmdList(ID3D12Device* pDevice, ID3D12PipelineState* pPipeline)
			:m_pDevice(pDevice), m_pPipeline(pPipeline)
		{
		}

		~ParaCmdList(){}

		bool Init(D3D12_COMMAND_LIST_TYPE type);

		ID3D12GraphicsCommandList* GetCmdList()
		{
			return m_pCmdList.Get();
		}

	private:

		bool CreateCmdList(D3D12_COMMAND_LIST_TYPE type);
		bool CreateCmdAllocator(D3D12_COMMAND_LIST_TYPE type);

		ID3D12Device* m_pDevice = nullptr;
		ID3D12PipelineState* m_pPipeline = nullptr;
		ComPtr<ID3D12CommandAllocator> m_pCmdAllor = nullptr;
		ComPtr<ID3D12GraphicsCommandList> m_pCmdList = nullptr;

	};
}
