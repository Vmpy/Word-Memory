#include <windows.h>
#include <windowsx.h> 
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
    static HWND BlockOne_WriteFile;
    
	switch(Message)
    {
        case WM_SIZE:
        {
            WinData.Window = hwnd;
            WinData.WindowWidth = GET_X_LPARAM(lParam);
            WinData.WindowHeight = GET_Y_LPARAM(lParam);
            
            WinData.BlockOne_WriteFile = CreateWindowEx(0,"STATIC",TEXT("写入文件"),WS_VISIBLE|WS_CHILD|SS_CENTER|SS_CENTERIMAGE,0,0,WinData.WindowWidth,WinData.WindowHeight/2,WinData.Window,(HMENU)WinData.BlockOneId,0,0);
            WinData.BlockTwo_CheckWords = CreateWindowEx(0,"STATIC",TEXT("单词记忆"),WS_VISIBLE|WS_CHILD|SS_CENTER|SS_CENTERIMAGE,0,1*(WinData.WindowHeight/2),WinData.WindowWidth,WinData.WindowHeight/2,WinData.Window,(HMENU)WinData.BlockTwoId,0,0);
            break;
        }
        
        case WM_CTLCOLORSTATIC:
        {
            HBRUSH hBrush;
            if((HWND)lParam == WinData.BlockOne_WriteFile)
            {
                hBrush = CreateSolidBrush(RGB(116,0,0));
                SetBkColor((HDC)wParam,RGB(116,0,0));
            }
            if((HWND)lParam == WinData.BlockTwo_CheckWords)
            {
                hBrush = CreateSolidBrush(RGB(199,97,20));
                SetBkColor((HDC)wParam,RGB(199,97,20));
            }
            return (INT_PTR)hBrush;
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
