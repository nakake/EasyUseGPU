#include "WindowOptions.hpp"
#include <EasyUseGPU/Windows/Windows.hpp>

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