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
        
        EUG_PARALLEL_STATE ParaState = {};
        
        if (pParaState == nullptr)
        {
            //ステート設定がなかった場合の処理を入れる
            
            ParaState.FilePath = L"../../EasyUseGPU/src/EasyUseGPU/Parallel/Shader/DefaultCS.hlsl";
            ParaState.EntryPoint = "MulIntMatrix";
        }
        else
        {
            ParaState = *pParaState;
        }

        D3D_SHADER_MACRO pDefines[] = {
            {"threadX", "2"},
            {"threadY", "2"},
            {"threadZ", "1"},
            { NULL , NULL }
        };

        HRESULT Result = D3DCompileFromFile(
            ParaState.FilePath.c_str(),pDefines, nullptr, 
            ParaState.EntryPoint.c_str(), ParaState.VersionHLSL.c_str(),
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