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

        // スワップチェインの生成.
        {
            // DXGIファクトリーの生成.
            ComPtr<IDXGIFactory4> pFactory = nullptr;
            hr = CreateDXGIFactory1(IID_PPV_ARGS(pFactory.GetAddressOf()));
            if (FAILED(hr))
            {
                return false;
            }

            // スワップチェインの設定.
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

            // スワップチェインの生成.
            ComPtr<IDXGISwapChain> pSwapChain;
            hr = pFactory->CreateSwapChain(m_pQueue.Get(), &desc, pSwapChain.GetAddressOf());
            if (FAILED(hr))
            {
                return false;
            }

            // IDXGISwapChain3 を取得.
            hr = pSwapChain.As(&m_pSwapChain);
            if (FAILED(hr))
            {
                return false;
            }

            // バックバッファ番号を取得.
            m_FrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

            // 不要になったので解放.
            pFactory.Reset();
            pSwapChain.Reset();
        }

        // コマンドアロケータの生成.
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

        // コマンドリストの生成.
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

        // レンダーターゲットビューの生成.
        {
            // ディスクリプタヒープの設定.
            D3D12_DESCRIPTOR_HEAP_DESC desc = {};
            desc.NumDescriptors = m_FrameCount;
            desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
            desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
            desc.NodeMask = 0;

            // ディスクリプタヒープを生成.
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

                // レンダーターゲットビューの生成.
                m_pDevice->CreateRenderTargetView(m_pRTVBuffer[i].Get(), &viewDesc, handle);

                m_HandleRTV[i] = handle;
                handle.ptr += incrementSize;
            }
        }

        // フェンスの生成.
        {
            // フェンスカウンターをリセット.
            for (auto i = 0u; i < m_FrameCount; ++i)
            {
                m_FenceCounter[i] = 0;
            }

            // フェンスの生成.
            hr = m_pDevice->CreateFence(
                m_FenceCounter[m_FrameIndex],
                D3D12_FENCE_FLAG_NONE,
                IID_PPV_ARGS(m_pFence.GetAddressOf()));
            if (FAILED(hr))
            {
                return false;
            }

            m_FenceCounter[m_FrameIndex]++;

            // イベントの生成.
            m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
            if (m_FenceEvent == nullptr)
            {
                return false;
            }
        }

        // コマンドリストを閉じる.
        m_pCmdList->Close();

        return true;
    }
}