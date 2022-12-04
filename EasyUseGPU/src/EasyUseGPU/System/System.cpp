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
}