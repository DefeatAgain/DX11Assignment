#include "D3DMain.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevinstance, LPSTR cmd, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hprevinstance);
	UNREFERENCED_PARAMETER(cmd);

	std::unique_ptr<D3DInitial> demo = std::make_unique<D3DMain>();
	
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"DX11WindowClass";

	if (!RegisterClassEx(&wndClass))
	{
		MessageBox(0, L"Class register fail! ", 0, 0);
		return -1;
	}

	RECT rc = { 0,0,800,600 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindow(wndClass.lpszClassName, L"Win32 Window",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		rc.right - rc.left, rc.bottom - rc.top,
		NULL, NULL, hInstance, NULL);

	// 隐藏鼠标
	ShowCursor(FALSE);

	if (!hwnd)
	{
		MessageBox(0, L"Window Create Fail! ", 0, 0);
		return -1;
	}
	ShowWindow(hwnd, nCmdShow);


	//init directx
	bool result = demo->Initialize(hInstance, hwnd);
	if (!result)
	{
		MessageBox(0, L"D3DInitial init Fail! ", 0, 0);
		return -1;
	}

	// 计算帧与帧的时间差
	float deltaTime = 0.0f; // 当前帧与上一帧的时间差
	INT64 lastFrame = 0; // 上一帧
	INT64 countsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	float secondsPerCount = 1.0f / (float)countsPerSec;

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Update and Draw

		__int64 currentFrame;
		QueryPerformanceCounter((LARGE_INTEGER*)&currentFrame);
		deltaTime = (currentFrame - lastFrame) * secondsPerCount;
		lastFrame = currentFrame;
		demo->Update(deltaTime);
		demo->Render();
	}

	//demo shutdown
	demo->Shutdown();

	return static_cast<int>(msg.wParam);
}