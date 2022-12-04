#include "GraphicsEngine.hpp"

namespace eug
{
    bool EUGGraphicsEngine::OnInit()
    {
        // 頂点バッファの生成.
        {
            // 頂点データ.
            Vertex vertices[] = {
                { DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
                { DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
                { DirectX::XMFLOAT3(0.0f,  1.0f, 0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
            };

            // ヒーププロパティ.
            D3D12_HEAP_PROPERTIES prop = {};
            prop.Type = D3D12_HEAP_TYPE_UPLOAD;
            prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            prop.CreationNodeMask = 1;
            prop.VisibleNodeMask = 1;

            // リソースの設定.
            D3D12_RESOURCE_DESC desc = {};
            desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
            desc.Alignment = 0;
            desc.Width = sizeof(vertices);
            desc.Height = 1;
            desc.DepthOrArraySize = 1;
            desc.MipLevels = 1;
            desc.Format = DXGI_FORMAT_UNKNOWN;
            desc.SampleDesc.Count = 1;
            desc.SampleDesc.Quality = 0;
            desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
            desc.Flags = D3D12_RESOURCE_FLAG_NONE;

            // リソースを生成.
            auto hr = m_pDevice->CreateCommittedResource(
                &prop,
                D3D12_HEAP_FLAG_NONE,
                &desc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(m_pVB.GetAddressOf()));
            if (FAILED(hr))
            {
                return false;
            }

            // マッピングする.
            void* ptr = nullptr;
            hr = m_pVB->Map(0, nullptr, &ptr);
            if (FAILED(hr))
            {
                return false;
            }

            // 頂点データをマッピング先に設定.
            memcpy(ptr, vertices, sizeof(vertices));

            // マッピング解除.
            m_pVB->Unmap(0, nullptr);

            // 頂点バッファビューの設定.
            m_VBV.BufferLocation = m_pVB->GetGPUVirtualAddress();
            m_VBV.SizeInBytes = static_cast<UINT>(sizeof(vertices));
            m_VBV.StrideInBytes = static_cast<UINT>(sizeof(Vertex));
        }

        // 定数バッファ用ディスクリプタヒープの生成.
        {
            D3D12_DESCRIPTOR_HEAP_DESC desc = {};
            desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            desc.NumDescriptors = 1 * m_FrameCount;
            desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
            desc.NodeMask = 0;

            auto hr = m_pDevice->CreateDescriptorHeap(
                &desc,
                IID_PPV_ARGS(m_pHeapCBV.GetAddressOf()));
            if (FAILED(hr))
            {
                return false;
            }
        }

        // 定数バッファの生成.
        {
            // ヒーププロパティ.
            D3D12_HEAP_PROPERTIES prop = {};
            prop.Type = D3D12_HEAP_TYPE_UPLOAD;
            prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            prop.CreationNodeMask = 1;
            prop.VisibleNodeMask = 1;

            // リソースの設定.
            D3D12_RESOURCE_DESC desc = {};
            desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
            desc.Alignment = 0;
            desc.Width = sizeof(Transform);
            desc.Height = 1;
            desc.DepthOrArraySize = 1;
            desc.MipLevels = 1;
            desc.Format = DXGI_FORMAT_UNKNOWN;
            desc.SampleDesc.Count = 1;
            desc.SampleDesc.Quality = 0;
            desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
            desc.Flags = D3D12_RESOURCE_FLAG_NONE;

            auto incrementSize = m_pDevice
                ->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

            for (auto i = 0; i < m_FrameCount; ++i)
            {
                // リソース生成.
                auto hr = m_pDevice->CreateCommittedResource(
                    &prop,
                    D3D12_HEAP_FLAG_NONE,
                    &desc,
                    D3D12_RESOURCE_STATE_GENERIC_READ,
                    nullptr,
                    IID_PPV_ARGS(m_pCB[i].GetAddressOf()));
                if (FAILED(hr))
                {
                    return false;
                }

                auto address = m_pCB[i]->GetGPUVirtualAddress();
                auto handleCPU = m_pHeapCBV->GetCPUDescriptorHandleForHeapStart();
                auto handleGPU = m_pHeapCBV->GetGPUDescriptorHandleForHeapStart();

                handleCPU.ptr += incrementSize * i;
                handleGPU.ptr += incrementSize * i;

                // 定数バッファビューの設定.
                m_CBV[i].HandleCPU = handleCPU;
                m_CBV[i].HandleGPU = handleGPU;
                m_CBV[i].Desc.BufferLocation = address;
                m_CBV[i].Desc.SizeInBytes = sizeof(Transform);

                // 定数バッファビューを生成.
                m_pDevice->CreateConstantBufferView(&m_CBV[i].Desc, handleCPU);

                // マッピング.
                hr = m_pCB[i]->Map(0, nullptr, reinterpret_cast<void**>(&m_CBV[i].pBuffer));
                if (FAILED(hr))
                {
                    return false;
                }

                auto eyePos = DirectX::XMVectorSet(0.0f, 0.0f, 5.0f, 0.0f);
                auto targetPos = DirectX::XMVectorZero();
                auto upward = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

                auto fovY = DirectX::XMConvertToRadians(37.5f);
                auto aspect = static_cast<float>(m_WndWidth) / static_cast<float>(m_WndHeight);

                // 変換行列の設定.
                m_CBV[i].pBuffer->World = DirectX::XMMatrixIdentity();
                m_CBV[i].pBuffer->View = DirectX::XMMatrixLookAtRH(eyePos, targetPos, upward);
                m_CBV[i].pBuffer->Proj = DirectX::XMMatrixPerspectiveFovRH(fovY, aspect, 1.0f, 1000.0f);
            }
        }

        // ルートシグニチャの生成.
        {
            auto flag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
            flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
            flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
            flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

            // ルートパラメータの設定.
            D3D12_ROOT_PARAMETER param = {};
            param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
            param.Descriptor.ShaderRegister = 0;
            param.Descriptor.RegisterSpace = 0;
            param.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

            // ルートシグニチャの設定.
            D3D12_ROOT_SIGNATURE_DESC desc = {};
            desc.NumParameters = 1;
            desc.NumStaticSamplers = 0;
            desc.pParameters = &param;
            desc.pStaticSamplers = nullptr;
            desc.Flags = flag;

            ComPtr<ID3DBlob> pBlob;
            ComPtr<ID3DBlob> pErrorBlob;

            // シリアライズ
            auto hr = D3D12SerializeRootSignature(
                &desc,
                D3D_ROOT_SIGNATURE_VERSION_1_0,
                pBlob.GetAddressOf(),
                pErrorBlob.GetAddressOf());
            if (FAILED(hr))
            {
                return false;
            }

            // ルートシグニチャを生成.
            hr = m_pDevice->CreateRootSignature(
                0,
                pBlob->GetBufferPointer(),
                pBlob->GetBufferSize(),
                IID_PPV_ARGS(m_pRootSignature.GetAddressOf()));
            if (FAILED(hr))
            {
                return false;
            }
        }

        // パイプラインステートの生成.
        {
            // 入力レイアウトの設定.
            D3D12_INPUT_ELEMENT_DESC elements[2];
            elements[0].SemanticName = "POSITION";
            elements[0].SemanticIndex = 0;
            elements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
            elements[0].InputSlot = 0;
            elements[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
            elements[0].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
            elements[0].InstanceDataStepRate = 0;

            elements[1].SemanticName = "COLOR";
            elements[1].SemanticIndex = 0;
            elements[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            elements[1].InputSlot = 0;
            elements[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
            elements[1].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
            elements[1].InstanceDataStepRate = 0;

            // ラスタライザーステートの設定.
            D3D12_RASTERIZER_DESC descRS;
            descRS.FillMode = D3D12_FILL_MODE_SOLID;
            descRS.CullMode = D3D12_CULL_MODE_NONE;
            descRS.FrontCounterClockwise = FALSE;
            descRS.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
            descRS.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
            descRS.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
            descRS.DepthClipEnable = FALSE;
            descRS.MultisampleEnable = FALSE;
            descRS.AntialiasedLineEnable = FALSE;
            descRS.ForcedSampleCount = 0;
            descRS.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

            // レンダーターゲットのブレンド設定.
            D3D12_RENDER_TARGET_BLEND_DESC descRTBS = {
                FALSE, FALSE,
                D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
                D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
                D3D12_LOGIC_OP_NOOP,
                D3D12_COLOR_WRITE_ENABLE_ALL
            };

            // ブレンドステートの設定.
            D3D12_BLEND_DESC descBS;
            descBS.AlphaToCoverageEnable = FALSE;
            descBS.IndependentBlendEnable = FALSE;
            for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
            {
                descBS.RenderTarget[i] = descRTBS;
            }

            ComPtr<ID3DBlob> pVSBlob;
            ComPtr<ID3DBlob> pPSBlob;

            // 頂点シェーダ読み込み.
            auto hr = D3DReadFileToBlob(L"TestVertexShader.cso", pVSBlob.GetAddressOf());
            if (FAILED(hr))
            {
                return false;
            }

            // ピクセルシェーダ読み込み.
            hr = D3DReadFileToBlob(L"TestPixelShader.cso", pPSBlob.GetAddressOf());
            if (FAILED(hr))
            {
                return false;
            }

            // パイプラインステートの設定.
            D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
            desc.InputLayout = { elements, _countof(elements) };
            desc.pRootSignature = m_pRootSignature.Get();
            desc.VS = { pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize() };
            desc.PS = { pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize() };
            desc.RasterizerState = descRS;
            desc.BlendState = descBS;
            desc.DepthStencilState.DepthEnable = FALSE;
            desc.DepthStencilState.StencilEnable = FALSE;
            desc.SampleMask = UINT_MAX;
            desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
            desc.NumRenderTargets = 1;
            desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
            desc.DSVFormat = DXGI_FORMAT_UNKNOWN;
            desc.SampleDesc.Count = 1;
            desc.SampleDesc.Quality = 0;

            // パイプラインステートを生成.
            hr = m_pDevice->CreateGraphicsPipelineState(
                &desc,
                IID_PPV_ARGS(m_pPSO.GetAddressOf()));
            if (FAILED(hr))
            {
                return false;
            }
        }

        // ビューポートとシザー矩形の設定.
        {
            m_Viewport.TopLeftX = 0;
            m_Viewport.TopLeftY = 0;
            m_Viewport.Width = static_cast<float>(m_WndWidth);
            m_Viewport.Height = static_cast<float>(m_WndHeight);
            m_Viewport.MinDepth = 0.0f;
            m_Viewport.MaxDepth = 1.0f;

            m_Scissor.left = 0;
            m_Scissor.right = m_WndWidth;
            m_Scissor.top = 0;
            m_Scissor.bottom = m_WndHeight;
        }

        return true;
    }

}