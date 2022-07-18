#pragma once

namespace eug
{
	class IEUGWindow
	{
	public:
		virtual ~IEUGWindow() = default;
		virtual void init() = 0;
		virtual void update() = 0;
		virtual void* getHandle() = 0;
		virtual void show() = 0;
	};
}