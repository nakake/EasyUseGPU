#include <EasyUseGPU/Parallel/Engine/ParallelEngine.hpp>

namespace eug
{
	bool EUGParallelEngine::CreateRootSignature(uint32_t SrvNums) 
	{
        // note:ルートパラメータには、テーブル、ルートディスクリプタ、ルート定数を指定することができる。
        vector<CD3DX12_ROOT_PARAMETER> slotRootParameter;

        // note<パフォーマンスのヒント>：頻度の高いものから低いものへと順番に並べる。
        // SRV用パラメータ
        CD3DX12_ROOT_PARAMETER para; 
        for (int i = 0; i < m_pUploadBuffers.size(); i++)
        {
            para.InitAsShaderResourceView(i);
            slotRootParameter.push_back(para);
        }

        // UAV用パラメータ
        for (int i = 0; i < m_pReadBuffer.size(); i++)
        {
            para.InitAsUnorderedAccessView(i);
            slotRootParameter.push_back(para);
        }


        //  ルート署名は、ルートパラメータの配列。
        CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(static_cast<UINT>(slotRootParameter.size()), 
            slotRootParameter.data(),0, nullptr,
            D3D12_ROOT_SIGNATURE_FLAG_NONE);

        ComPtr<ID3DBlob> signature;
        ComPtr<ID3DBlob> error;
        auto hr = D3D12SerializeRootSignature(
            &rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, 
            signature.GetAddressOf(), error.GetAddressOf());

         hr = m_pDevice->CreateRootSignature(
            0, signature->GetBufferPointer(), signature->GetBufferSize(), 
            IID_PPV_ARGS(m_pComputeRootSignature.GetAddressOf()));

         return true;
	}
}