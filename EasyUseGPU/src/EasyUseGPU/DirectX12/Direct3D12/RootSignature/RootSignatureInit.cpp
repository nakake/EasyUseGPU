#include "D3D12RootSignature.hpp"

namespace eug
{
	bool EUGRootSignature::Init(ROOTSIGNATURE_STATUS status)
	{
		switch (status)
		{
		case Triangle:
		case ComputeShader:
			if(CreateComputeRootSignature())return true;
		}
		return false;
	}

	bool EUGRootSignature::CreateComputeRootSignature()
	{
		return true;
	}
}