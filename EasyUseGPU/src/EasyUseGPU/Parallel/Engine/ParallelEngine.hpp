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
	using std::vector;

	struct EUG_PARALLEL_STATE
	{
		vector<D3D_SHADER_MACRO> pDefines;
		std::wstring FilePath;
		std::string  EntryPoint = "main";
		std::string  VersionHLSL = "cs_5_1";
	};



	class EUGParallelEngine
	{	
	public:

		EUGParallelEngine(const ComPtr<ID3D12Device> pDevice):
			m_pDevice(pDevice)
		{
		}

		~EUGParallelEngine()
		{
		}
		
		void Init();

		bool ParaSetParaState(const EUG_PARALLEL_STATE* pParaState);

		bool ParaExecutionGPU(uint32_t x, uint32_t y, uint32_t z);

	//templateégópä÷êî
	public:

		template<typename T>
		bool ParaSetUploadBufferData(const vector<T>& pUploadData)
		{
			return CreateUploadResource((void*)pUploadData.data(),
				static_cast<uint32_t>(pUploadData.size() * sizeof(pUploadData[0])));
		}

		template<typename T>
		bool ParaSetOutputBufferData(const vector<T>& pUploadData)
		{
			return CreateReadResource(static_cast<uint32_t>(pUploadData.size() * sizeof(pUploadData[0])));
		}

		template<typename T>
		bool ParaGetBufferDataFromGPU(vector<T>& pOutputData, uint32_t DataIndex)
		{
			if (DataIndex >= m_ReadBufferNum)
			{
				return false;
			}

			GetGPUResource();

			CommandExecute();

			ReadGPUData(m_pReadBuffer[DataIndex], pOutputData);

			return true;
		}

	private:

		ComPtr<ID3D12Device> m_pDevice = nullptr;
		ComPtr<ID3D12GraphicsCommandList> m_pCmdList = nullptr;
		ComPtr<ID3D12CommandAllocator> m_pAllocator = nullptr;
		ComPtr<ID3D12CommandQueue> m_pQueue = nullptr;
		ComPtr<ID3D12DescriptorHeap> m_pResourceHeap = nullptr;
		ComPtr<ID3D12Fence> m_pFence = nullptr;

		ComPtr<ID3D12RootSignature>	m_pComputeRootSignature = nullptr;
		ComPtr<ID3D12PipelineState> m_pComputePipeline = nullptr;
		

		vector<ComPtr<ID3D12Resource>> m_pOutputBuffer;
		vector<ComPtr<ID3D12Resource>> m_pReadBuffer;
		vector<ComPtr<ID3D12Resource>> m_pInputBuffers;
		vector<ComPtr<ID3D12Resource>> m_pUploadBuffers;
		vector<ComPtr<ID3D12Resource>> m_pInput2DBuffers;
		vector<ComPtr<ID3D12Resource>> m_pUpload2DBuffers;

		uint32_t m_UploadBufferNum = 0;
		uint32_t m_Upload2DBufferNum = 0;
		uint32_t m_ReadBufferNum = 0;

		const EUG_PARALLEL_STATE* m_pParaState = nullptr;

		bool m_IsSettingParaState = false;

	private:

		bool CreateRootSignature(uint32_t SrvNums);
		bool CreatePipeline(const EUG_PARALLEL_STATE* pParaState);


		bool CreateReadResource(const uint32_t DataSize);
		bool CreateUploadResource(const void* pData, const uint32_t DataSize);

		bool CreateUpload2DResource(
			const void* pData, const uint32_t SizeX, const uint32_t SizeY, const uint32_t DataByte);

		bool SetResource();
		bool GetGPUResource();
		

		void CommandExecute();
		void Wait();

		bool InitPara();

		bool ScrutinyState(const EUG_PARALLEL_STATE* pParaState);


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


