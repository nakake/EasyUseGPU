#pragma once

#include <EasyUseGPU/Direct3D12/Direct3D12.hpp>
#include <EasyUseGPU/Windows/Windows.hpp>

//#include <EasyUseGPU/DirectX12/Direct3D12/Device/D3D12Device.hpp>

#include <memory>

namespace eug
{
	using std::unique_ptr;

	enum ROOTSIGNATURE_STATUS
	{
		Triangle,
		ComputeShader
	};

	class EUGRootSignature
	{
	public:

		EUGRootSignature(){}
		~EUGRootSignature(){}

		bool Init(ROOTSIGNATURE_STATUS status);

	private:

		unique_ptr<ID3D12RootSignature> m_pRootSignature = nullptr;
		ComPtr<ID3DBlob> m_pShader = nullptr;
		ComPtr<ID3DBlob> m_pErrorMsg = nullptr;

		bool CreateComputeRootSignature();

	};
}