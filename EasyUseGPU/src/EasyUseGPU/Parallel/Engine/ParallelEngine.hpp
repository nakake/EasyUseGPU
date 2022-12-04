#pragma once

#include <EasyUseGPU/DirectX12/Direct3D12/Device/D3D12Device.hpp>
#include <EasyUseGPU/DirectX12/Direct3D12/CmdList/D3D12CmdList.hpp>

#include <EasyUseGPU/Resource/ResourceManager/ResourceManager.hpp>

#include <memory>
#include <string>
#include <algorithm>
#include <vector>


namespace eug
{	

	struct EUG_RAPALLEL_THREAD_NUMS
	{
		UINT x = 1;
		UINT y = 1;
		UINT z = 1;
	};

	struct EUG_PARALLEL_STATE
	{
		EUG_RAPALLEL_THREAD_NUMS TheadNums;
		EUG_RAPALLEL_THREAD_NUMS TheadGroupNums;
		//vector<D3D_SHADER_MACRO> pDefines;
		std::wstring FilePath;
		std::string  EntryPoint;
		std::string  VersionHLSL = "cs_5_1";
	};

	using std::vector;

	class EUGParallelEngine
	{	
	public:

		EUGParallelEngine(ID3D12Device* pDevice):
			m_pDevice(pDevice)
		{
		}

		~EUGParallelEngine(){}
		
		void Init();

		void Run();

		void Test();

	private:

		ID3D12Device* m_pDevice = nullptr;
		ComPtr<ID3D12GraphicsCommandList> m_pCmdList = nullptr;
		ComPtr<ID3D12CommandAllocator> m_pAllocator = nullptr;
		ComPtr<ID3D12CommandQueue> m_pQueue = nullptr;
		ComPtr<ID3D12DescriptorHeap> m_pResourceHeap = nullptr;
		ComPtr<ID3D12Fence> m_pFence = nullptr;

		ComPtr<ID3D12RootSignature>	m_pComputeRootSignature = nullptr;
		ComPtr<ID3D12PipelineState> m_pComputePipeline = nullptr;
		

		ComPtr<ID3D12Resource> m_pOutputBuffer = nullptr;
		ComPtr<ID3D12Resource> m_pReadBuffer = nullptr;
		vector<ComPtr<ID3D12Resource>> m_pInputBuffers;
		vector<ComPtr<ID3D12Resource>> m_pUploadBuffers;

		vector<float>	uavData;
		vector<vector<float>>	inputData;

	private:

		bool CreateList();
		bool CreateDescriptor();
		bool CreateQueue();
		bool CreateResource(const vector<int>& pOutputData);
		bool CreateRootSignature(uint32_t SrvNums);
		bool CreatePipeline(const EUG_PARALLEL_STATE* pParaState);

		bool SetResource(uint32_t SrvNums, vector<vector<int>>& inputData);
		//bool SetResource(uint32_t SrvNums, vector<vector<float>>& inputData);
		bool GetGPUResource();
		

		void CommandExecute();
		void Wait();

		bool InitPara();

		bool ScrutinyState(const EUG_PARALLEL_STATE* pParaState);
	

	//templateégópä÷êî
	public:

		void MulMatrix(vector<int>& Data1, vector<int>& Data2, int ResultMatrixSize, 
			const EUG_PARALLEL_STATE* pParaState = nullptr);
		void MulMatrix(vector<float>& Data1, vector<float>& Data2, int ResultMatrixSize,
			const EUG_PARALLEL_STATE* pParaState = nullptr);

	private:
		template<typename T>
		void ReadGPUData(const ComPtr<ID3D12Resource>& pReadResource, vector<T>& ReadData)
		{
			T* mapData = nullptr;
			pReadResource->Map(0, nullptr, reinterpret_cast<void**>(&mapData));
			std::copy_n(mapData, ReadData.size(), ReadData.data());
			pReadResource->Unmap(0, nullptr);
		}

	};
}


