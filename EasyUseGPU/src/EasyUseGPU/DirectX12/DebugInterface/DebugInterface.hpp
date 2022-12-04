#pragma once

#include <EasyUseGPU/Direct3D12/Direct3D12.hpp>
#include <EasyUseGPU/Windows/Windows.hpp>

namespace eug
{
	namespace Debug
	{
		class EUGDebugInterface
		{
		public:
			EUGDebugInterface(ID3D12Device* pDevice)
				:m_pDevice(pDevice)
			{
			}

			~EUGDebugInterface()
			{
			}

			bool Init();

		private:
			ComPtr<ID3D12Debug> Debug = nullptr;
			//ComPtr<ID3D12DebugDevice> DebugInterface = nullptr;
			ID3D12DebugDevice* DebugInterface = nullptr;

			ID3D12Device* m_pDevice = nullptr;
		};

	}
}
