#include <EasyUseGPU/Parallel/Engine/ParallelEngine.hpp>

namespace eug
{
	bool EUGParallelEngine::CreateResource(const vector<int>& pOutputData)
	{
        auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
        auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(
            pOutputData.size() * sizeof(pOutputData[0]), D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

        auto result = m_pDevice->CreateCommittedResource(
            &heapProp,
            D3D12_HEAP_FLAG_NONE,
            &resourceDesc,
            D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
            nullptr,
            IID_PPV_ARGS(m_pOutputBuffer.GetAddressOf()));

        heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_READBACK);
        resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(pOutputData.size() * sizeof(pOutputData[0]));

        result = m_pDevice->CreateCommittedResource(
            &heapProp,
            D3D12_HEAP_FLAG_NONE,
            &resourceDesc,
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS(m_pReadBuffer.GetAddressOf()));

        return true;
	}

    bool EUGParallelEngine::SetResource(uint32_t SrvNums, vector<vector<int>>& inputData)
    {   
        m_pInputBuffers.resize(SrvNums);
        m_pUploadBuffers.resize(SrvNums);

        for (int i = 0; i < SrvNums; ++i)
        {
            m_pInputBuffers[i] = Resource::CreateUploadBuffer(
                m_pDevice, m_pCmdList.Get(), (void*)inputData[i].data(),
                inputData[i].size() * sizeof(inputData[i][0]), m_pUploadBuffers[i]);
        
            m_pCmdList->SetComputeRootShaderResourceView(i, m_pInputBuffers[i]->GetGPUVirtualAddress());
        }
        
        m_pCmdList->SetComputeRootUnorderedAccessView(SrvNums, m_pOutputBuffer->GetGPUVirtualAddress());

        return true;
    }

    bool EUGParallelEngine::GetGPUResource()
    {
        auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_pOutputBuffer.Get(),
            D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);

        m_pCmdList->ResourceBarrier(1, &resourceBarrier);

        m_pCmdList->CopyResource(m_pReadBuffer.Get(), m_pOutputBuffer.Get());

        resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_pOutputBuffer.Get(),
            D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

        m_pCmdList->ResourceBarrier(1, &resourceBarrier);
    
        return true;
    }


}