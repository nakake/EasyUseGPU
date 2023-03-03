#include "System.hpp"
#include <EasyUseGPU/Window/WindowOptions.hpp>

namespace eug {
	namespace System {
		//Windowsアプリケーション用の実行関数
		
		bool Run() {
			engine->Init();
			while (PumpMessages()){
				engine->StartRender();
				engine->EndRender();
			}
			return true;
		}

		//Windowsアプリケーション用の更新関数
		bool Updata() {
			engine->EndRender();

			if (PumpMessages()) {
				engine->StartRender();
				return true;
			}
			return false;
		}
	}

	namespace Parallel
	{
		bool SetParaState(EUG_PARALLEL_STATE* pParaState)
		{
			if (pParaState->FilePath.empty()) return false;
			pParaState->FilePath = L"../../user/" + pParaState->FilePath;
			pParaState->pDefines.push_back({NULL,NULL});
			return engine->EUGSetParaState(pParaState);
		}

		bool ExecutionGPU(uint32_t x = 1, uint32_t y = 1, uint32_t z = 1)
		{
			if (x < 1 || y < 1 || z < 1) return false;
			return engine->EUGExecutionGPU(x, y, z);
		}
	}
}