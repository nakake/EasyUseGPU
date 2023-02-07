#pragma once

#include <EasyUseGPU/DirectX12/Direct3D12/Device/D3D12Device.hpp>
#include <EasyUseGPU/DirectX12/Direct3D12/CmdQueue/D3D12CmdQueue.hpp>
#include <EasyUseGPU/DirectX12/Direct3D12/CmdList/D3D12CmdList.hpp>
#include <EasyUseGPU/DirectX12/Direct3D12/RenderTarget/D3D12RenderTargetView.hpp>
#include <EasyUseGPU/DirectX12/Direct3D12/Fence/D3D12Fence.hpp>

#include <EasyUseGPU/DirectX12/DXGI/Factory/DXGIFactorys.hpp>
#include <EasyUseGPU/DirectX12/DXGI/SwapChain/DXGISwapChain.hpp>

#include <EasyUseGPU/Resource/ResourceManager/ResourceManager.hpp>

#include <memory>
#include <vector>

namespace eug
{
	class EUGD3D12Device;
	class EUGD3D12CmdQueue;
	class EUGDXGIFactory;
	class EUGDXGISwapChain;
	class EUGD3D12CmdList;
	class EUGD3D12RenderTargetView;
	class EUGD3D12Fence;

	struct alignas(256) Transform // ���ǉ�.
	{
		DirectX::XMMATRIX   World;      // ���[���h�s��ł�.
		DirectX::XMMATRIX   View;       // �r���[�s��ł�.
		DirectX::XMMATRIX   Proj;       // �ˉe�s��ł�.
	};

	///////////////////////////////////////////////////////////////////////////////
	// ConstantBufferView structure
	///////////////////////////////////////////////////////////////////////////////
	template<typename T>
	struct ConstantBufferView // ���ǉ�.
	{
		D3D12_CONSTANT_BUFFER_VIEW_DESC Desc;               // �萔�o�b�t�@�̍\���ݒ�ł�.
		D3D12_CPU_DESCRIPTOR_HANDLE     HandleCPU;          // CPU�f�B�X�N���v�^�n���h���ł�.
		D3D12_GPU_DESCRIPTOR_HANDLE     HandleGPU;          // GPU�f�B�X�N���v�^�n���h���ł�.
		T* pBuffer;            // �o�b�t�@�擪�ւ̃|�C���^�ł�.
	};

	struct Vertex
	{
		DirectX::XMFLOAT3   Position;    // �ʒu���W�ł�.
		DirectX::XMFLOAT4   Color;       // ���_�J���[�ł�.
	};

	class EUGGraphicsEngine
	{
	public:

		EUGGraphicsEngine(uint32_t WndWidth, uint32_t WndHeight, HWND Window, 
			const ComPtr<ID3D12Device> pDevice, uint32_t FrameCount = 2)
			:m_WndWidth(WndWidth), m_WndHeight(WndHeight), m_Window(Window), m_pDevice(pDevice) 
		{
			m_FrameIndex = 0;
		}
		
		~EUGGraphicsEngine()
		{
		}

		bool Init()
		{
			InitObjects();
			OnInit();
			return true;
		}

		void StartRender();

		void EndRender();
	private:
		static const uint32_t m_FrameCount = 2;
		//uint32_t m_FrameIndex;

		uint32_t m_WndWidth = 0;
		uint32_t m_WndHeight = 0;
		HWND m_Window = nullptr;

		ComPtr<ID3D12Device> m_pDevice = nullptr;
                      
		ComPtr<ID3D12CommandQueue>          m_pQueue = nullptr;                       
		ComPtr<IDXGISwapChain3>             m_pSwapChain = nullptr;
		ComPtr<ID3D12Resource>              m_pRTVBuffer[m_FrameCount] = {};
		ComPtr<ID3D12CommandAllocator>      m_pCmdAllocator[m_FrameCount] = {};
		ComPtr<ID3D12GraphicsCommandList>   m_pCmdList = nullptr;
		ComPtr<ID3D12DescriptorHeap>        m_pHeapRTV = nullptr;
		ComPtr<ID3D12Fence>                 m_pFence = nullptr;

		ComPtr<ID3D12DescriptorHeap>           m_pHeapCBV = nullptr;                     // ���ǉ��F�f�B�X�N���v�^�q�[�v�ł�(�萔�o�b�t�@�r���[�E�V�F�[�_���\�[�X�r���[�E�A���I�[�_�[�h�A�N�Z�X�r���[)
		ComPtr<ID3D12Resource>                 m_pVB = nullptr;                          // ���ǉ��F���_�o�b�t�@�ł�.
		ComPtr<ID3D12Resource>                 m_pCB[m_FrameCount] = {};              // ���ǉ��F�萔�o�b�t�@�ł�.
		ComPtr<ID3D12RootSignature>            m_pRootSignature = nullptr;               // ���ǉ��F���[�g�V�O�j�`���ł�.
		ComPtr<ID3D12PipelineState>            m_pPSO = nullptr;                         // ���ǉ��F�p�C�v���C���X�e�[�g�ł�.

		HANDLE                              m_FenceEvent = nullptr;
		uint64_t                            m_FenceCounter[m_FrameCount] = {};
		uint32_t                            m_FrameIndex = 0;                   
		D3D12_CPU_DESCRIPTOR_HANDLE         m_HandleRTV[m_FrameCount] = {};
		D3D12_VERTEX_BUFFER_VIEW        m_VBV = {};                          // ���ǉ��F���_�o�b�t�@�r���[�ł�.
		D3D12_VIEWPORT                  m_Viewport = {};                     // ���ǉ��F�r���[�|�[�g�ł�.
		D3D12_RECT                      m_Scissor = {};                      // ���ǉ��F�V�U�[��`�ł�.
		ConstantBufferView<Transform>   m_CBV[m_FrameCount] = {};              // ���ǉ��F�萔�o�b�t�@�r���[�ł�.
		float                           m_RotateAngle = 0;                  // ���ǉ��F��]�p�ł�.

		bool InitObjects();

		void Present(uint32_t interval);

		//void WaitDiscard();
		void WaitGpu();

		bool OnInit();
		
	};

}