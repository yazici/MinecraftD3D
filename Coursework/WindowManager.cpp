#include "WindowManager.h"

WindowManager::WindowManager()
{
}

WindowManager::WindowManager(const WindowManager& other)
{
}

WindowManager::~WindowManager()
{
}

bool WindowManager::Initialise()
{
	// Initialise the width and height of the screen to zero before sending the variables into the function
	InitialiseWindows(WindowResolution_.width, WindowResolution_.height);

	// Initialise application
	ApplicationManager_ = new ApplicationManager;
	if (!ApplicationManager_)
		return false;

	// Initialise the ApplicationManager
	Result_ = ApplicationManager_->Initialise(HWND_, WindowResolution_);
	if(!Result_)
	{
		return false;
	}

	return true;
}

void WindowManager::InitialiseWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	D3DXVECTOR2 screenPosition;

	// Get the instance of this ApplicationManager.
	HInstance_ = GetModuleHandle(NULL);

	// Give the ApplicationManager a name.
	ApplicationManagerName_ = L"Programming Games in DirectXManager";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = HInstance_;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = ApplicationManagerName_;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (Config::Global::FullScreen)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		screenPosition.x = screenPosition.y = 0;
	}
	else
	{
		// Set to default window resolution
		screenWidth = Config::Global::ScreenWidth;
		screenHeight = Config::Global::ScreenHeight;

		// Place the window in the middle of the active window space (top of screen to top of taskbar)
		screenPosition.x = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		screenPosition.y = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

		// Get size of the task bar
		RECT rect;
		HWND taskBar = FindWindow(L"Shell_traywnd", NULL);
		if (taskBar && GetWindowRect(taskBar, &rect))
		{
			// Move the window up
			float taskbarHeight = (rect.bottom - rect.top);
			screenPosition.y -= (taskbarHeight / 2);
		}
	}

	// Create the window with the screen settings and get the handle to it.
	HWND_ = CreateWindowEx(WS_EX_APPWINDOW, ApplicationManagerName_, ApplicationManagerName_,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		screenPosition.x, screenPosition.y, screenWidth, screenHeight,
		NULL, NULL, HInstance_, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(HWND_, SW_SHOW);
	SetForegroundWindow(HWND_);
	SetFocus(HWND_);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}

void WindowManager::Shutdown()
{
	// Shutdown scenes
	ApplicationManager::Instance()->Shutdown();

	//=====================
	// Shutdown the Window
	//=====================

	// Show the mouse cursor
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode
	if (Config::Global::FullScreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window
	DestroyWindow(HWND_);
	HWND_ = NULL;

	// Remove the ApplicationManager instance
	UnregisterClass(ApplicationManagerName_, HInstance_);
	HInstance_ = NULL;

	return;
	
	return;
}

void WindowManager::Run()
{
	MSG msg;
	bool Running = true;

	// Initialise the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	
	// Keep running until an escape is processed
	while(Running)
	{
		// Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the ApplicationManager then exit out.
		if(msg.message == WM_QUIT)
		{
			Running = false;
		}
		else
		{
			// Do the frame processing for the ApplicationManager object.
			Result_ = ApplicationManager_->Frame();
			if (!Result_)
			{
				Running = false;
			}
		}
	}

	return;
}

LRESULT CALLBACK WindowManager::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch(umsg)
	{
		// Any other messages send to the default message handler as our ApplicationManager won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}

	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

Rect2D WindowManager::GetWindowResolution()
{
	return WindowResolution_;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	if(umessage == WM_DESTROY || umessage == WM_CLOSE)
	{
		PostQuitMessage(0);
		return 0;
	}
	else
	{
		return WindowManager::Instance() -> MessageHandler(hwnd, umessage, wparam, lparam);
	}
}