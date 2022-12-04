#pragma once

namespace eug
{
	class IRootSignature
	{
	public:

		virtual ~IRootSignature() = 0 {}
		virtual bool Init() = 0;
	
	private:

		virtual bool CreateRootSignature() = 0;

	};

}