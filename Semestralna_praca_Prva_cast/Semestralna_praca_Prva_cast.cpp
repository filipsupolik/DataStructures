// Semestralna_praca_Prva_cast.cpp : This file contains the 'main' function. Program execution begins and ends there.
//	
#include "PrvaCast.h"
#include "DruhaCast.h"
#include "Algorithm.h"
//pouzitie konzoly na subory kodovane vo Windows-1250
#include <Windows.h>
#include <iostream>
#include <string>
#include "TretiaCast.h"
int main()
{
	initHeapMonitor();
	SetConsoleOutputCP(1250);
	SetConsoleCP(1250);

	std::string vstupnySubor = "C:\\Users\\filip\\OneDrive - Žilinská univerzita v Žiline\\4. sem\\AUS\\AUS-VS2022\\GRT_Stops – kópia.csv";
	Data data(vstupnySubor);
	PrvaCast prvaCast(data);
	DruhaCast druhaCast(data);
	TretiaCast tretiaCast(data);
    int cisloUrovne = 50;
    while (cisloUrovne != 0) {
        std::cout << "Zadaj cislo urovne [1-3] alebo 0 pre koniec: ";
        std::string vstup;
        std::cin >> vstup;
        cisloUrovne = std::isdigit(vstup.at(0)) ? std::stoi(vstup.substr(0, 1)) : 42;
        std::cin.ignore(256, '\n');

        if (cisloUrovne == 1) {
            prvaCast.spustiFilter();
        }
        else if (cisloUrovne == 2) {
            druhaCast.IteratorInterface();
        }
        else if (cisloUrovne == 3) {
            tretiaCast.spustiTretiuCast();
        }
        else if (cisloUrovne == 0) {

            std::cout << "Koniec\n";
        }
        else {
            std::cout << "\x1B[31mNespravne cislo urovne\033[0m\n";
        }
    }
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
