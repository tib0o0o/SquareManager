#include <iostream>
#include "RectLoader.h"
int main()
{
    RectLoader* manager = new RectLoader("G:/Temp/dataRect.txt");
    manager->load();
    manager->backe();
    std::cout << "Hello World!\n";
}
