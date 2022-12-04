#pragma once
#include "EasyUseGPUEngine.hpp"

namespace eug
{ 
	void EngineCreate() 
	{
		engine.reset(new EasyUseGPUEngine);
		//engine->Init();
	}
}