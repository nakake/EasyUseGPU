#pragma once

#include "EasyUseGPUEngine.hpp"

namespace eug
{
	namespace state
	{
		uint32_t GetWndWidth()
		{
			return engine->GetWindow()->GetWidth();
		}

		uint32_t GetWndHeight()
		{
			return engine->GetWindow()->GetHeight();
		}

		HWND GetWindow()
		{
			return engine->GetWindow()->GetWindow();
		}
	}
}