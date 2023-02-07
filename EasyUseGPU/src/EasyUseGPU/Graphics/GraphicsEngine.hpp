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

	struct alignas(256) Transform // ★追加.
	{
		DirectX::XMMATRIX   World;      // ワールド行列です.
		DirectX::XMMATRIX   View;       // ビュー行列です.
		DirectX::XMMATRIX   Proj;       // 射影行列です.
	};

	///////////////////////////////////////////////////////////////////////////////
	// ConstantBufferView structure
	///////////////////////////////////////////////////////////////////////////////
	template<typename T>
	struct ConstantBufferView // ★追加.
	{
		D3D12_CONSTANT_BUFFER_VIEW_DESC Desc;               // 定数バッファの構成設定です.
		D3D12_CPU_DESCRIPTOR_HANDLE     HandleCPU;          // CPUディスクリプタハンドルです.
		D3D12_GPU_DESCRIPTOR_HANDLE     HandleGPU;          // GPUディスクリプタハンドルです.
		T* pBuffer;            // バッファ先頭へのポインタです.
	};

	struct Vertex
	{
		DirectX::XMFLOAT3   Position;    // 位置座標です.
		DirectX::XMFLOAT4   Color;       // 頂点カラーです.
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

		ComPtr<ID3D12DescriptorHeap>           m_pHeapCBV = nullptr;                     // ★追加：ディスクリプタヒープです(定数バッファビュー・シェーダリソースビュー・アンオーダードアクセスビュー)
		ComPtr<ID3D12Resource>                 m_pVB = nullptr;                          // ★追加：頂点バッファです.
		ComPtr<ID3D12Resource>                 m_pCB[m_FrameCount] = {};              // ★追加：定数バッファです.
		ComPtr<ID3D12RootSignature>            m_pRootSignature = nullptr;               // ★追加：ルートシグニチャです.
		ComPtr<ID3D12PipelineState>            m_pPSO = nullptr;                         // ★追加：パイプラインステートです.

		HANDLE                              m_FenceEvent = nullptr;
		uint64_t                            m_FenceCounter[m_FrameCount] = {};
		uint32_t                            m_FrameIndex = 0;                   
		D3D12_CPU_DESCRIPTOR_HANDLE         m_HandleRTV[m_FrameCount] = {};
		D3D12_VERTEX_BUFFER_VIEW        m_VBV = {};                          // ★追加：頂点バッファビューです.
		D3D12_VIEWPORT                  m_Viewport = {};                     // ★追加：ビューポートです.
		D3D12_RECT                      m_Scissor = {};                      // ★追加：シザー矩形です.
		ConstantBufferView<Transform>   m_CBV[m_FrameCount] = {};              // ★追加：定数バッファビューです.
		float                           m_RotateAngle = 0;                  // ★追加：回転角です.

		bool InitObjects();

		void Present(uint32_t interval);

		//void WaitDiscard();
		void WaitGpu();

		bool OnInit();
		
	};

}