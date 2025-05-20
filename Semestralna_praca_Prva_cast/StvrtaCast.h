#pragma once
#include <libds/adt/sorts.h>
#include <libds/amt/implicit_sequence.h>
#include <locale>

#include "Dopravca.h"
class StvrtaCast
{
	ds::adt::Sort<Dopravca>* triedenie; 
	std::locale skLokalizacia;
public:
	StvrtaCast(ds::amt::ImplicitSequence<Dopravca>& zastavky)
	{
		this->skLokalizacia = std::locale("Slovak_Slovakia.1250");
		triedenie = new ds::adt::QuickSort<Dopravca>;
		std::cout << "Vyber utriedenie [o]bec/ulica, [i]d zastavky, [b]ez triedenia" << std::endl;
		std::string utried;
		std::cin >> utried;
		std::cin.ignore(256, '\n');

		if (utried == "o")
		{
			triedenie->sort(zastavky, [&](Dopravca dp1, Dopravca dp2) {
				return dp1.manicipality == dp2.manicipality ?
					this->skLokalizacia(dp1.street, dp2.street) :
					this->skLokalizacia(dp1.manicipality, dp2.manicipality); }
			);

			for (auto it = zastavky.accessFirst(); it != zastavky.accessLast(); it++)
			{
				std::cout << it->data_.manicipality + " ," + it->data_.street << std::endl;
			}
		}
		else if(utried == "i")
		{
			triedenie->sort(zastavky, [&](Dopravca dp1, Dopravca dp2)
				{
					return dp1.stopId < dp2.stopId;
				}
			);

			for (auto it = zastavky.accessFirst(); it != zastavky.accessLast(); it++)
			{
				std::cout << it->data_.FullNameBusStop();
			}
		}
		else
		{
			for (auto it = zastavky.accessFirst(); it != zastavky.accessLast(); it++)
			{
				std::cout << it->data_.FullNameBusStop() << std::endl;
			}
		}

		
	};

	~StvrtaCast()
	{
		delete triedenie;
	};
};