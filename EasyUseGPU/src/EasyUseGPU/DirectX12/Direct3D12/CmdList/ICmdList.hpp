#pragma once

#include <EasyUseGPU/Windows/Windows.hpp>
#include <EasyUseGPU/Direct3D12/Direct3D12.hpp>

class IEUGCmdList
{
public:

	virtual ~IEUGCmdList() = 0{}
	virtual bool Init() = 0;

private:
	
	virtual bool CreateCmdList() = 0;
	virtual bool CreateCmdAllocator() = 0;

};
