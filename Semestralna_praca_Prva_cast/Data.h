	#pragma once
#include <vector>
#include <libds/amt/implicit_sequence.h>
#include <libds/amt/explicit_hierarchy.h>
#include "Dopravca.h"
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
	using HierarchyBlockType = ds::amt::MultiWayExplicitHierarchyBlock<Dopravca*>;
	using SequenceBlockType = ds::amt::MemoryBlock<Dopravca*>;
	using ZoznamObci = ds::amt::ImplicitSequence<Dopravca*>;
	using ZoznamUlic = ds::amt::ImplicitSequence<Dopravca*>;
	using ZoznamZastavok = ds::amt::ImplicitSequence<Dopravca>;
	ZoznamObci* zoznamObci = nullptr;
	ZoznamUlic* zoznamUlic = nullptr;
	ZoznamZastavok* zoznamZastavok = nullptr;

public:
	Data();
	Data(const Data& data)
	{
		this->zoznamZastavok = data.zoznamZastavok;
		this->filtrovanyZoznam = data.filtrovanyZoznam;

		this->zoznamObci = new ZoznamObci();
		for (auto it = data.zoznamObci->begin(); it != data.zoznamObci->end(); ++it)
		{
			Dopravca* novaObec = new Dopravca(**it);
			this->zoznamObci->insertLast().data_ = novaObec;
		}

		this->zoznamUlic = new ZoznamUlic();
		for (auto it = data.zoznamUlic->begin(); it != data.zoznamUlic->end(); ++it)
		{
			Dopravca* novaUlica = new Dopravca(**it);
			this->zoznamUlic->insertLast().data_ = novaUlica;
		}

		this->zoznamZastavok = new ZoznamZastavok();
	}
	Data(std::string subor)
	{
		this->zoznamObci= new ZoznamObci();
		this->zoznamUlic = new ZoznamUlic();
		this->zoznamZastavok = new ZoznamZastavok();
		nacitajZastavku(subor);
		nacitajObceDoSekvencie();
		nacitajUliceDoSekvencie();
	};
	~Data()
	{
		for (auto it = this->zoznamObci->begin(); it != this->zoznamObci->end(); ++it) {
			delete (*it);
		}
		delete this->zoznamObci;
		for (auto it = this->zoznamUlic->begin(); it != this->zoznamUlic->end(); ++it) {
			delete (*it);
		}
		delete this->zoznamUlic;
	}

	void nacitajObceDoSekvencie()
	{
		size_t poradieObce = 0;
		for (size_t i = 0; i < this->zoznamZastavok->size(); ++i)
		{
			std::string nacitavanyNazov = this->zoznamZastavok->access(i)->data_.manicipality;
			if (zoznamObci->isEmpty())
			{
				zoznamObci->insertFirst().data_ = new Dopravca();
				zoznamObci->accessFirst()->data_->manicipality =  nacitavanyNazov;
			}
			else if(zoznamObci->access(poradieObce)->data_->manicipality != nacitavanyNazov)
			{
				zoznamObci->insertLast().data_ = new Dopravca();
				zoznamObci->accessLast()->data_->manicipality = nacitavanyNazov;
				poradieObce++;
			}
		}
	}

	void nacitajUliceDoSekvencie()
	{
		size_t poradieUlice = 0;
		for (size_t i = 0; i < this->zoznamZastavok->size(); ++i)
		{
			std::string nacitavanyNazovUlice = this->zoznamZastavok->access(i)->data_.street;
			std::string nacitavanyNazovObce = this->zoznamZastavok->access(i)->data_.manicipality;
			if (zoznamUlic->isEmpty())
			{
				zoznamUlic->insertFirst().data_ = new Dopravca();
				zoznamUlic->accessFirst()->data_->manicipality = nacitavanyNazovObce;
				zoznamUlic->accessFirst()->data_->street = nacitavanyNazovUlice;
			}
			else if (zoznamUlic->access(poradieUlice)->data_->street != nacitavanyNazovUlice)
			{
				zoznamUlic->insertLast().data_ = new Dopravca();
				zoznamUlic->accessLast()->data_->manicipality = nacitavanyNazovObce;
				zoznamUlic->accessLast()->data_->street = nacitavanyNazovUlice;
				poradieUlice++;
			}
		}
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

