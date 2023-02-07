#include "GraphicsEngine.hpp"

namespace eug
{
    bool EUGGraphicsEngine::InitObjects()
    {

        HRESULT  hr;
        {
            D3D12_COMMAND_QUEUE_DESC desc = {};
            desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
            desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
            desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
            desc.NodeMask = 0;

            hr = m_pDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(m_pQueue.GetAddressOf()));
            if (FAILED(hr))
            {
                return false;
            }
        }

        // �X���b�v�`�F�C���̐���.
        {
            // DXGI�t�@�N�g���[�̐���.
            ComPtr<IDXGIFactory4> pFactory = nullptr;
            hr = CreateDXGIFactory1(IID_PPV_ARGS(pFactory.GetAddressOf()));
            if (FAILED(hr))
            {
                return false;
            }

            // �X���b�v�`�F�C���̐ݒ�.
            DXGI_SWAP_CHAIN_DESC desc = {};
            desc.BufferDesc.Width = m_WndWidth;
            desc.BufferDesc.Height = m_WndHeight;
            desc.BufferDesc.RefreshRate.Numerator = 60;
            desc.BufferDesc.RefreshRate.Denominator = 1;
            desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
            desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
            desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            desc.SampleDesc.Count = 1;
            desc.SampleDesc.Quality = 0;
            desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            desc.BufferCount = m_FrameCount;
            desc.OutputWindow = m_Window;
            desc.Windowed = TRUE;
            desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
            desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

            // �X���b�v�`�F�C���̐���.
            ComPtr<IDXGISwapChain> pSwapChain;
            hr = pFactory->CreateSwapChain(m_pQueue.Get(), &desc, pSwapChain.GetAddressOf());
            if (FAILED(hr))
            {
                return false;
            }

            // IDXGISwapChain3 ���擾.
            hr = pSwapChain.As(&m_pSwapChain);
            if (FAILED(hr))
            {
                return false;
            }

            // �o�b�N�o�b�t�@�ԍ����擾.
            m_FrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

            // �s�v�ɂȂ����̂ŉ��.
            pFactory.Reset();
            pSwapChain.Reset();
        }

        // �R�}���h�A���P�[�^�̐���.
        {
            for (auto i = 0u; i < m_FrameCount; ++i)
            {
                hr = m_pDevice->CreateCommandAllocator(
                    D3D12_COMMAND_LIST_TYPE_DIRECT,
                    IID_PPV_ARGS(m_pCmdAllocator[i].GetAddressOf()));
                if (FAILED(hr))
                {
                    return false;
                }
            }
        }

        // �R�}���h���X�g�̐���.
        {
            hr = m_pDevice->CreateCommandList(
                0,
                D3D12_COMMAND_LIST_TYPE_DIRECT,
                m_pCmdAllocator[m_FrameIndex].Get(),
                nullptr,
                IID_PPV_ARGS(m_pCmdList.GetAddressOf()));
            if (FAILED(hr))
            {
                return false;
            }
        }

        // �����_�[�^�[�Q�b�g�r���[�̐���.
        {
            // �f�B�X�N���v�^�q�[�v�̐ݒ�.
            D3D12_DESCRIPTOR_HEAP_DESC desc = {};
            desc.NumDescriptors = m_FrameCount;
            desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
            desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
            desc.NodeMask = 0;

            // �f�B�X�N���v�^�q�[�v�𐶐�.
            hr = m_pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(m_pHeapRTV.GetAddressOf()));
            if (FAILED(hr))
            {
                return false;
            }

            auto handle = m_pHeapRTV->GetCPUDescriptorHandleForHeapStart();
            auto incrementSize = m_pDevice
                ->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

            for (auto i = 0u; i < m_FrameCount; ++i)
            {
                hr = m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(m_pRTVBuffer[i].GetAddressOf()));
                if (FAILED(hr))
                {
                    return false;
                }

                D3D12_RENDER_TARGET_VIEW_DESC viewDesc = {};
                viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
                viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
                viewDesc.Texture2D.MipSlice = 0;
                viewDesc.Texture2D.PlaneSlice = 0;

                // �����_�[�^�[�Q�b�g�r���[�̐���.
                m_pDevice->CreateRenderTargetView(m_pRTVBuffer[i].Get(), &viewDesc, handle);

                m_HandleRTV[i] = handle;
                handle.ptr += incrementSize;
            }
        }

        // �t�F���X�̐���.
        {
            // �t�F���X�J�E���^�[�����Z�b�g.
            for (auto i = 0u; i < m_FrameCount; ++i)
            {
                m_FenceCounter[i] = 0;
            }

            // �t�F���X�̐���.
            hr = m_pDevice->CreateFence(
                m_FenceCounter[m_FrameIndex],
                D3D12_FENCE_FLAG_NONE,
                IID_PPV_ARGS(m_pFence.GetAddressOf()));
            if (FAILED(hr))
            {
                return false;
            }

            m_FenceCounter[m_FrameIndex]++;

            // �C�x���g�̐���.
            m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
            if (m_FenceEvent == nullptr)
            {
                return false;
            }
        }

        // �R�}���h���X�g�����.
        m_pCmdList->Close();

        return true;
    }
}