#include "DruhaCast.h"
#include "Algorithm.h"

DruhaCast::DruhaCast(const Data& other) : Data(other)
{
	this->vytvorHierarchiu();	
}

void DruhaCast::vytvorHierarchiu()
{
	hierarchy.emplaceRoot().data_ = new Dopravca();
	hierarchy.accessRoot()->data_->manicipality = "Britsky narodny dopravca";

	NacitajObce();
	NacitajUlice();
	NacitajZastavky();
}

void DruhaCast::NacitajObce()
{
	size_t indexPridanejObce = 0;
	for (auto it = this->zoznamObci->begin(); it != this->zoznamObci->end(); ++it)
	{
		Dopravca* obec = (*it);
		auto& novyVrcholObec = hierarchy.emplaceSon(*hierarchy.accessRoot(), hierarchy.accessRoot()->sons_->size());
		novyVrcholObec.data_ = obec;
		indexPridanejObce++;
		obec->indexObce = indexPridanejObce;
	}
}

void DruhaCast::NacitajUlice()
{
	size_t indexPridanejUlice = 0;
	for (auto it = this->zoznamUlic->begin(); it != this->zoznamUlic->end(); ++it)
	{
		Dopravca* ulica = (*it);
		auto& aktualnaObec = DajObec(ulica->manicipality);
		auto& vrcholUlica = hierarchy.emplaceSon(aktualnaObec, aktualnaObec.sons_->size());
		vrcholUlica.data_ = ulica;
		ulica->indexUlice = indexPridanejUlice;
	}
}

void DruhaCast::NacitajZastavky()
{
	size_t poradiePridania = 0;
	for (auto it = this->zoznamZastavok->begin(); it != this->zoznamZastavok->end(); ++it)
	{
		Dopravca* zastavka = (*it);
		auto& aktualnaObec = DajObec(zastavka->manicipality);
		auto& aktualnaUlica = DajUlicu(zastavka->manicipality, zastavka->street);
		if (aktualnaUlica.data_->street == zastavka->street)
		{
			auto& vrcholZastavka = hierarchy.emplaceSon(aktualnaUlica, aktualnaUlica.sons_->size());
		}
		zastavka->indexZastavky = poradiePridania;
		poradiePridania++;
	}
}

DruhaCast::HierarchyBlockType& DruhaCast::DajObec(std::string nazovObce)
{
	size_t obecIndex = 0;
	auto* obecSons = hierarchy.accessRoot()->sons_;
	HierarchyBlockType* obec = nullptr;
	for (obecIndex = 0; obecIndex < obecSons->size(); ++obecIndex) {
		obec = obecSons->access(obecIndex)->data_;

		if (obec->data_->manicipality == nazovObce) {
			break;
		}
	}
	return *obec;
}

DruhaCast::HierarchyBlockType& DruhaCast::DajUlicu(std::string nazovObce,std::string nazovUlice)
{
	int obecIndex = 0;
	int ulicaIndex = 0;

	auto* rootSons = hierarchy.accessRoot()->sons_;
	for (auto it = rootSons->begin(); it != rootSons->end(); ++it, ++obecIndex) {
		const auto* rootSon = (*it)->data_;

		if (rootSon->manicipality == nazovObce) {
			break;
		}
	}

	auto* obecSons = hierarchy.accessRoot()->sons_->access(obecIndex)->data_->sons_;
	HierarchyBlockType* ulica = nullptr;
	for (ulicaIndex = 0; ulicaIndex < obecSons->size(); ++ulicaIndex) {
		ulica = obecSons->access(ulicaIndex)->data_;

		if (ulica->data_->street == nazovUlice) {
			break;
		}
	}
	return *ulica;
}

void DruhaCast::IteratorInterface()
{
	auto iteratorDopredu = hierarchy.beginPre();
	this->VypisAktualnuPoziciuIteratora(*iteratorDopredu);
	while (true) {
		this->VypisSynovNaAktualnejPozicii(iteratorDopredu.dajBlockType().sons_);

		if (this->VstupOdUzivatela(iteratorDopredu)) {
			break;
		}

		this->VypisAktualnuPoziciuIteratora(*iteratorDopredu);
	}
}

void DruhaCast::VypisAktualnuPoziciuIteratora(Dopravca* dp)
{
	if (dp->street == "")
	{
		std::cout << "Aktualna pozicia: \x1B[32m" + dp->toString(dp->indexObce) << "\033[0m" << std::endl;
	}
	else
	{
		std::cout << "Aktualna pozicia: \x1B[32m" + dp->toString(dp->indexUlice) << "\033[0m" << std::endl;
	}
}

void DruhaCast::VypisSynovNaAktualnejPozicii(ds::amt::IS<HierarchyBlockType*>* sons)
{
	size_t indexSyna = 0;
	std::cout << "Synovia aktualnej pozicie dopravcu: \n";

	for (auto it = sons->begin(); it != sons->end(); ++it) {
		std::cout << (*it)->data_->toString(indexSyna) << std::endl;
		++indexSyna;
	}
}

void DruhaCast::FiltrujZoznamZastavok(ds::amt::Hierarchy<HierarchyBlockType>::PreOrderHierarchyIterator& it)
{
	using Vysledok = ds::amt::ImplicitSequence<Dopravca*>;
	Vysledok vysledok;
	char vstupZKlavesnice = this->vstupZKlavesnice();
	zadajZoznamParametrov(vstupZKlavesnice);
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
	switch (vstupZKlavesnice)
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

	auto itBegin = ds::amt::MultiWayExplicitHierarchy<Dopravca*>::PreOrderHierarchyIterator(&hierarchy, &it.dajBlockType());
	auto itEnd = ds::amt::MultiWayExplicitHierarchy<Dopravca*>::PreOrderHierarchyIterator(&hierarchy, nullptr);

	Algorithm::findAndInsertElement(
		itBegin,
		itEnd,
		pouzivanyFilter,
		vysledok,
		[&]( auto& vystupnyZoznam, auto hodnota) {
			vystupnyZoznam.insertLast().data_ = hodnota;
		}
	);
}

bool DruhaCast::VstupOdUzivatela(ds::amt::Hierarchy<HierarchyBlockType>::PreOrderHierarchyIterator& it)
{
	std::string vstup;
	std::cout << "Zadaj moznost [u] -- postup hore, [s] (index) -- zvol syna na indexe, [f](argument) (predicate) -- filtruj zastavky, [q] -- koniec:\n";
	std::cin >> vstup;
	std::cin.ignore(256, '\n');

	if (vstup == "u") {
		--it;
	}
	else if (vstup == "s") {
		std::string poradieStr;
		std::cin >> poradieStr;
		try {
			it += std::stoi(poradieStr);
		}
		catch (const std::exception& exc) {
			std::cout << "\x1B[31mArgument nie je cislo\033[0m\n";
		}
		std::cin.ignore(256, '\n');
	}
	else if (vstup == "f") {
		this->FiltrujZoznamZastavok(it);
	}
	else if (vstup == "q") {
		std::cout << "\x1B[31mKoniec\033[0m\n";
		return true;
	}
	else {
		std::cout << "\x1B[31mNeznama moznost\033[0m\n";
	}

	std::cout << "\n";
	return false;
}


