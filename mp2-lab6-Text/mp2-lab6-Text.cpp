//#include <iostream>
#include "TText.h"


TMem TTextLink::mem;

int main()
{
    TTextLink::InitMem;
    TText text;
    text.Read("Textt.txt");
    text.Print();
}
