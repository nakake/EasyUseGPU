#include <EasyUseGPU/Parallel/Engine/ParallelEngine.hpp>
#include <filesystem>

namespace eug
{
    using std::wstring;
    using std::string;
    bool EUGParallelEngine::CreatePipeline(
        const EUG_PARALLEL_STATE* pParaState)
    {
        ComPtr<ID3DBlob> csBlob = nullptr;
        ComPtr<ID3DBlob> errBlob = nullptr;


        HRESULT Result = D3DCompileFromFile(
            pParaState->FilePath.c_str(),pParaState->pDefines.data(), nullptr,
            pParaState->EntryPoint.c_str(), pParaState->VersionHLSL.c_str(),
            0, 0, &csBlob, &errBlob);

        if (FAILED(Result))
        {
            OutputDebugStringA(static_cast<char*>(errBlob->GetBufferPointer()));
            return false;
        }

        D3D12_COMPUTE_PIPELINE_STATE_DESC desc = {};
        desc.CS.pShaderBytecode = csBlob->GetBufferPointer();
        desc.CS.BytecodeLength = csBlob->GetBufferSize();
        desc.NodeMask = 0;
        desc.pRootSignature = m_pComputeRootSignature.Get();
        Result = m_pDevice->CreateComputePipelineState(
            &desc, IID_PPV_ARGS(m_pComputePipeline.GetAddressOf()));

        if (FAILED(Result))
        {
            return false;
        }

        return true;
    }
}