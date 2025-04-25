#include "DruhaCast.h"
#include "Algorithm.h"

DruhaCast::DruhaCast(const Data& other) : Data(other)
{
	this->vytvorHierarchiu();	
}

void DruhaCast::vytvorHierarchiu()
{
	auto& root = hierarchy.emplaceRoot();
	root.data_ = NodeDopravca("Britsky narodny dopravca", 0, 0);

	HierarchyBlockType lastObec = nullptr;
	HierarchyBlockType lastUlica = nullptr;

	for (auto it = zoznamZastavok->begin(); it != zoznamZastavok->end(); ++it)
	{
		const std::string& obec = (*it).manicipality;
		const std::string& ulica = (*it).street;

		if (!lastObec || lastObec->data_.dataNode_ != obec)
		{
			auto& novaObec = hierarchy.emplaceSon(root, root.sons_->size());
			novaObec.data_ = NodeDopravca(obec, 1, root.sons_->size());
			lastObec = &novaObec;

			auto& novaUlica = hierarchy.emplaceSon(novaObec, 0);
			novaUlica.data_ = NodeDopravca(ulica, 2, 0);
			lastUlica = &novaUlica;
		}
		else if (!lastUlica || lastUlica->data_.dataNode_ != ulica)
		{
			auto& novaUlica = hierarchy.emplaceSon(*lastObec, lastObec->sons_->size());
			novaUlica.data_ = NodeDopravca(ulica, 2, lastObec->sons_->size());
			lastUlica = &novaUlica;
		}


		lastUlica->data_.getZastavky()->insertLast().data_ = (*it);
	}
}

void DruhaCast::IteratorInterface()
{
	auto iteratorDopredu = DopravcaIterator(&hierarchy, hierarchy.accessRoot());
	this->VypisAktualnuPoziciuIteratora(*iteratorDopredu);
	while (true) {

		this->VypisSynovNaAktualnejPozicii(iteratorDopredu);

		if (this->VstupOdUzivatela(iteratorDopredu)) {
			break;
		}

		this->VypisAktualnuPoziciuIteratora(*iteratorDopredu);
	}
}

void DruhaCast::VypisAktualnuPoziciuIteratora(NodeDopravca& dp)
{
	std::cout << "Aktualna pozicia: \x1B[32m" + dp.toString(dp.poradieNode_) << "\033[0m" << std::endl;
}

void DruhaCast::VypisSynovNaAktualnejPozicii(DopravcaIterator& dp)
{
	if ((*dp).indexUrovne_ != 2)
	{
		size_t indexSyna = 0;
		std::cout << "Synovia aktualnej pozicie dopravcu: \n";

		for (auto it = dp.dajSynov()->begin(); it != dp.dajSynov()->end(); ++it) {
			std::cout << (*it)->data_.toString(indexSyna) << std::endl;
			++indexSyna;
		}
	}
	else
	{
		size_t indexSyna = 0;
		std::cout << "Zastavky na aktualnej ulici: \n";

		VypisZastavkyNaAkutalnejPozicii(*dp);
	}
}

void DruhaCast::VypisZastavkyNaAkutalnejPozicii(NodeDopravca& dp)
{
	size_t indexZastavky = 0;
	auto zastavky = dp.getZastavky();
	if (zastavky != nullptr)
	{
		for (auto it = zastavky->begin(); it != zastavky->end(); ++it) {
			std::cout << (*it).FullNameBusStop() << std::endl;
			indexZastavky;
		}
	}
	else
	{
		std::cout << "Na tejto ulice nie su ziadne zastavky\n";
	}
}

void DruhaCast::FiltrujZoznamZastavok(DopravcaIterator& it)
{
	auto itHierarchyBegin = it;
	auto itHierarchyEnd = DopravcaIterator(&hierarchy, nullptr);
	using VysledokHladaniaVHierarchii = ds::amt::ImplicitSequence<NodeDopravca>;
	using VysledokHladaniaVSekvencii = ds::amt::ImplicitSequence<Dopravca>;
	VysledokHladaniaVHierarchii vysledokH;
	VysledokHladaniaVSekvencii vysledokS;
	char uroven;
	std::cout << "Chcete filtrovat podla: [u]lice, [z]astavky\n";
	std::cin >> uroven;
	std::function<bool(const NodeDopravca&)> jeNaUlici = [&](const NodeDopravca& zastavka) -> bool {return toLowerCase(zastavka.dataNode_).find(this->castNazvuUice) != std::string::npos;};
	std::function<bool(const Dopravca&)> jeLatitudeVacsia = [&](const Dopravca& z) {return z.latitude > this->minLat;};
	std::function<bool(const Dopravca&)> jeLatitudeMensia = [&](const Dopravca& z) {return z.latitude < this->maxLat;};
	std::function<bool(const Dopravca&)> jeLatitudeRovna = [&](const Dopravca& z) {return z.latitude == this->minLat;};
	std::function<bool(const Dopravca&)> jeLatitudeVacsiarovna = [&](const Dopravca& z) {return z.latitude >= this->minLat;};
	std::function<bool(const Dopravca&)> jeLatitudeMensiarovna = [&](const Dopravca& z) {return z.latitude <= this->maxLat;};
	std::function<bool(const Dopravca&)> jeLongitudeVacsia = [&](const Dopravca& z) {return z.longitude > this->minLong;};
	std::function<bool(const Dopravca&)> jeLongitudeMensia = [&](const Dopravca& z) {return z.longitude < this->maxLong;};
	std::function<bool(const Dopravca&)> jeLongitudeRovna = [&](const Dopravca& z) {return z.longitude == this->minLong;};
	std::function<bool(const Dopravca&)> jeLongitudeVacsiarovna = [&](const Dopravca& z) {return z.longitude >= this->minLong;};
	std::function<bool(const Dopravca&)> jeLongitudeMensiarovna = [&](const Dopravca& z) {return z.longitude <= this->maxLong;};


	std::function<bool(const Dopravca&)> pouzivanyFilterVSekvencii;
	std::function<bool(const NodeDopravca&)> pouzivanyFilterVHierarchii;
	std::string operatorPorovnania;
	char definiciaDlzkySirky;
	if (uroven == 'u')
	{
		std::cout << "Zadajte cast nazvu ulice: ";
		std::cin >> this->castNazvuUice;
		this->castNazvuUice = toLowerCase(this->castNazvuUice);
		pouzivanyFilterVHierarchii = jeNaUlici;
		Algorithm::findAndInsertElement(
			itHierarchyBegin,
			itHierarchyEnd,
			pouzivanyFilterVHierarchii,
			vysledokH,
			[&](ds::amt::ImplicitSequence<NodeDopravca>& vystupnyZoznam, NodeDopravca& hodnota) {
				vystupnyZoznam.insertLast().data_ = hodnota;
			}
		);
		for (size_t i = 0; i < vysledokH.size(); i++)
		{
			vysledokH.access(i)->data_.vypisZastavky();
		};
	}
	else
	{
		std::cout << "Zadajte operator porovnania(<,>,=, >=, <=): ";
		std::cin >> operatorPorovnania;
		std::cout << "[S]irka/[V]yska";
		std::cin >> definiciaDlzkySirky;
		if (definiciaDlzkySirky == 's') {
			if (operatorPorovnania == "<") {
				std::cout << "Zadajte maximalnu sirku: ";
				std::cin >> this->maxLat;
				pouzivanyFilterVSekvencii = jeLatitudeMensia;
			}
			else if (operatorPorovnania == ">") {
				std::cout << "Zadajte minimalnu sirku: ";
				std::cin >> this->minLat;
				pouzivanyFilterVSekvencii = jeLatitudeVacsia;
			}
			else if (operatorPorovnania == "=") {
				std::cout << "Zadajte sirku rovnu: ";
				std::cin >> this->minLat;
				this->maxLat = this->minLat;
				pouzivanyFilterVSekvencii = jeLatitudeRovna;
			}
			else if (operatorPorovnania == ">=") {
				std::cout << "Zadajte sirku vacsiu alebo rovnu: ";
				std::cin >> this->minLat;
				pouzivanyFilterVSekvencii = jeLatitudeVacsiarovna;
			}
			else if (operatorPorovnania == "<=") {
				std::cout << "Zadajte sirku mensiu alebo rovnu: ";
				std::cin >> this->maxLat;
				pouzivanyFilterVSekvencii = jeLatitudeMensiarovna;
			}
		}
		else {
			if (operatorPorovnania == "<") {
				std::cout << "Zadajte maximalnu dlzku: ";
				std::cin >> this->maxLong;
				pouzivanyFilterVSekvencii = jeLongitudeMensia;
			}
			else if (operatorPorovnania == ">") {
				std::cout << "Zadajte minimalnu dlzku: ";
				std::cin >> this->minLong;
				pouzivanyFilterVSekvencii = jeLongitudeVacsia;
			}
			else if (operatorPorovnania == "=") {
				std::cout << "Zadajte dlzku rovnu: ";
				std::cin >> this->minLong;
				this->maxLong = this->minLong;
				pouzivanyFilterVSekvencii = jeLongitudeRovna;
			}
			else if (operatorPorovnania == ">=") {
				std::cout << "Zadajte dlzku vacsiu alebo rovnu: ";
				std::cin >> this->minLong;
				pouzivanyFilterVSekvencii = jeLongitudeVacsiarovna;
			}
			else if (operatorPorovnania == "<=") {
				std::cout << "Zadajte dlzku mensiu alebo rovnu: ";
				std::cin >> this->maxLong;
				pouzivanyFilterVSekvencii = jeLongitudeMensiarovna;
			}
		}
		Algorithm::findAndInsertElement(
			(*it).getZastavky()->begin(),
			(*it).getZastavky()->end(),
			pouzivanyFilterVSekvencii,
			vysledokS,
			[&](ds::amt::ImplicitSequence<Dopravca>& vystupnyZoznam, Dopravca hodnota) {
				vystupnyZoznam.insertLast().data_ = hodnota;
			}
		);
		for (size_t i = 0; i < vysledokH.size(); i++)
		{
			vysledokH.access(i)->data_.vypisZastavky();
		};
	}
}

bool DruhaCast::VstupOdUzivatela(DopravcaIterator& it)
{
	std::string vstup;
	std::cout << "Zadaj moznost [u] -- postup hore, [s] (index) -- zvol syna na indexe, [f] -- filtruj zastavky, [q] -- koniec:\n";
	std::cin >> vstup;
	std::cin.ignore(256, '\n');

	if (vstup == "u") {
		it.chodNaRodica();
	}
	else if (vstup == "s") {
		std::string poradieStr;
		std::cin >> poradieStr;
		try {
			it.chodNaSyna(std::stoi(poradieStr));
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
