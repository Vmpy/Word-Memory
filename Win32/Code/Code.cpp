#include <windows.h>
#include <windowsx.h> 
#include <fstream>
#include <cstdio>
#include "Data.h" 


WordsChecking::WorkWindowData WinData;
WordsChecking::Word* WordTable;

bool IsMenuDisplay = true;

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

void WriteFile(void);
 
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

	WinData.Window = CreateWindowEx(WS_EX_CLIENTEDGE,"WordMemory",WinData.Caption,WS_VISIBLE|WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_MAXIMIZEBOX,
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
    static HDC hdc;
    
	switch(Message)
    {
        case WM_SIZE:
        {
            if(IsMenuDisplay)
            {
                WinData.Window = hwnd;
                WinData.WindowWidth = GET_X_LPARAM(lParam);
                WinData.WindowHeight = GET_Y_LPARAM(lParam);
                
                WinData.BlockOne_WriteFile = CreateWindowEx(0,"STATIC",TEXT("写入文件"),SS_NOTIFY|WS_VISIBLE|WS_CHILD|SS_CENTER|SS_CENTERIMAGE,0,0,WinData.WindowWidth,WinData.WindowHeight/2,WinData.Window,(HMENU)WinData.BlockOneId,0,0);
                WinData.BlockTwo_CheckWords = CreateWindowEx(0,"STATIC",TEXT("单词记忆"),SS_NOTIFY|WS_VISIBLE|WS_CHILD|SS_CENTER|SS_CENTERIMAGE,0,1*(WinData.WindowHeight/2),WinData.WindowWidth,WinData.WindowHeight/2,WinData.Window,(HMENU)WinData.BlockTwoId,0,0);
                
                hdc = GetDC(hwnd);
                GetTextMetrics(hdc,&(WinData.TextData));
                ReleaseDC(hwnd,hdc);
            }
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
        
        case WM_COMMAND:
        {
            if((HWND)lParam == WinData.BlockOne_WriteFile && HIWORD(wParam) == STN_CLICKED)
            {
                DestroyWindow(WinData.BlockOne_WriteFile);
                DestroyWindow(WinData.BlockTwo_CheckWords);
                WriteFile();
            }
            else if((HWND)lParam == WinData.BlockTwo_CheckWords && HIWORD(wParam) == STN_CLICKED)
            {
                DestroyWindow(WinData.BlockOne_WriteFile);
                DestroyWindow(WinData.BlockTwo_CheckWords);
            }
            else if((HWND)lParam == WinData.WriteWindow.OK)
            {
                std::ofstream File;
                WordsChecking::Word TmpWord;
                SendMessage(WinData.WriteWindow.Word,WM_GETTEXT,(WPARAM)35,(LPARAM)TmpWord.Word); 
                SendMessage(WinData.WriteWindow.Meaning,WM_GETTEXT,(WPARAM)35,(LPARAM)TmpWord.Meaning); 
                SendMessage(WinData.WriteWindow.Part_of_speech,WM_GETTEXT,(WPARAM)10,(LPARAM)TmpWord.Part_of_speech);
                
                SendMessage(WinData.WriteWindow.Word,WM_SETTEXT,0,0); 
                SendMessage(WinData.WriteWindow.Meaning,WM_SETTEXT,0,0); 
                SendMessage(WinData.WriteWindow.Part_of_speech,WM_SETTEXT,0,0);
                File.open("SomeTempFile.tmp",std::ios_base::app | std::ios_base::binary);
                File.write((char*)&TmpWord,sizeof(WordsChecking::Word));
                File.close();
                if(MessageBox(hwnd,"是否继续录入单词?","提示",MB_YESNO) == IDNO)
                {
                    char WorkName[100];
                    GetModuleFileName(0,WorkName,100);
                    char FileName[50] = "NoName";
                    OPENFILENAME ofn;
                    memset(&ofn,0,sizeof(ofn));
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = WinData.Window;
                    ofn.lpstrFile = FileName;
                    ofn.lpstrFile[0] = '\0';
                    ofn.nMaxFile = sizeof(FileName);
                    ofn.lpstrFilter = "WordTable Files(*.wsm)\0*.wsm\0All Files(*.*)\0*.*\0\0";
                    ofn.nFilterIndex = 1;
                    ofn.lpstrFileTitle = NULL;
                    ofn.nMaxFileTitle = 0;
                    ofn.lpstrInitialDir = WorkName;
                    ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
                    GetSaveFileName(&ofn);
                    
                    rename("SomeTempFile.tmp",FileName);
                }
            }
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

void WriteFile(void)
{

    WinData.WriteWindow.WordIn = CreateWindowEx(0,"STATIC","单词:",WS_VISIBLE|WS_CHILD|SS_LEFT,0,0,WinData.TextData.tmAveCharWidth*6,WinData.TextData.tmHeight+5,WinData.Window,(HMENU)WinData.WriteWindow.WordInId,0,0);
    WinData.WriteWindow.MeaningIn = CreateWindowEx(0,"STATIC","单词意义:",WS_VISIBLE|WS_CHILD|SS_LEFT,0,(WinData.TextData.tmHeight+5)*1,WinData.TextData.tmAveCharWidth*10,WinData.TextData.tmHeight+5,WinData.Window,(HMENU)WinData.WriteWindow.MeaningInId,0,0);
    WinData.WriteWindow.SpeechIn = CreateWindowEx(0,"STATIC","单词词性:",WS_VISIBLE|WS_CHILD|SS_LEFT,0,(WinData.TextData.tmHeight+5)*2,WinData.TextData.tmAveCharWidth*10,WinData.TextData.tmHeight+5,WinData.Window,(HMENU)WinData.WriteWindow.SpeechInId,0,0);
    IsMenuDisplay = false;
    WinData.WriteWindow.Word = CreateWindowEx(0,"EDIT","",WS_VISIBLE|WS_CHILD|WS_BORDER|ES_MULTILINE|ES_LEFT,WinData.TextData.tmAveCharWidth*6,0,WinData.TextData.tmAveCharWidth*20,WinData.TextData.tmHeight+5,WinData.Window,(HMENU)WinData.WriteWindow.WordId,0,0);
    WinData.WriteWindow.Meaning = CreateWindowEx(0,"EDIT","",WS_VISIBLE|WS_CHILD|WS_BORDER|ES_MULTILINE|ES_LEFT,WinData.TextData.tmAveCharWidth*10,(WinData.TextData.tmHeight+5)*1,WinData.TextData.tmAveCharWidth*20,WinData.TextData.tmHeight+5,WinData.Window,(HMENU)WinData.WriteWindow.MeaningId,0,0);
    WinData.WriteWindow.Part_of_speech = CreateWindowEx(0,"EDIT","",WS_VISIBLE|WS_CHILD|WS_BORDER|ES_MULTILINE|ES_LEFT,WinData.TextData.tmAveCharWidth*10,(WinData.TextData.tmHeight+5)*2,WinData.TextData.tmAveCharWidth*20,WinData.TextData.tmHeight+5,WinData.Window,(HMENU)WinData.WriteWindow.Part_of_speechId,0,0);
    WinData.WriteWindow.OK = CreateWindowEx(0,"BUTTON","录入",WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON,50,(WinData.TextData.tmHeight+5)*3,WinData.TextData.tmAveCharWidth*7,WinData.TextData.tmHeight+8,WinData.Window,(HMENU)WinData.WriteWindow.OKId,0,0);
    return;
}
