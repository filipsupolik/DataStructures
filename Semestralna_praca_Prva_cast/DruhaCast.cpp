#include "DruhaCast.h"

DruhaCast::DruhaCast(const Data& other) : Data(other)
{
	this->vytvorHierarchiu();	
}

void DruhaCast::vytvorHierarchiu()
{
	hierarchy.emplaceRoot();
	NacitajObce();
	NacitajUlice();
	NacitajZastavky();
}

void DruhaCast::NacitajObce()
{
	for (auto it = this->zoznamObci->begin(); it != this->zoznamObci->end(); ++it)
	{
		Dopravca* obec = (*it);
		auto& novyVrcholObec = hierarchy.emplaceSon(*hierarchy.accessRoot(), hierarchy.accessRoot()->sons_->size());
		novyVrcholObec.data_ = obec;
	}
}

void DruhaCast::NacitajUlice()
{
	for (auto it = this->zoznamUlic->begin(); it != this->zoznamUlic->end(); ++it)
	{
		Dopravca* ulica = (*it);
		auto& aktualnaObec = dajObec(ulica->manicipality);
		auto& vrcholUlica = hierarchy.emplaceSon(aktualnaObec, aktualnaObec.sons_->size());
		vrcholUlica.data_ = ulica;
	}
}

void DruhaCast::NacitajZastavky()
{
	size_t poradiePridania = 1;
	for (auto it = zoznamZastavok.begin(); it != zoznamZastavok.end(); ++it)
	{
		Dopravca* zastavka = (*it);
		auto& aktualnaObec = dajObec(zastavka->manicipality);
		auto& aktualnaUlica = dajUlicu(zastavka->manicipality, zastavka->street);
		std::cout << "Pridana zastavka: " << zastavka->stopId << std::endl;
		if (aktualnaUlica.data_->street == zastavka->street)
		{
			auto& vrcholZastavka = hierarchy.emplaceSon(aktualnaUlica, aktualnaUlica.sons_->size());
		}
		poradiePridania++;
	}
}

DruhaCast::HierarchyBlockType& DruhaCast::dajObec(std::string nazovObce)
{
	size_t poradieObce = -1;
	
	auto* najdenaObecSvlastnostou = this->zoznamObci->findBlockWithProperty([&](SequenceBlockType* blockObec) {
		return blockObec->data_->manicipality == nazovObce;
		});
	if (najdenaObecSvlastnostou != nullptr) {

		poradieObce = this->zoznamObci->calculateIndex(*najdenaObecSvlastnostou);
	}
	return *hierarchy.accessRoot()->sons_->access(poradieObce)->data_;
}

DruhaCast::HierarchyBlockType& DruhaCast::dajUlicu(std::string nazovObce,std::string nazovUlice)
{
	size_t poradieObce = -1;
	size_t poradieUlice = -1;
	auto* najdenaObecSvlastnostou = this->zoznamObci->findBlockWithProperty([&](SequenceBlockType* blockObec) {
		return blockObec->data_->manicipality == nazovObce;
		});
	auto* najdenaUlicaSvlastnostou = this->zoznamUlic->findBlockWithProperty([&](SequenceBlockType* blockObec) {
		return blockObec->data_->street == nazovUlice;
		});
	if (najdenaUlicaSvlastnostou != nullptr && najdenaObecSvlastnostou != nullptr) {
		
		poradieUlice = this->zoznamUlic->calculateIndex(*najdenaUlicaSvlastnostou);
		poradieObce = this->zoznamObci->calculateIndex(*najdenaObecSvlastnostou);
	}
	return *hierarchy.accessRoot()->sons_->access(poradieObce)->data_->sons_->access(poradieUlice)->data_;
}


