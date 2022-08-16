#include <tchar.h>
#include <EasyUseGPU/Windows/Windows.hpp>
#include <EasyUseGPU/Direct3D12/Direct3D12.hpp>
#include <EasyUseGPU/Engine/EasyUseGPUEngine.hpp>
#include <EasyUseGPU/Engine/EngineFactory.hpp>

void Main();

namespace eug 
{
}

//エントリーポイントの引数は使わない(あとから取得する)ので未定義
//詳しくは「https://blog.misw.jp/entry/2018/07/27/043402」など参照

int WINAPI _tWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPTSTR, _In_ int) 
{

#if defined(DEBUG) || defined(_DEBUG)
	{
		ComPtr<ID3D12Debug> debug;

		HRESULT hr = D3D12GetDebugInterface(IID_PPV_ARGS(debug.GetAddressOf()));

		if (SUCCEEDED(hr)) debug->EnableDebugLayer();
	}
#endif

	using namespace eug;

	EngineCreate();
	Main();
 	return 0;
}
