#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

namespace WordsChecking{ 
    class Word
    {
        public:
        char Word[50] = {'\0'};
        char Meaning[50] = {'\0'};
        char Part_of_speech[10] = {'\0'};
    };
}

WordsChecking::Word * WordTable;
int MaxNum;

int Menu(void);
int GetWordNum(const char*);
int WriteWordsFile(void);
int ReloadWordsFile(const char*,WordsChecking::Word*,int);
int PrepareTest(void);
int Test(void);

int main(void)
{
    switch(Menu())
    {
        case 1:WriteWordsFile();break;
        case 2:PrepareTest();Test();break;
        default:exit(0);
    }
    system("pause");
	return 0;
}

int WriteWordsFile(void)
{
    std::string FileName;
    std::cout << "请提供一个文件名（未使用）：" << std::endl;
    std::cin >> FileName;
    std::cin.get();
    std::ofstream File;
    File.open(FileName.c_str(),std::ios_base::app | std::ios_base::binary);
    while(true)
    {
        WordsChecking::Word TmpWord;
        std::cout << "请给一个单词（如果输入是'exit10'，则退出）：" << std::endl;
        std::cin.getline(TmpWord.Word,60);
        if(std::string(TmpWord.Word) == "exit10")
        {
            break;
        }
        std::cout << "请提供一个单词意义:" << std::endl;
        std::cin.getline(TmpWord.Meaning,60);
        std::cout << "请提供单词的词性:" << std::endl;
        std::cin.getline(TmpWord.Part_of_speech,10);
        
        File.write((char*)&TmpWord,sizeof(WordsChecking::Word));
    }
    File.close();
    return 0;
}

int ReloadWordsFile(const char* FileName,WordsChecking::Word* BeLoaded,int size)
{
    std::ifstream File;
    File.open(FileName,std::ios_base::binary);
    for(int i = 0 ;i < size;i++)
    {
        File.read((char*)(BeLoaded+i),sizeof(WordsChecking::Word));
    }
    File.close();
    return 0;
}

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

int PrepareTest(void)
{
    system("cls");
    std::string FileName;
    std::cout << "请输入单词文件名:";
    std::cout.flush();
    std::cin >> FileName;
    std::ifstream File;
    File.open(FileName.c_str(),std::ios_base::binary);
    while(!File.is_open())
    {
        std::cout << "不存在该文件，请检查输入再次输入:" << std::endl;
        std::cin >> FileName;
        File.open(FileName.c_str(),std::ios_base::binary);
    }
    
    MaxNum = GetWordNum(FileName.c_str());
    WordTable = new WordsChecking::Word[MaxNum];
    ReloadWordsFile(FileName.c_str(),WordTable,MaxNum);
    return 0;
}

int Menu(void)
{
    int Ch;
    std::cout << "*****************单词记忆程序*****************" << std::endl;
    std::cout << "               1.写入单词文件;" << std::endl;
    std::cout << "               2.选择文件测试;" << std::endl;
    std::cout << "              [Others].退出程序." << std::endl;
    std::cin >> Ch;
    return Ch;
}

int Test(void)
{
    bool Tested[MaxNum] = {0};
    int RightAnswerIndex;
    int index;
    
    while(true)
    {
        do{
            index = rand()%MaxNum;
        }while(Tested[index] == true);
        
        system("cls");
        std::cout << WordTable[index].Word << std::endl;
        RightAnswerIndex = 1+rand()%4;
        
        for(int i = 1;i < 5;i++)
        {
            if(RightAnswerIndex == i)
            {
                std::cout << i << '.' << WordTable[index].Meaning << std::endl;
                continue;
            }
            std::cout << i << '.' << WordTable[rand()%MaxNum].Meaning << std::endl;
        }
        
        int Ch;
        std::cin >> Ch;
        
        if(Ch == RightAnswerIndex)
        {
            std::cout << "恭喜，单词意义正确!" << std::endl;
            Tested[index] = true;
        }
        else
        {
            std::cout << "啊哦，单词意义错误!" << std::endl;
        }
        
        std::cin.get();
        std::cin.get();
        
        //Count
        int TrueNum = 0;
        for(int i = 0;i < MaxNum;i++)
        {
            if(Tested[i] == true)
            {
                TrueNum++;
            }
        }
        if (TrueNum == MaxNum)
        {
            break;
        }
        
    }
    
    std::cout << "单词测试完成!" << std::endl;
    delete [] WordTable;
    return 0;
}




