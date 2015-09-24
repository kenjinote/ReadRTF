#include <windows.h>
#include <richedit.h>

TCHAR szClassName[] = TEXT("Window");

DWORD CALLBACK ReadProc(DWORD dwCookie, LPBYTE pbBuf, LONG cb, LONG *pcb)
{
	ReadFile((HANDLE)dwCookie, pbBuf, cb, (LPDWORD)pcb, NULL);
	return 0;
}

BOOL ReadRTF(HWND hEdit, LPCTSTR lpszFilePath)
{
	const HANDLE hFile = CreateFile(lpszFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	EDITSTREAM eds;
	eds.dwCookie = (DWORD)hFile;
	eds.dwError = 0;
	eds.pfnCallback = ReadProc;
	SendMessage(hEdit, EM_STREAMIN, SF_RTF, (LPARAM)&eds); //SendMessage(hEdit, EM_STREAMIN, SF_TEXT, (LPARAM)&eds);
	CloseHandle(hFile);
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE hRtLib;
	static HWND hEdit;
	switch (msg)
	{
	case WM_CREATE:
		hRtLib = LoadLibrary(TEXT("RICHED32"));
		hEdit = CreateWindow(TEXT("RICHEDIT"), 0, WS_CHILD | WS_VISIBLE, 120, 10, 480, 28, hWnd, NULL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		ReadRTF(hEdit, TEXT("sample.rtf"));
		break;
	case WM_SIZE:
		MoveWindow(hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), 1);
		break;
	case WM_SETFOCUS:
		SetFocus(hEdit);
		break;
	case WM_DESTROY:
		DestroyWindow(hEdit);
		FreeLibrary(hRtLib);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("RTF Çì«Ç›çûÇﬁ"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
		);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
