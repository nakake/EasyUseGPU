#include "ParallelEngine.hpp"

#include <iostream>

namespace eug
{

    bool EUGParallelEngine::ScrutinyState(const EUG_PARALLEL_STATE* pParaState)
    {
        if (pParaState == nullptr)
        {
            return true;
        }

        if (!pParaState->FilePath.empty() && pParaState->EntryPoint.empty())
        {
            //ERROR:�G���g���[�|�C���g���ݒ肳��Ă��܂���
            //�t�@�C���w�������ꍇ�̓G���g���[�|�C���g��ݒ肵�Ă�������
            return false;
        }

        if (pParaState->FilePath.empty() && !pParaState->EntryPoint.empty())
        {
            //ERROR:�t�@�C�������ݒ肳��Ă��܂���
            //�G���g���[�|�C���g���w�肷��ꍇ�̓t�@�C������ݒ肵�Ă�������
            return false;
        }

        if (pParaState->TheadGroupNums.x == 0 || 
            pParaState->TheadGroupNums.y == 0 || 
            pParaState->TheadGroupNums.z == 0 ||
            pParaState->TheadNums.x == 0 || 
            pParaState->TheadNums.y == 0 || 
            pParaState->TheadNums.z == 0  )
        {
            //ERROR:�X���b�h������уX���b�h�O���[�v����0���w�肷�邱�Ƃ͂ł��܂���
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
        //�҂�
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

	void EUGParallelEngine::Run()
	{
        //int srvnums = 2;
        ////rootsig
        //CreateRootSignature(srvnums);
        //m_pCmdList->SetComputeRootSignature(m_pComputeRootSignature.Get());

        ////pipeline
        ////CreatePipeline(
        ////    L"../../EasyUseGPU/src/EasyUseGPU/Parallel/Shader/TestCS.hlsl",
        ////    "MulMatrix", "cs_5_1");
        //m_pCmdList->SetPipelineState(m_pComputePipeline.Get());
        //
        ////resource
        //
        //uavData.resize(4);
        //inputData.resize(srvnums);

        //for (auto& a:inputData)
        //{
        //    a.resize(4);
        //    int c = 10;
        //    for (auto& b : a)
        //    {
        //        b = 10 + c;
        //    }
        //    c = 20;
        //}

        //CreateResource();
        //    
        //ID3D12DescriptorHeap* descHeaps[] = { m_pResourceHeap.Get() };
        //m_pCmdList->SetDescriptorHeaps(1, descHeaps);

        //SetResource(srvnums);

        //m_pCmdList->Dispatch(1, 1, 1);

        //GetGPUResource();

        //CommandExecute();

        //ReadGPUData(m_pReadBuffer, uavData);

        //auto a = uavData[0];
    }
}