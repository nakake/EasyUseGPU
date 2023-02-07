#include "System.hpp"
#include <EasyUseGPU/Window/WindowOptions.hpp>

namespace eug {
	namespace System {
		//ŒŸØ—p
		
		bool Run() {
			engine->Init();
			while (PumpMessages()){
				engine->StartRender();
				engine->EndRender();
			}
			return true;
		}

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
			pParaState->FilePath = L"../../EasyUseGPU/src/" + pParaState->FilePath;
			pParaState->pDefines.push_back({NULL,NULL});
			return engine->EUGSetParaState(pParaState);
		}

		bool ExecutionGPU(uint32_t x, uint32_t y, uint32_t z)
		{
			return engine->EUGExecutionGPU(1, 1, 1);
		}
	}
}