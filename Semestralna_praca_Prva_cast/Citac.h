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
	bool vytvorZastavku(Dopravca& zastavka)
	{
		std::string hodnota;
		std::stringstream riadokStream(aktualnyRiadok);

		// stopId
		std::getline(riadokStream, hodnota, ';');
		if (hodnota.empty()) return false;
		zastavka.stopId = std::stoi(hodnota);

		// street
		std::getline(riadokStream, hodnota, ';');
		if (hodnota.empty()) return false;
		zastavka.street = hodnota;

		// longitude
		std::getline(riadokStream, hodnota, ';');
		if (hodnota.empty()) return false;
		zastavka.longitude = std::stod(hodnota);

		// latitude
		std::getline(riadokStream, hodnota, ';');
		if (hodnota.empty()) return false;
		zastavka.latitude = std::stod(hodnota);

		// manicipality
		std::getline(riadokStream, hodnota, ';');
		if (hodnota.empty()) return false;
		zastavka.manicipality = hodnota;

		return true;
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
