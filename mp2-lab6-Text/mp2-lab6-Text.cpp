//#include <iostream>
#include "TText.h"


TMem TTextLink::mem;

int main()
{
    TTextLink::InitMem(100);
    TText text;
    text.ReadRec("Textt.txt");
}
