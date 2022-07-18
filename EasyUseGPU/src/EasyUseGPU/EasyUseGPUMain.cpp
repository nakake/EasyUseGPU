#include <tchar.h>
#include <EasyUseGPU/Windows/Windows.hpp>
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
	using namespace eug;
	EngineCreate();
	Main();
 	return 0;
}
