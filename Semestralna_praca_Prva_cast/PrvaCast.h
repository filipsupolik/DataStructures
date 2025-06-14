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
public:
	PrvaCast(Data& data) : Data(data) {};
	void spustiFilter() 
	{
		while (!this->koniecProgramu)
		{
			char nacitajVstup = vstupZKlavesnice();
			if (nacitajVstup == 'k') {
				break;
			}
			else {
				zadajZoznamParametrov(nacitajVstup);
				filtrujZoznam(*zoznamZastavok, *filtrovanyZoznam, nacitajVstup);
				vypisZastavky();
			}
		};
	}

	void filtrujZoznam(ZoznamZastavok& vstupnyZoznam, ZoznamZastavok& vystupnyZoznam, const char typFiltru)
	{
		std::function<bool(const Dopravca)> jeVObci = [&](const Dopravca zastavka) -> bool {
			return toLowerCase(zastavka.manicipality) == this->nazovObce;
		};

		std::function<bool(const Dopravca)> jeNaUlici = [&](const Dopravca zastavka) -> bool {
			return toLowerCase(zastavka.street).find(this->castNazvuUice) != std::string::npos;
		};

		std::function<bool(const Dopravca)> jeVOblasti =
			[&](const Dopravca zastavka) {
				return zastavka.latitude >= this->minLat
				&& zastavka.latitude <= this->maxLat
				&& zastavka.longitude >= this->minLong
				&& zastavka.longitude <= this->maxLong;		
		};


		std::function<bool(const Dopravca)> pouzivanyFilter;
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
			break;
		default:
			break;
		}

		Algorithm::findAndInsertElement(
			vstupnyZoznam.begin(),
			vstupnyZoznam.end(),
			pouzivanyFilter,
			vystupnyZoznam,
			[&](ZoznamZastavok& vystupnyZoznam, Dopravca hodnota) {
				vystupnyZoznam.insertLast().data_ = hodnota;
			}
		);
	}
	void vypisZastavky()
	{
		for (size_t i = 0; i < filtrovanyZoznam->size(); ++i)
		{
			std::cout << filtrovanyZoznam->access(i)->data_.FullNameBusStop() << std::endl;
		}
	}
};

