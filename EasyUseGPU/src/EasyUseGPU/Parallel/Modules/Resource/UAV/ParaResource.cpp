#include <EasyUseGPU/Parallel/Engine/ParallelEngine.hpp>

namespace eug
{
	bool EUGParallelEngine::CreateReadResource(const uint32_t DataSize)
	{
        ComPtr<ID3D12Resource> OutputResource = nullptr;
        ComPtr<ID3D12Resource> ReadResource = nullptr;
        auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
        auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(
            DataSize, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

        auto result = m_pDevice->CreateCommittedResource(
            &heapProp,
            D3D12_HEAP_FLAG_NONE,
            &resourceDesc,
            D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
            nullptr,
            IID_PPV_ARGS(OutputResource.GetAddressOf()));

        heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_READBACK);
        resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(DataSize);

        result = m_pDevice->CreateCommittedResource(
            &heapProp,
            D3D12_HEAP_FLAG_NONE,
            &resourceDesc,
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS(ReadResource.GetAddressOf()));

        m_pOutputBuffer.push_back(OutputResource);
        m_pReadBuffer.push_back(ReadResource);
        
        ++m_ReadBufferNum;
        return true;
	}

    bool EUGParallelEngine ::CreateUploadResource(const void* pData, const uint32_t DataSize)
    {
        ComPtr<ID3D12Resource> InputBuffer = nullptr;
        ComPtr<ID3D12Resource> UploadBuffer = nullptr;

        InputBuffer = Resource::CreateUploadBuffer(
            m_pDevice.Get(), m_pCmdList.Get(), pData, DataSize, UploadBuffer);

        m_pInputBuffers.push_back(InputBuffer);
        m_pUploadBuffers.push_back(UploadBuffer);

        ++m_UploadBufferNum;

        return true;
    }

    bool EUGParallelEngine::SetResource()
    {   

        for (uint32_t i = 0; i < m_UploadBufferNum; ++i)
        {
            m_pCmdList->SetComputeRootShaderResourceView(i, m_pInputBuffers[i]->GetGPUVirtualAddress());
        }

        for (uint32_t i = 0; i < m_ReadBufferNum; ++i)
        {
            m_pCmdList->SetComputeRootUnorderedAccessView(i + m_UploadBufferNum + m_Upload2DBufferNum, m_pOutputBuffer[i]->GetGPUVirtualAddress());
        }
        

        return true;
    }

    bool EUGParallelEngine::GetGPUResource()
    {
        for (uint32_t i = 0; i < m_ReadBufferNum; ++i)
        {
            auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_pOutputBuffer[i].Get(),
                D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);

            m_pCmdList->ResourceBarrier(1, &resourceBarrier);

            m_pCmdList->CopyResource(m_pReadBuffer[i].Get(), m_pOutputBuffer[i].Get());

            resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_pOutputBuffer[i].Get(),
                D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

            m_pCmdList->ResourceBarrier(1, &resourceBarrier);
        }

    
        return true;
    }


}