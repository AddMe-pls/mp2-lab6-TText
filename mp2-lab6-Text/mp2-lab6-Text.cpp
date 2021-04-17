//#include <iostream>
#include "TText.h"


TMem TTextLink::mem;

int main()
{
    TTextLink::InitMem(100);
    TText text;
    text.Read("Textt.txt");
    text.Print();
    text.GoFirstLink();
    std::cout << "1 - InsDownLine" << std::endl << "2 - InsNextLine" << std::endl << "3 - DelDownLine" << std::endl <<
        "4 - DelNextLine" << std::endl << "5 - PrintFree" << std::endl << "6 - Print Text" << std::endl <<
        "7 - GoDownLink" << std::endl << "8 - GoNextLink" << std::endl << "9 - GoFirstLink" << std::endl << "10 - Clean" << std::endl << "11 - Save"<< std::endl;
    while (true)
    {
        int k;
        std::cin >> k;
        switch (k)
        {
        case (1):
        {
            char stroka[80];
            std::cin >> stroka;
            //text.GoFirstLink();
            text.InsDownLine(stroka);
            text.Print();
            break;
        }
        case(2):
        {
            char stroka1[80];
            std::cin >> stroka1;
            //text.GoFirstLink();
            text.InsNextLine(stroka1);
            text.Print();
            break;
        }
        case(3):
        {
            //text.GoFirstLink();
           // text.GoNextLink();
            text.DelDownLine();
            text.Print();
            break;
        }
        case(4):
        {
            //text.GoFirstLink();
            text.DelNextLine();
            text.Print();
            break;
        }
        case(5):
        {
            TTextLink::PrintFree();
            break;
        }
        case(6):
        {
            text.Print();
            break;
        }
        case(7):
        {
            text.GoDownLink();
            break;
        }
        case(8):
        {
            text.GoNextLink();
            break;
        }
        case(9):
        {
            text.GoFirstLink();
            break;
        }
        case(10):
        {
            TTextLink::Clean(text);
            break;
        }
        case(11):
        {
            text.Save("Saved.txt");
            break;
        }
        }
    }
}
