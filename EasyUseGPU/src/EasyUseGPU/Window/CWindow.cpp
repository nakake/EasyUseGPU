#include "CWindow.hpp"
#include "WindowProc.hpp"

namespace eug 
{
	void RegisterWindowClass(HINSTANCE hInstance, const wchar_t* className) 
	{
		WNDCLASSEX wc { };
		wc.cbSize			= sizeof(WNDCLASSEX);
		wc.style			= CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc		= WindowProc;
		wc.hInstance		= hInstance;
		wc.hIcon			= LoadIconW(hInstance, IDI_APPLICATION);
		wc.hCursor			= nullptr;
		wc.hbrBackground	= GetSysColorBrush(COLOR_BACKGROUND);
		wc.lpszClassName	= className;

		if (!RegisterClassEx(&wc)) 
		{
			return;
		}
	}

	CWindow::CWindow() {}
	
	CWindow::~CWindow() 
	{
		if (m_hInstance != nullptr) 
		{
			UnregisterClass(m_windowClassName.c_str(), m_hInstance);
		}
	
		if (m_hWnd != nullptr) 
		{
			DestroyWindow(m_hWnd);
		}

		m_hInstance = nullptr;
		m_hWnd = nullptr;
	}
	
	void CWindow::init() 
	{
		
		m_hInstance = GetModuleHandleW(nullptr);
		
		if (m_hInstance == nullptr) 
		{
			return;
		}

		m_windowClassName = L"MainWindow";

		RegisterWindowClass(m_hInstance, m_windowClassName.c_str());

		auto style = WS_OVERLAPPEDWINDOW;

		m_hWnd = CreateWindowExW(
			0,
			m_windowClassName.c_str(),
			m_windowClassName.c_str(),
			style,
			CW_USEDEFAULT, CW_USEDEFAULT, 
			CW_USEDEFAULT, CW_USEDEFAULT,
			nullptr,
			nullptr,
			m_hInstance,
			nullptr
		);

		if (!m_hWnd) 
		{
			return;
		}
		update();
	}

	void CWindow::update() 
	{
		show();
	}

	void CWindow::show() 
	{
		ShowWindow(m_hWnd, SW_SHOWNORMAL);
		UpdateWindow(m_hWnd);
		SetFocus(m_hWnd);
	}

	void* CWindow::getHandle() 
	{
		return m_hWnd;
	}
}

