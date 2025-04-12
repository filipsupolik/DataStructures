// Semestralna_praca_Prva_cast.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "PrvaCast.h"
#include "DruhaCast.h"
#include "Algorithm.h"
//pouzitie konzoly na subory kodovane vo Windows-1250
#include <Windows.h>
#include <iostream>
#include <string>
int main()
{
	SetConsoleOutputCP(1250);
	SetConsoleCP(1250);

	std::string vstupnySubor = "C:\\Users\\filip\\OneDrive - Žilinská univerzita v Žiline\\4. sem\\AUS\\AUS-VS2022\\GRT_Stops – kópia.csv";
	Data data(vstupnySubor);
	PrvaCast prvaCast(data);
	DruhaCast druhaCast(data);
	/*while (prvaCast.koniec())
	{
		prvaCast.spustiFilter(vstupnySubor);
	}*/
	prvaCast.spustiFilter(vstupnySubor);
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
