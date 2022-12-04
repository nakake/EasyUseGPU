#pragma once

#include <EasyUseGPU/Direct3D12/Direct3D12.hpp>
#include <EasyUseGPU/Windows/Windows.hpp>

#include <memory>

namespace eug
{

	class EUGSamplePolygon
	{
	public:
		EUGSamplePolygon();
		~EUGSamplePolygon();

	private:
		std::unique_ptr<ID3D12Resource> m_pVB;
		
	};
}

