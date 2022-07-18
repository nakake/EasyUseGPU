#include "System.hpp"
#include <EasyUseGPU/Window/WindowOptions.hpp>

namespace eug {
	namespace System {
		bool update() {
			//engine->GetWindow()->update();
			return PumpMessages();
		}
	}
}