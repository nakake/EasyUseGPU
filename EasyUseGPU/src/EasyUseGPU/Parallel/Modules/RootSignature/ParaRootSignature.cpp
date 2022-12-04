#include <EasyUseGPU/Parallel/Engine/ParallelEngine.hpp>

namespace eug
{
	bool EUGParallelEngine::CreateRootSignature(uint32_t SrvNums) 
	{
        // Root parameter can be a table, root descriptor or root constants.
        vector<CD3DX12_ROOT_PARAMETER> slotRootParameter;

        // Perfomance TIP: Order from most frequent to least frequent.
        // SRV用パラメータ
        CD3DX12_ROOT_PARAMETER para; 
        for (int i = 0; i < SrvNums; i++)
        {
            para.InitAsShaderResourceView(i);
            slotRootParameter.push_back(para);
        }

        // UAV用パラメータ
        para.InitAsUnorderedAccessView(0);
        slotRootParameter.push_back(para);

        // A root signature is an array of root parameters.
        CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(slotRootParameter.size(), slotRootParameter.data(),
            0, nullptr,
            D3D12_ROOT_SIGNATURE_FLAG_NONE);

        ComPtr<ID3DBlob> signature;
        ComPtr<ID3DBlob> error;
        auto hr =D3D12SerializeRootSignature(
            &rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, 
            signature.GetAddressOf(), error.GetAddressOf());

         hr = m_pDevice->CreateRootSignature(
            0, signature->GetBufferPointer(), signature->GetBufferSize(), 
            IID_PPV_ARGS(m_pComputeRootSignature.GetAddressOf()));

         return true;
	}
}