#pragma once
#include <libds/adt/table.h>
#include <libds/heap_monitor.h>
#include "Dopravca.h"

class TretiaCast: public Data
{
private:
	ds::adt::Treap<int, Dopravca*>* tabulkaZastavok;
public:
	TretiaCast(const Data& data): Data(data)
	{
		tabulkaZastavok = new ds::adt::Treap<int, Dopravca*>;
		this->naplntabulku(this->zoznamZastavok, (*tabulkaZastavok));
	};

	~TretiaCast()
	{
		delete tabulkaZastavok;
	}

	void naplntabulku(ZoznamZastavok* nacitaneZastavky, ds::adt::Table<int, Dopravca*>& utriedenaTabulkaZastavok)
	{
		Dopravca** duplikat;
		for (auto it = nacitaneZastavky->begin(); it != nacitaneZastavky->end(); it++)
		{
			utriedenaTabulkaZastavok.insert((*it).stopId, &(*it));
		}
	}

	Dopravca* najdiZastavku(int idZastavky, ds::adt::Table<int, Dopravca*>& vstupnaTabulka)
	{
		Dopravca** najdenaZastavka;
		if (vstupnaTabulka.tryFind(idZastavky, najdenaZastavka))
		{
			return *najdenaZastavka;
		}
		return nullptr;
	}

	void vypisZastavku(int idZastavky)
	{
		Dopravca* najdenaZastavka = this->najdiZastavku(idZastavky, *tabulkaZastavok);
		if (najdenaZastavka == nullptr)
		{
			std::cout << "Zastavka nebola najdena" << std::endl;
		}
		else
		{
			std::cout << najdenaZastavka->FullNameBusStop();
		}
	}

	void spustiTretiuCast()
	{
		std::string nacitaneIDZastavky;
		std::cout << "Zadaj id zastavky ktoru chces najst: " << std::endl;
		std::cin >> nacitaneIDZastavky;
		int kluc = std::stoi(nacitaneIDZastavky);
		this->vypisZastavku(kluc);
	}
};
