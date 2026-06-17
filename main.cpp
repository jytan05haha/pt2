#include <iostream>
#include "UI.h"
#include <windows.h>
#include <string>
#include <vector>
#include<iomanip>
#include<cctype>
using namespace std;

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    sendUserMsg("Press [Alt + Enter] to maximize the window and press [Enter]", 15, 11);

    //Loading({ .speed = 2});

    //printSocialSystem();

    //printEconomicSystem();

    printEandCSystem();

    //printNewSystem();

	return 0;
}