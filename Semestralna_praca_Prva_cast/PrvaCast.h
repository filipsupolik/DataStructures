#pragma once
#include "Algorithm.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <algorithm>
#include "Data.h"
#include "Citac.h"
class PrvaCast: public Data
{
private:
	bool koniecProgramu = false;
	std::string nazovObce, castNazvuUice = "";
	double minLong, minLat, maxLong, maxLat = 0.0;
	std::string toLowerCase(const std::string& input) {
		std::string result = input;
		std::transform(result.begin(), result.end(), result.begin(),
			[](unsigned char c) { return std::tolower(c); });
		return result;
	}
public:
	PrvaCast(Data& data) : Data(data) {}
	void spustiFilter(const std::string& vstupnySubor) 
	{
		char nacitajVstup = vstupZKlavesnice();
		zadajZoznamParametrov(nacitajVstup);
		filtrujZoznam(zoznamZastavok, filtrovanyZoznam, nacitajVstup);
		vypisZastavky();
	}
	char vstupZKlavesnice()
	{
		char vstup;
		std::cout << "Zadajte typ filtra \n";
		std::cout << "o - nazov obce kde sa zastavka nachadza, \n";
		std::cout << "n - cast nazvu ulice \n";
		std::cout << "g - zastavka sa nachadza v geografickej oblasti \n";
		std::cout << "k - ukonci program \n";
		std::cin >> vstup;
		return vstup;
	}

	void zadajZoznamParametrov(const char& typFiltra)
	{
		switch (typFiltra)
		{
		case 'o':
			std::cout << "Zadajte cely nazov obce, kde sa zastavka nachadza: \n";
			std::cin >> this->nazovObce;
			this->nazovObce = toLowerCase(this->nazovObce);
			break;
		case 'n':
			std::cout << "Zadajte cast nazvu ulice, kde sa zastavka nachadza: \n";
			std::cin >> this->castNazvuUice;
			this->castNazvuUice = toLowerCase(this->castNazvuUice);
			break;
		case 'g':
			std::cout << "Zadajte minimalnu dlzku:\n";
			std::cin >> this->minLong;
			std::cout << "Zadajte maximalnu dlzku:\n";
			std::cin >> this->maxLong;
			std::cout << "Zadajte minimalnu sirku:\n";
			std::cin >> this->minLat;
			std::cout << "Zadajte maximalnu dlzku:\n";
			std::cin >> this->maxLat;
		}
	}

	void filtrujZoznam(const std::vector<Dopravca*>& vstupnyZoznam, std::vector<Dopravca*>& vystupnyZoznam, const char typFiltru)
	{
		std::function<bool(const Dopravca*)> jeVObci = [&](const Dopravca* zastavka) -> bool {
			return toLowerCase(zastavka->manicipality) == this->nazovObce;
		};

		std::function<bool(const Dopravca*)> jeNaUlici = [&](const Dopravca* zastavka) -> bool {
			return toLowerCase(zastavka->street).find(this->castNazvuUice) != std::string::npos;
		};

		std::function<bool(const Dopravca*)> jeVOblasti =
			[&](const Dopravca* zastavka) {
				return zastavka->latitude >= this->minLat
				&& zastavka->latitude <= this->maxLat
				&& zastavka->longitude >= this->minLong
				&& zastavka->longitude <= this->maxLong;		
		};


		std::function<bool(const Dopravca*)> pouzivanyFilter;
		switch (typFiltru)
		{
		case 'o':
			pouzivanyFilter = jeVObci;
			break;
		case 'n':
			pouzivanyFilter = jeNaUlici;
			break;
		case 'g':
			pouzivanyFilter = jeVOblasti;

		default:
			break;
		}

		Algorithm::findAndInsertElement(
			vstupnyZoznam.begin(),
			vstupnyZoznam.end(),
			pouzivanyFilter,
			vystupnyZoznam,
			[&](std::vector<Dopravca*>& vystupnyZoznam, Dopravca* hodnota) {
				vystupnyZoznam.push_back(hodnota);
			}
		);
	}
	void vypisZastavky()
	{
		for (size_t i = 0; i < filtrovanyZoznam.size(); ++i)
		{
			std::cout << filtrovanyZoznam[i]->stopId << ", " << filtrovanyZoznam[i]->street << ", " << filtrovanyZoznam[i]->manicipality << std::endl;
		}
	}

	bool koniec()
	{
		switch (vstupZKlavesnice())
		{
		case 'k':
			this->koniecProgramu = true;
			break;
		}
		return this->koniecProgramu;
	}
};

