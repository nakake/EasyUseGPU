#include "ParallelEngine.hpp"

#include <iostream>

namespace eug
{

    bool EUGParallelEngine::ScrutinyState(const EUG_PARALLEL_STATE* pParaState)
    {
        if (pParaState == nullptr)
        {
            return false;
        }

        if (pParaState->FilePath.empty())
        {
            //ERROR:ファイル名が設定されていません
            //エントリーポイントを指定する場合はファイル名を設定してください
            return false;
        }
        
        return true;
    }

	void EUGParallelEngine::Init()
	{

        InitPara();

	}

    void EUGParallelEngine::Wait()
    {
        int32_t fenceValue = 0;
        m_pQueue->Signal(m_pFence.Get(), ++fenceValue);
        //待ち
        while (m_pFence->GetCompletedValue() < fenceValue) {
            ;
        }
    }

    void EUGParallelEngine::CommandExecute()
    {
        m_pCmdList->Close();

        ID3D12CommandList* cmdLists[] = { m_pCmdList.Get() };
        m_pQueue->ExecuteCommandLists(1, cmdLists);

        Wait();
    }

    bool EUGParallelEngine::ParaSetParaState(const EUG_PARALLEL_STATE* pParaState)
    {
        if (!ScrutinyState(pParaState))
        {
            return m_IsSettingParaState = false;
        }

        m_pParaState = pParaState;

        return m_IsSettingParaState = true;
    }

    bool EUGParallelEngine::ParaExecutionGPU(uint32_t x, uint32_t y, uint32_t z)
    {
        if (!m_IsSettingParaState)return m_IsSettingParaState;

        //rootsig
        CreateRootSignature(static_cast<uint32_t>(m_pUploadBuffers.size()));
        m_pCmdList->SetComputeRootSignature(m_pComputeRootSignature.Get());

        //pipeline
        CreatePipeline(m_pParaState);
        m_pCmdList->SetPipelineState(m_pComputePipeline.Get());

        ID3D12DescriptorHeap* descHeaps[] = { m_pResourceHeap.Get() };
        m_pCmdList->SetDescriptorHeaps(1, descHeaps);

        SetResource();

        m_pCmdList->Dispatch(x, y, z);
        
        return true;
    }
}