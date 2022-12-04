#include "ParaCmdList.hpp"

namespace eug
{
	bool ParaCmdList::Init(D3D12_COMMAND_LIST_TYPE type)
	{
		if (!CreateCmdAllocator(type))return false;
		return CreateCmdList(type);
	}

	bool ParaCmdList::CreateCmdAllocator(D3D12_COMMAND_LIST_TYPE type)
	{

		HRESULT hr = m_pDevice->CreateCommandAllocator(
			type,IID_PPV_ARGS(m_pCmdAllor.GetAddressOf()));

		if (FAILED(hr))return false;

		return true;
	}

	bool ParaCmdList::CreateCmdList(D3D12_COMMAND_LIST_TYPE type)
	{
		HRESULT hr = m_pDevice->CreateCommandList(
			0,
			type,
			m_pCmdAllor.Get(),
			m_pPipeline,
			IID_PPV_ARGS(m_pCmdList.GetAddressOf()));

		return SUCCEEDED(hr);
	}
}