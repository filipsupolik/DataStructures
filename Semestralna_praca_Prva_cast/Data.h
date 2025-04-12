#pragma once
#include <vector>
#include <libds/amt/implicit_sequence.h>
#include <libds/amt/explicit_hierarchy.h>
#include "Zastavka.h"
#include "Citac.h"

class Data
{
protected:
	std::vector<Dopravca*> zoznamZastavok;
	std::vector<Dopravca*> filtrovanyZoznam;
	using HierarchyBlockType = ds::amt::MultiWayExplicitHierarchyBlock<Dopravca*>;
	using SequenceBlockType = ds::amt::MemoryBlock<Dopravca*>;
	using ZoznamObci = ds::amt::ImplicitSequence<Dopravca*>;
	using ZoznamUlic = ds::amt::ImplicitSequence<Dopravca*>;
	ZoznamObci* zoznamObci = nullptr;
	ZoznamUlic* zoznamUlic = nullptr;
public:
	Data();
	Data(const Data& data) : zoznamZastavok(data.zoznamZastavok), filtrovanyZoznam(data.filtrovanyZoznam), zoznamObci(data.zoznamObci), zoznamUlic(data.zoznamUlic) {};
	Data(std::string subor)
	{
		this->nacitajZastavku(subor);
		this->zoznamObci= new ZoznamObci();
		this->zoznamUlic = new ZoznamUlic();
		nacitajObceDoSekvencie();
		nacitajUliceDoSekvencie();
	};
	~Data()
	{
		for (auto it = this->zoznamObci->begin(); it != this->zoznamObci->end(); ++it) {
			delete (*it);
		}
		delete this->zoznamObci;
	}

	void nacitajObceDoSekvencie()
	{
		for (size_t i = 0; i < zoznamZastavok.size(); ++i)
		{
			std::string nacitavanyNazov = zoznamZastavok[i]->manicipality;
			if (zoznamObci->isEmpty())
			{
				zoznamObci->insertFirst().data_ = new Dopravca();
				zoznamObci->accessFirst()->data_->manicipality =  nacitavanyNazov;
				std::cout << "Pridana obec: " << nacitavanyNazov << std::endl;
			}
			else
			{
				bool existuje = false;
				for (auto it = zoznamObci->begin(); it != zoznamObci->end(); ++it)
				{
					Dopravca* obec = (*it);
					if (obec->manicipality.substr(0, 3) == nacitavanyNazov.substr(0, 3))
					{
						existuje = true;
						break;
					}
				}
				if (!existuje)
				{
					zoznamObci->insertLast().data_ = new Dopravca();
					zoznamObci->accessLast()->data_->manicipality = nacitavanyNazov;
					std::cout << "Pridana obec: " << nacitavanyNazov << std::endl;
				}
			}
			//zoznamObci->processAllBlocksForward([&](std::string nacitavanyNazov) {
				//});
		}
	}

	void nacitajUliceDoSekvencie()
	{
		for (size_t i = 0; i < zoznamZastavok.size(); ++i)
		{
			std::string nacitavanyNazovUlice = zoznamZastavok[i]->street;
			std::string nacitavanyNazovObce = zoznamZastavok[i]->manicipality;
			if (zoznamUlic->isEmpty())
			{
				zoznamUlic->insertFirst().data_ = new Dopravca();
				zoznamUlic->accessFirst()->data_->manicipality = nacitavanyNazovObce;
				zoznamUlic->accessFirst()->data_->street = nacitavanyNazovUlice;
				std::cout << "Pridana ulica: " << nacitavanyNazovUlice << ", " << nacitavanyNazovObce << std::endl;
			}
			else
			{
				bool existuje = false;
				for (auto it = zoznamUlic->begin(); it != zoznamUlic->end(); ++it)
				{
					Dopravca* ulica = (*it);
					if (ulica->street == nacitavanyNazovUlice)
					{
						existuje = true;
						break;
					}
				}
				if (!existuje)
				{
					zoznamUlic->insertLast().data_ = new Dopravca();
					zoznamUlic->accessLast()->data_->manicipality = nacitavanyNazovObce;
					zoznamUlic->accessLast()->data_->street = nacitavanyNazovUlice;
					std::cout << "Pridana ulica: " << nacitavanyNazovUlice << ", " << nacitavanyNazovObce << std::endl;
				}
			}
		}
	}
private:
	void nacitajZastavku(std::string subor) 
	{
		Citac citac(subor);
		citac.preskocPrvyRiadok();
		while (citac.citajRiadok()) 
		{
			zoznamZastavok.push_back(citac.vytvorZastavku());
		}
	};
};