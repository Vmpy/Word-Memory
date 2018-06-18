#include <windows.h>

//单词类声明. 
namespace WordsChecking
{
    class Word
    {
        public:
        char Word[35] = {'\0'};
        char Meaning[35] = {'\0'};
        char Part_of_speech[10] = {'\0'};
    };
    
    class WriteWindowData
    {
        public:
        HWND Word = 0;
        const int WordId = 2001;
        HWND Meaning = 0;
        const int MeaningId = 2002;
        HWND Part_of_speech = 0; 
        const int Part_of_speechId = 2003;
        HWND WordIn = 0;
        const int WordInId = 1;
        HWND MeaningIn = 0;
        const int MeaningInId = 2;
        HWND SpeechIn = 0;
        const int SpeechInId = 3;
        HWND OK = 0;
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
    
    class CheckWordsData
    {
        public:
        int MaxNum = 0;
        Word* WordTable = 0;
        bool* Tested = 0;
        int Displayed[4] = {-1,-1,-1,-1};
        //Group Button
        HWND Choice;
        const int ChoiceId = 3000;
        //Radio Buttons
        HWND ChoiceA;
        const int ChoiceAId = 3001;
        HWND ChoiceB;
        const int ChoiceBId = 3002;
        HWND ChoiceC;
        const int ChoiceCId = 3003;
        HWND ChoiceD;
        const int ChoiceDId = 3004;
        
        HWND ChoiceOK;
        const int ChoiceOKId = 3005;
        //Static 
        HWND Word;
        const int WordId = 3005;
        
        int NowIndex = 0; 
        
        int GetWordNum(const char* FileName)
        {
            int Num = 0;
            std::ifstream File;
            File.open(FileName,std::ios_base::binary);
            while(true)
            {
                WordsChecking::Word User;
                File.read((char*)&User,sizeof(WordsChecking::Word));
                if(File.eof())
                {
                    break;
                }
                Num++;
            }
            File.close();
            return Num;
        }
        
        void DestroyWindows(void)
        {
            DestroyWindow(Choice);
            DestroyWindow(ChoiceA);
            DestroyWindow(ChoiceB);
            DestroyWindow(ChoiceC);
            DestroyWindow(ChoiceD);
            DestroyWindow(ChoiceOK);
            DestroyWindow(Word);
        }
        
        bool IsDone(void)
        {
            int Count = 0;
            for(int i = 0;i < MaxNum;i++)
            {
                if(Tested[i] != true)
                {
                    return false;
                }
            }
            return true;
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
        CheckWordsData CheckWindow;
        
        TEXTMETRIC TextData;
        //这里的小字体=大字体长宽/2，所以在代码中会见到 TextData.tmHeight/2 等； 
        HFONT hFont = CreateFont(44,16,0,0,400,FALSE, FALSE, FALSE,DEFAULT_CHARSET,OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,FF_DONTCARE, TEXT("微软雅黑"));
        HFONT ShFont = CreateFont(22,8,0,0,400,FALSE, FALSE, FALSE,DEFAULT_CHARSET,OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,FF_DONTCARE, TEXT("微软雅黑"));
        void MainMenu(void)
        {
            if(!BlockOne_WriteFile && !BlockTwo_CheckWords)
            {
                HDC hdc;
                hdc = GetDC(Window);
                GetTextMetrics(hdc,&(TextData));
                TextData.tmHeight = 44;
                TextData.tmAveCharWidth = 16;
                ReleaseDC(Window,hdc);
                
                BlockOne_WriteFile = CreateWindowEx(0,"STATIC",TEXT("写入文件"),SS_NOTIFY|WS_VISIBLE|WS_CHILD|SS_CENTER|SS_CENTERIMAGE,0,0,WindowWidth,WindowHeight/2,Window,(HMENU)BlockOneId,0,0);
                BlockTwo_CheckWords = CreateWindowEx(0,"STATIC",TEXT("单词记忆"),SS_NOTIFY|WS_VISIBLE|WS_CHILD|SS_CENTER|SS_CENTERIMAGE,0,1*(WindowHeight/2),WindowWidth,WindowHeight/2,Window,(HMENU)BlockTwoId,0,0);
                SendMessage(BlockOne_WriteFile,WM_SETFONT,(WPARAM)hFont,0);
                SendMessage(BlockTwo_CheckWords,WM_SETFONT,(WPARAM)hFont,0);     
            }
        }
    };
}
