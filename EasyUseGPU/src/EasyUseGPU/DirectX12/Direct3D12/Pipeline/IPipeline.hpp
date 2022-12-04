#pragma once

namespace eug
{
	class IPipeline
	{
	public:

		virtual bool Init() = 0;

	private:

		virtual bool CreatePipeline() = 0;

	};

}