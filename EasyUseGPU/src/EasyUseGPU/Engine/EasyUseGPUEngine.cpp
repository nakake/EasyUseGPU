#include "EasyUseGPUEngine.hpp"

namespace eug {
	EasyUseGPUEngine::EasyUseGPUEngine()
	{
		m_Window.reset(new CWindow);
		m_Window->init();
	}

	EasyUseGPUEngine::~EasyUseGPUEngine()
	{

	}

}

