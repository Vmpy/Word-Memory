#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

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
}

//单词表指针，用于单词测试. 
WordsChecking::Word * WordTable;
int MaxNum;

int Menu(void);
int GetWordNum(const char*);
int WriteWordsFile(void);
int ReloadWordsFile(const char*,WordsChecking::Word*,int);
int PrepareTest(bool,const char*);
int Test(void);

int main(int argc,char** argv)
{
    if(argc == 2)
    {
        PrepareTest(true,*(argv+1));
        Test();
    }
    else
    {
        while(1)
        {
            switch(Menu())
            {
                case 1:WriteWordsFile();break;
                case 2:PrepareTest(false,0);Test();break;
                default:exit(0);
            }
        }
    }
	return 0;
}

int WriteWordsFile(void)
{
    int Count = 4;
    std::string FileName;
    std::cout << "请提供一个文件名（未使用）：" << std::endl;
    std::cin >> FileName;
    std::cin.get();
    std::ofstream File;
    File.open(FileName.c_str(),std::ios_base::app | std::ios_base::binary);
    std::cout << "请保证储存单词数目大于4." << std::endl;
    while(true)
    {
        WordsChecking::Word TmpWord;
        std::cout << "请给一个单词（如果输入是'ExIt'，则退出）：" << std::endl;
        std::cin.getline(TmpWord.Word,60);
        
        if(std::string(TmpWord.Word) == "ExIt" && Count < 0)
        {
            break;
        }
        
        if(std::string(TmpWord.Word) == "\0")
        {
            std::cout << "输入有误，请重新输入" << std::endl;
            continue; 
        }
        
        /**/
        MEAN:
        std::cout << "请提供一个单词意义:" << std::endl;
        std::cin.getline(TmpWord.Meaning,60);
        
        if(std::string(TmpWord.Meaning) == "\0")
        {
            std::cout << "输入有误，请重新输入" << std::endl;
            goto MEAN;
        }
        /**/
        
        /**/
        SPCH:
        std::cout << "请提供单词的词性:" << std::endl;
        
        std::cin.getline(TmpWord.Part_of_speech,10);
        if(std::string(TmpWord.Part_of_speech) == "\0")
        {
            std::cout << "输入有误，请重新输入" << std::endl;
            goto SPCH;
        }
        /**/
        
        File.write((char*)&TmpWord,sizeof(WordsChecking::Word));
        Count--;
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

/**
*做测试前的准备工作，读取文件,装填数组等.
*@Param iscmd:判断是否由控制台命令输入. 
*@Param CmdFileName:控制台命令输入的文件名. 
*/

int PrepareTest(bool iscmd,const char* CmdFileName)
{
    std::string FileName;
    std::ifstream File;
    if(!iscmd)
    {
        system("cls");
        std::cout << "请输入单词文件名:";
        std::cin >> FileName;
        File.open(FileName.c_str(),std::ios_base::binary);
    }
    else
    {
        File.open(CmdFileName,std::ios_base::binary);
    }
    
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

/**
*开始测试函数体. 
*/
int Test(void)
{
    bool Tested[MaxNum] = {0};
    int Displayed[4] = {-1,-1,-1,-1};
    
    int RightAnswerIndex;
    int index;      //被测试的单词的在整个单词表单数据结构的下标. 
    
    while(true)
    {
        do{
            index = rand()%MaxNum;
        }while(Tested[index] == true);
        
        system("cls");
        //显示: 
        std::cout << "  "<< WordTable[index].Word  << "\t\t"<< '[' << WordTable[index].Part_of_speech << ']' << std::endl;
        RightAnswerIndex = 1+rand()%4;
        
        for(int i = 1;i < 5;i++)
        {
            int Fourofindex;
            if(RightAnswerIndex == i)
            {
                std::cout << ' ' << i << '.' << WordTable[index].Meaning << std::endl;
                continue;
            }
            do{
                Fourofindex = rand()%MaxNum;
            }while(std::string(WordTable[Fourofindex].Word) == std::string(WordTable[index].Word) || Displayed[0] == Fourofindex ||Displayed[1] == Fourofindex ||Displayed[2] == Fourofindex ||Displayed[3] == Fourofindex);
            Displayed[i-1] = Fourofindex;
            
            std::cout << ' ' << i << '.' << WordTable[Fourofindex].Meaning << std::endl;
        }
        
        std::cout << std::endl << "请输入您的选择:";
        
        int Ch;
        std::cin >> Ch;
        
        if(Ch == RightAnswerIndex)
        {
            std::cout  << std::endl << "    恭喜,选择正确!" << std::endl;
            Tested[index] = true;
        }
        else
        {
            std::cout  << std::endl << "    啊哦,选择错误!" << std::endl;
            std::cout  << std::endl << "    正确意义: "  << WordTable[index].Meaning << std::endl;
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
        
        //复位 
        Displayed[0] = -1;
        Displayed[1] = -1;
        Displayed[2] = -1;
        Displayed[3] = -1;
        
    }
    
    std::cout << "单词测试完成!" << std::endl;
    delete [] WordTable;
    return 0;
}
