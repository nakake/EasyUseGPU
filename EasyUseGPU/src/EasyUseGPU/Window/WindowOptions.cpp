#include "WindowOptions.hpp"
#include <EasyUseGPU/Windows/Windows.hpp>
#include <EasyUseGPU/Engine/EasyUseGPUEngine.hpp>

void Update();
void Draw();

namespace eug
{

	bool PumpMessages()
	{
		MSG msg = {};

		while (WM_QUIT != msg.message) {
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				return true;
			}
		}
		return false;
	}
}