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
    };
}
