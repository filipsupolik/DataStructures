#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
class Citac
{
public:
	Citac(const std::string& nazovSuboru)
	{
		subor.open(nazovSuboru);
		if (!subor.is_open())
		{
			std::cout << "Subor sa nepodarilo otvorit" << std::endl;
			return;
		}
		else
		{
			std::cout << "Subor bol otvoreny" << std::endl;
		}
	};
	~Citac()
	{
		subor.close();
	};
	Dopravca vytvorZastavku()
	{
		Dopravca zastavka;
		std::string hodnota;
		std::stringstream riadokStream(aktualnyRiadok);
		std::getline(riadokStream, hodnota, ';');
		zastavka.stopId = std::stoi(hodnota);

		std::getline(riadokStream, hodnota, ';');
		zastavka.street = hodnota;

		std::getline(riadokStream, hodnota, ';');
		if (hodnota == "") {
			zastavka.longitude = 0.0;
			zastavka.latitude = 0.0;
		}
		else
		{
			zastavka.longitude = std::stod(hodnota);
			std::getline(riadokStream, hodnota, ';');
			zastavka.latitude = std::stod(hodnota);
		}
		std::getline(riadokStream, hodnota, ';');
		zastavka.manicipality = hodnota;

		return zastavka;
	}

	void preskocPrvyRiadok()
	{
		std::string riadok;
		std::getline(subor, riadok);
	}

	bool citajRiadok()
	{
		return !std::getline(subor, aktualnyRiadok).fail();
	}

private:
	std::string aktualnyRiadok = "";
	std::ifstream subor;
};
