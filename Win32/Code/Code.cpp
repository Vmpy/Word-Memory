#include <windows.h>
#include "Data.h" 


WordsChecking::WorkWindowData WinData;
WordsChecking::Word WordTable;

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
 
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	WNDCLASSEX wc;
	MSG msg;

	memset(&wc,0,sizeof(wc));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WordMemory";
	wc.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL,IDI_APPLICATION);

	if(!RegisterClassEx(&wc))
    {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	WinData.Window = CreateWindowEx(WS_EX_CLIENTEDGE,"WordMemory",WinData.Caption,WS_VISIBLE|WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
		CW_USEDEFAULT,
		640,
		480,
		NULL,NULL,hInstance,NULL);

	if(WinData.Window == NULL)
    {
		MessageBox(NULL,"Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	while(GetMessage(&msg,NULL,0,0))
    {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
{
	switch(Message)
    {
        case WM_SIZE:
        {
            WinData.WindowWidth = HIWORD(lParam);
            WinData.WindowHeight = LOWORD(lParam);
            break;
        }
		
		case WM_DESTROY:
        {
			PostQuitMessage(0);
			break;
		}

		default:
			return DefWindowProc(hwnd,Message,wParam,lParam);
	}
	return 0;
}
