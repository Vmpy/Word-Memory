#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

int WriteFile(void);

int main(int argc,char** argv)
{
    WriteFile();
    system("pause");
	return 0;
}

int WriteFile(void)
{
    std::string FileName;
    std::cout << "Please Give A File Name(Not uesd):" << std::endl;
    std::cin >> FileName;
    std::ofstream File;
    File.open(FileName.c_str(),std::ios_base::app);
    while(1)
    {
        std::string Word,Meaning;
        std::cout << "Please Give A Word(if Word is exit10,then exit):" << std::endl;
        std::cin >> Word;
        if(Word == "exit10")
        {
            break;
        }
        std::cout << "Please Give A Meaning:" << std::endl;
        std::cin >> Meaning;
        File.write(Word.c_str(),Word.size());
        File << "\n";
        File.write(Meaning.c_str(),Meaning.size());
        File << "\n";
    }
    File.close();
}
