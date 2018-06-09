#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include "Data.h"

//单词表指针，用于单词测试. 
WordsChecking::Word * WordTable;
char WorkFileName[50];
int MaxNum;

int Menu(void);
int GetWordNum(const char*);
int WriteWordsFile(void);
int ReloadWordsFile(const char*,WordsChecking::Word*,int);
int PrepareTest(bool,const char*);
int Test(void);
int DisplayWords(int);
int RewriteFile(int);
int DeleteLemma(const int);


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
                case 3:PrepareTest(false,0);DisplayWords(MaxNum);delete [] WordTable;WordTable = nullptr;break;
                case 4:PrepareTest(false,0);DeleteLemma(GetWordNum(WorkFileName));break;
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
        
        if(std::string(TmpWord.Word) == "ExIt" && Count <= 0)
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
    strcpy(WorkFileName,FileName.c_str());
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
        FileName = CmdFileName;
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
    strcpy(WorkFileName,FileName.c_str());
    return 0;
}

int Menu(void)
{
    int Ch;
    std::cout << "*****************单词记忆程序*****************" << std::endl;
    std::cout << "               1.写入单词文件;" << std::endl;
    std::cout << "               2.选择文件测试;" << std::endl;
    std::cout << "               3.选择文件显示;" << std::endl;
    std::cout << "               4.删除单词词条;" << std::endl;
    std::cout << "            [其他任意键].退出程序." << std::endl;
    std::cin >> Ch;
    return Ch;
}

/**
*开始测试函数体. 
*/
int Test(void)
{
    int Checked = 0;
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
        std::cout << "  单词总数:" << MaxNum << "  剩余:" << (MaxNum-Checked) << std::endl; 
        std::cout << "  "<< WordTable[index].Word  << "\t  "<< '[' << WordTable[index].Part_of_speech << ']' << std::endl;
        RightAnswerIndex = 1+rand()%4;
        
        for(int i = 1;i < 5;i++)
        {
            int Fourofindex;//四个选项的index变量 
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
            std::cout  << std::endl << "    选择正确!" << std::endl;
            Tested[index] = true;
            Checked++;
        }
        else
        {
            std::cout  << std::endl << "    选择错误!" << std::endl;
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
    delete [] WordTable;
    WordTable = nullptr; 
    std::cout << "单词测试完成!" << std::endl;
    return 0;
}

int DisplayWords(int Max)
{
    system("cls");
    std::cout.flags(std::ios::left); //左对齐
    for(int i = 0;i < Max;i++)
    {
        std::cout <<  i << ".单词:";
        std::cout.width(15);
        std::cout << WordTable[i].Word;
        std::cout << '[';
        std::cout << WordTable[i].Part_of_speech;
        std::cout.width(10);
        std::cout << ']';
        std::cout << "意义:";
        std::cout.width(15);
        std::cout << WordTable[i].Meaning << std::endl; 
    }
    return 0;
}

int DeleteLemma(const int Max)
{
    if(Max == 0)
    {
        std::cout << "Error!Cannot open empty file!";
        return 0;
    }
    
    int OrderNum = -1;  //词条序号 
    bool Mark[Max] = {0};   //表示被标记（被删除）的词条 
    char Yes;       //是否继续条件测试 
    DisplayWords(Max);  //显示词表 
    
    while(true)
    {
        do
        {
            std::cout << "请输入即将删除的词条序号:";
            std::cin >> OrderNum;
        }while(OrderNum < 0 && OrderNum > Max);
        
        Mark[OrderNum] = true;  //标记为true 

        std::cout << "是否继续,Y键继续[其他任意键退出]:" << std::flush;
        std::cin >> Yes;
        if(!(Yes == 'Y' || Yes == 'y'))
        {
            break;
        }
    }
    
    std::ofstream FileReWrite;
    FileReWrite.open(WorkFileName);
    
    for(int i = 0;i < Max;i++)
    {
        if(!Mark[i])    //没有被标记的词条重新写入. 
        {
            FileReWrite.write((char*)&WordTable[i],sizeof(WordsChecking::Word));
        }
    }
    FileReWrite.close();
    std::cout << "单词词条删除完成." << std::endl;
    //照例清空内存 
    delete [] WordTable;
    WordTable = nullptr;
    return 0;
}
