#pragma once
#include <EasyUseGPU/Window/IWindow.hpp>
#include <EasyUseGPU/Windows/Windows.hpp>

#include <string>

namespace eug {
	class CWindow final : public IEUGWindow
	{
	private:
		
		HWND m_hWnd = nullptr;
		HINSTANCE m_hInstance = nullptr;
		std::wstring m_windowClassName;

	public:

		CWindow();
		~CWindow();
		void init() override;
		void update() override;
		void* getHandle() override;
		void show() override;

	};
}


