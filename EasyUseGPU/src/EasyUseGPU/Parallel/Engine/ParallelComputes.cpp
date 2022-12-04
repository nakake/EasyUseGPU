#include "ParallelEngine.hpp"

namespace eug
{
    void EUGParallelEngine::MulMatrix(vector<int>& Data1, vector<int>& Data2,
        int ResultMatrixSize, const EUG_PARALLEL_STATE* pParaState)
    {
        ScrutinyState(pParaState);

        int DataNums = 2;
        //rootsig
        auto result = CreateRootSignature(DataNums);
        m_pCmdList->SetComputeRootSignature(m_pComputeRootSignature.Get());

        //pipeline
        result = CreatePipeline(pParaState);
        m_pCmdList->SetPipelineState(m_pComputePipeline.Get());

        vector<int> ResultMatrix(ResultMatrixSize);

        result = CreateResource(ResultMatrix);

        ID3D12DescriptorHeap* descHeaps[] = { m_pResourceHeap.Get() };
        m_pCmdList->SetDescriptorHeaps(1, descHeaps);

        vector<vector<int>> InputData(DataNums);

        InputData[0] = Data1;
        InputData[1] = Data2;

        result = SetResource(DataNums, InputData);

        m_pCmdList->Dispatch(1, 1, 1);

        result = GetGPUResource();

        CommandExecute();

        ReadGPUData(m_pReadBuffer, ResultMatrix);

        auto a = ResultMatrix[0];
    }
}