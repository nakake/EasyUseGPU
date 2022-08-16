#pragma once
#include <EasyUseGPU/Windows/Windows.hpp>

#include <string>

namespace eug {
	class Window final
	{
	public:

		Window(
			uint32_t x = CW_USEDEFAULT,
			uint32_t y = CW_USEDEFAULT,
			uint32_t Width = 1024,
			uint32_t Height = 720
		)
			:m_x(x), m_y(y), m_Width(Width), m_Height(Height)
		{}

		~Window();

		void init();
		void update();
		void show();

		void* getHandle() const
		{
			return m_hWnd;
		}

		uint32_t GetHeight() const
		{
			return m_Height;
		}

		uint32_t GetWidth() const
		{
			return m_Width;
		}

		HWND GetWindow() const
		{
			return m_hWnd;
		}

	private:
		
		HWND m_hWnd = nullptr;
		HINSTANCE m_hInstance = nullptr;
		std::wstring m_windowClassName;
		
		uint32_t m_x;
		uint32_t m_y;
		uint32_t m_Width;
		uint32_t m_Height;

	};
}


