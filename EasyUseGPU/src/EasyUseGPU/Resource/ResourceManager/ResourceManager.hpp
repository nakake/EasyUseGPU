#pragma once

#include <EasyUseGPU/Direct3D12/Direct3D12.hpp>
#include <EasyUseGPU/Windows/Windows.hpp>

namespace eug
{
	namespace Resource
	{
		ComPtr<ID3D12Resource> CreateUploadBuffer(
			ID3D12Device* pDevice,
			ID3D12GraphicsCommandList* pCmdList,
			const void* initData,
			uint64_t byteSize,
			ComPtr<ID3D12Resource>& pUploadBuffer
		);
	}
}