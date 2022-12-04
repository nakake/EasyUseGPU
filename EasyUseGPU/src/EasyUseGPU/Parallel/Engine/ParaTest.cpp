#include "ParallelEngine.hpp"

namespace eug
{
	void EUGParallelEngine::Test()
	{

        int DataNums = 1;
        //rootsig
        auto result = CreateRootSignature(DataNums);
        m_pCmdList->SetComputeRootSignature(m_pComputeRootSignature.Get());

        //pipeline
        result = CreatePipeline(nullptr);
        m_pCmdList->SetPipelineState(m_pComputePipeline.Get());

        vector<int> ResultMatrix(4);

        result = CreateResource(ResultMatrix);

        ID3D12DescriptorHeap* descHeaps[] = { m_pResourceHeap.Get() };
        m_pCmdList->SetDescriptorHeaps(1, descHeaps);

        vector<vector<int>> InputData(DataNums);



        result = SetResource(DataNums, InputData);

        m_pCmdList->Dispatch(1, 1, 1);

        result = GetGPUResource();

        CommandExecute();

        ReadGPUData(m_pReadBuffer, ResultMatrix);

        auto a = ResultMatrix[0];
	}
}