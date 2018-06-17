#include <windows.h>

//单词类声明. 
namespace WordsChecking
{
    class Word
    {
        public:
        char Word[35];
        char Meaning[35];
        char Part_of_speech[10];
    };
    
    class WriteWindowData
    {
        public:
        HWND Word;
        const int WordId = 2001;
        HWND Meaning;
        const int MeaningId = 2002;
        HWND Part_of_speech; 
        const int Part_of_speechId = 2003;
        HWND WordIn;
        const int WordInId = 1;
        HWND MeaningIn;
        const int MeaningInId = 2;
        HWND SpeechIn;
        const int SpeechInId = 3;
        HWND OK;
        const int OKId = 1111;
        void DestoryWindows(void)
        {
            DestroyWindow(Meaning);
            DestroyWindow(MeaningIn);
            DestroyWindow(OK);
            DestroyWindow(Part_of_speech);
            DestroyWindow(SpeechIn);
            DestroyWindow(Word);
            DestroyWindow(WordIn);
        }
    };
    
    class WorkWindowData
    {
        public:
        HWND Window = 0;
        unsigned int WindowHeight = 0;
        unsigned int WindowWidth = 0;
        char Caption[15] = "单词记忆";
        
        HWND WordText = 0;
        unsigned int WordTextPosx = 0;
        unsigned int WordTextPosy = 0;
        
        HWND BlockOne_WriteFile;
        const int BlockOneId = 1001; 
        HWND BlockTwo_CheckWords;
        const int BlockTwoId = 1002;
        
        WriteWindowData WriteWindow;
        TEXTMETRIC TextData; 
        void MainMenu(void)
        {
            HDC hdc;
            hdc = GetDC(Window);
            GetTextMetrics(hdc,&(TextData));
            ReleaseDC(Window,hdc);
            BlockOne_WriteFile = CreateWindowEx(0,"STATIC",TEXT("写入文件"),SS_NOTIFY|WS_VISIBLE|WS_CHILD|SS_CENTER|SS_CENTERIMAGE,0,0,WindowWidth,WindowHeight/2,Window,(HMENU)BlockOneId,0,0);
            BlockTwo_CheckWords = CreateWindowEx(0,"STATIC",TEXT("单词记忆"),SS_NOTIFY|WS_VISIBLE|WS_CHILD|SS_CENTER|SS_CENTERIMAGE,0,1*(WindowHeight/2),WindowWidth,WindowHeight/2,Window,(HMENU)BlockTwoId,0,0);     
        }
    };
}
