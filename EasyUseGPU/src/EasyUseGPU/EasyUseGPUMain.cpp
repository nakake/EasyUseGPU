#include <tchar.h>
#include <EasyUseGPU/Windows/Windows.hpp>
#include <EasyUseGPU/Engine/EasyUseGPUEngine.hpp>
#include <EasyUseGPU/Engine/EngineFactory.hpp>

void Main();

namespace eug 
{
}

//�G���g���[�|�C���g�̈����͎g��Ȃ�(���Ƃ���擾����)�̂Ŗ���`
//�ڂ����́uhttps://blog.misw.jp/entry/2018/07/27/043402�v�ȂǎQ��

int WINAPI _tWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPTSTR, _In_ int) 
{
	using namespace eug;
	EngineCreate();
	Main();
 	return 0;
}
