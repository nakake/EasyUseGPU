#include "EasyUseGPUEngine.hpp"

namespace eug {
	EasyUseGPUEngine::EasyUseGPUEngine()
	{
		m_Window.reset(new Window);
		m_Window->init();
	}

	EasyUseGPUEngine::~EasyUseGPUEngine()
	{
	}

}

