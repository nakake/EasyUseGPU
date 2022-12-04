#pragma once

#include "GraphicsEngine.hpp"
#include <cassert>


namespace eug
{

    void EUGGraphicsEngine::Present(uint32_t interval)
    {
        // ��ʂɕ\��.
        m_pSwapChain->Present(interval, 0);

        // �V�O�i������.
        const auto currentValue = m_FenceCounter[m_FrameIndex];
        m_pQueue->Signal(m_pFence.Get(), currentValue);

        // �o�b�N�o�b�t�@�ԍ����X�V.
        m_FrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

        // ���̃t���[���̕`�揀�����܂��ł���Αҋ@����
        if (m_pFence->GetCompletedValue() < m_FenceCounter[m_FrameIndex])
        {
            m_pFence->SetEventOnCompletion(m_FenceCounter[m_FrameIndex], m_FenceEvent);
            WaitForSingleObjectEx(m_FenceEvent, INFINITE, FALSE);
        }

        // ���̃t���[���̃t�F���X�J�E���^�[�𑝂₷.
        m_FenceCounter[m_FrameIndex] = currentValue + 1;
    }

    void EUGGraphicsEngine::StartRender()
    {
        // �R�}���h�̋L�^���J�n.
        m_pCmdAllocator[m_FrameIndex]->Reset();
        m_pCmdList->Reset(m_pCmdAllocator[m_FrameIndex].Get(), nullptr);

        // ���\�[�X�o���A�̐ݒ�.
        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource = m_pRTVBuffer[m_FrameIndex].Get();
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        // ���\�[�X�o���A.
        m_pCmdList->ResourceBarrier(1, &barrier);

        // �����_�[�Q�b�g�̐ݒ�.
        m_pCmdList->OMSetRenderTargets(1, &m_HandleRTV[m_FrameIndex], FALSE, nullptr);

        // �N���A�J���[�̐ݒ�.
        float clearColor[] = { 0.25f, 0.25f, 0.25f, 1.0f };

        // �����_�[�^�[�Q�b�g�r���[���N���A.
        m_pCmdList->ClearRenderTargetView(m_HandleRTV[m_FrameIndex], clearColor, 0, nullptr);
    }

     void EUGGraphicsEngine::EndRender()
     {
         // �`�揈��.
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

        // ���\�[�X�o���A�̐ݒ�.
        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource = m_pRTVBuffer[m_FrameIndex].Get();
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        // ���\�[�X�o���A.
        m_pCmdList->ResourceBarrier(1, &barrier);

        // �R�}���h�̋L�^���I��.
        m_pCmdList->Close();

        // �R�}���h�����s.
        ID3D12CommandList* ppCmdLists[] = { m_pCmdList.Get() };
        m_pQueue->ExecuteCommandLists(1, ppCmdLists);

        // ��ʂɕ\��.
        Present(1);
    }

    void EUGGraphicsEngine::WaitGpu()
    {
        assert(m_pQueue != nullptr);
        assert(m_pFence != nullptr);
        assert(m_FenceEvent != nullptr);

        // �V�O�i������.
        m_pQueue->Signal(m_pFence.Get(), m_FenceCounter[m_FrameIndex]);

        // �������ɃC�x���g��ݒ肷��..
        m_pFence->SetEventOnCompletion(m_FenceCounter[m_FrameIndex], m_FenceEvent);

        // �ҋ@����.
        WaitForSingleObjectEx(m_FenceEvent, INFINITE, FALSE);

        // �J�E���^�[�𑝂₷.
        m_FenceCounter[m_FrameIndex]++;
    }
}