#pragma once

#include "GraphicsEngine.hpp"
#include <cassert>


namespace eug
{

    void EUGGraphicsEngine::Present(uint32_t interval)
    {
        // 画面に表示.
        m_pSwapChain->Present(interval, 0);

        // シグナル処理.
        const auto currentValue = m_FenceCounter[m_FrameIndex];
        m_pQueue->Signal(m_pFence.Get(), currentValue);

        // バックバッファ番号を更新.
        m_FrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

        // 次のフレームの描画準備がまだであれば待機する
        if (m_pFence->GetCompletedValue() < m_FenceCounter[m_FrameIndex])
        {
            m_pFence->SetEventOnCompletion(m_FenceCounter[m_FrameIndex], m_FenceEvent);
            WaitForSingleObjectEx(m_FenceEvent, INFINITE, FALSE);
        }

        // 次のフレームのフェンスカウンターを増やす.
        m_FenceCounter[m_FrameIndex] = currentValue + 1;
    }

    void EUGGraphicsEngine::StartRender()
    {
        // コマンドの記録を開始.
        m_pCmdAllocator[m_FrameIndex]->Reset();
        m_pCmdList->Reset(m_pCmdAllocator[m_FrameIndex].Get(), nullptr);

        // リソースバリアの設定.
        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource = m_pRTVBuffer[m_FrameIndex].Get();
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        // リソースバリア.
        m_pCmdList->ResourceBarrier(1, &barrier);

        // レンダーゲットの設定.
        m_pCmdList->OMSetRenderTargets(1, &m_HandleRTV[m_FrameIndex], FALSE, nullptr);

        // クリアカラーの設定.
        float clearColor[] = { 0.25f, 0.25f, 0.25f, 1.0f };

        // レンダーターゲットビューをクリア.
        m_pCmdList->ClearRenderTargetView(m_HandleRTV[m_FrameIndex], clearColor, 0, nullptr);
    }

     void EUGGraphicsEngine::EndRender()
     {
         // 描画処理.
         {
             m_pCmdList->SetGraphicsRootSignature(m_pRootSignature.Get());
             m_pCmdList->SetDescriptorHeaps(1, m_pHeapCBV.GetAddressOf());
             m_pCmdList->SetGraphicsRootConstantBufferView(0, m_CBV[m_FrameIndex].Desc.BufferLocation);
             m_pCmdList->SetPipelineState(m_pPSO.Get());

             m_pCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
             m_pCmdList->IASetVertexBuffers(0, 1, &m_VBV);
             m_pCmdList->RSSetViewports(1, &m_Viewport);
             m_pCmdList->RSSetScissorRects(1, &m_Scissor);

             m_pCmdList->DrawInstanced(3, 1, 0, 0);
         }

        // リソースバリアの設定.
        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource = m_pRTVBuffer[m_FrameIndex].Get();
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        // リソースバリア.
        m_pCmdList->ResourceBarrier(1, &barrier);

        // コマンドの記録を終了.
        m_pCmdList->Close();

        // コマンドを実行.
        ID3D12CommandList* ppCmdLists[] = { m_pCmdList.Get() };
        m_pQueue->ExecuteCommandLists(1, ppCmdLists);

        // 画面に表示.
        Present(1);
    }

    void EUGGraphicsEngine::WaitGpu()
    {
        assert(m_pQueue != nullptr);
        assert(m_pFence != nullptr);
        assert(m_FenceEvent != nullptr);

        // シグナル処理.
        m_pQueue->Signal(m_pFence.Get(), m_FenceCounter[m_FrameIndex]);

        // 完了時にイベントを設定する..
        m_pFence->SetEventOnCompletion(m_FenceCounter[m_FrameIndex], m_FenceEvent);

        // 待機処理.
        WaitForSingleObjectEx(m_FenceEvent, INFINITE, FALSE);

        // カウンターを増やす.
        m_FenceCounter[m_FrameIndex]++;
    }
}