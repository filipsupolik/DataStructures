	#pragma once
#include <vector>
#include <libds/amt/implicit_sequence.h>
#include <libds/amt/explicit_hierarchy.h>
#include "Dopravca.h"
#include "NodeDopravca.h"
#include "Citac.h"

class Data
{
protected:
	bool koniecProgramu = false;
	std::string nazovObce, castNazvuUice = "";
	double minLong, minLat, maxLong, maxLat, equal = 0.0;
	std::string toLowerCase(const std::string& input) {
		std::string result = input;
		std::transform(result.begin(), result.end(), result.begin(),
			[](unsigned char c) { return std::tolower(c); });
		return result;
	}
	std::vector<Dopravca> filtrovanyZoznam;
	using ZoznamZastavok = ds::amt::ImplicitSequence<Dopravca>;
	ZoznamZastavok* zoznamZastavok = nullptr;

public:
	Data();
	Data(const Data& data)
	{
		this->zoznamZastavok = data.zoznamZastavok;
		this->filtrovanyZoznam = data.filtrovanyZoznam;
	}
	Data(std::string subor)
	{
		this->zoznamZastavok = new ZoznamZastavok();
		nacitajZastavku(subor);
	};
	~Data()
	{
		delete this->zoznamZastavok;
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
private:
	void nacitajZastavku(std::string subor) 
	{
		size_t pocetZastavok = 0;
		Citac citac(subor);
		citac.preskocPrvyRiadok();
		while (citac.citajRiadok()) 
		{
			Dopravca novaZastavka = citac.vytvorZastavku();
				this->zoznamZastavok->insertLast().data_ = novaZastavka;
				pocetZastavok++;
		}
		std::cout << "Nacitalo sa " << pocetZastavok << " zastavok." << std::endl;
	};
};

