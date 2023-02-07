#pragma once

#include <EasyUseGPU/Engine/EasyUseGPUEngine.hpp>

namespace eug {
	namespace System 
	{
		bool Run();
		bool Updata();
	}
	namespace Parallel
	{

		bool SetParaState(EUG_PARALLEL_STATE* pParaState);

		bool ExecutionGPU(uint32_t x, uint32_t y, uint32_t z);
	}

	namespace Resource
	{
		template<typename T>
		bool SetUploadBufferData(const vector<T>& pUploadData)
		{
			if (typeid(T) != typeid(int) && typeid(T) != typeid(float)) return false;
			return engine->EUGSetUploadBufferData(pUploadData);
		}

		template<typename T>
		bool SetOutputBufferData(const vector<T>& pOutputData)
		{
			if (typeid(T) != typeid(int) && typeid(T) != typeid(float)) return false;
			return engine->EUGSetOutputBufferData(pOutputData);
		}

		template<typename T>
		bool GetBufferDataFromGPU(vector<T>& pOutputData, uint32_t DataIndex)
		{
			if (typeid(T) != typeid(int) && typeid(T) != typeid(float)) return false;
			return engine->EUGGetBufferDataFromGPU(pOutputData, DataIndex);
		}
	}
}
