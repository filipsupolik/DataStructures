#include "DruhaCast.h"
#include "Algorithm.h"

DruhaCast::DruhaCast(const Data& other) : Data(other)
{
	this->vytvorHierarchiu();	
}

void DruhaCast::vytvorHierarchiu()
{
    hierarchy.emplaceRoot().data_ = NodeDopravca();
    hierarchy.accessRoot()->data_.dataNode_ = "Britsky narodny dopravca";
	auto* lastInserted = hierarchy.accessRoot();
    for (auto it = this->zoznamZastavok->begin(); it != this->zoznamZastavok->end(); it++)
    {
        if (lastInserted->sons_ == nullptr || hierarchy.accessSon(*hierarchy.accessRoot(), hierarchy.accessRoot()->sons_->size())->data_.dataNode_ != (*it).manicipality)
        {
			auto& addedNodeObec = hierarchy.emplaceSon(*hierarchy.accessRoot(), hierarchy.accessRoot()->sons_->size());
			addedNodeObec.data_ = NodeDopravca((*it).manicipality, 1, hierarchy.accessRoot()->sons_->size());
			auto& addedNodeUlica = hierarchy.emplaceSon(addedNodeObec, addedNodeObec.sons_->size());
			addedNodeUlica.data_ = NodeDopravca((*it).street, 2, addedNodeObec.sons_->size());
			lastInserted = &addedNodeUlica;
        }
        else if (lastInserted->data_.dataNode_ != (*it).street)
        {
			auto& parent = *hierarchy.accessParent(*lastInserted);
			auto& addedNodeUlica = hierarchy.emplaceSon(parent, parent.sons_->size());
			addedNodeUlica.data_ = NodeDopravca((*it).street, 2, parent.sons_->size());
        }
		lastInserted->data_.zastavky.insertLast().data_ = *it;
    }
}

void DruhaCast::IteratorInterface()
{
	auto iteratorDopredu = DopravcaIterator(&hierarchy, hierarchy.accessRoot());
	this->VypisAktualnuPoziciuIteratora(*iteratorDopredu);
	while (true) {
		
		this->VypisSynovNaAktualnejPozicii(iteratorDopredu.dajSynov());

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

void DruhaCast::VypisSynovNaAktualnejPozicii(ds::amt::IS<HierarchyBlockType>* sons)
{
	size_t indexSyna = 0;
	std::cout << "Synovia aktualnej pozicie dopravcu: \n";

	for (auto it = sons->begin(); it != sons->end(); ++it) {
		std::cout << (*it)->data_.toString(indexSyna) << std::endl;
		++indexSyna;
	}
}

void DruhaCast::FiltrujZoznamZastavok(DopravcaIterator& it)
{
	auto itHierarchyBegin = it;
	auto itHierarchyEnd = DopravcaIterator(&hierarchy, nullptr);
	using VysledokHladaniaVHierarchii = ds::amt::ImplicitSequence<NodeDopravca&>;
	using VysledokHladaniaVSekvencii = ds::amt::ImplicitSequence<Dopravca&>;
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
			[&](ds::amt::ImplicitSequence<NodeDopravca&>& vystupnyZoznam, NodeDopravca& hodnota) {
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
			(*it).getZastavky().begin(),
			(*it).getZastavky().end(),
			pouzivanyFilterVSekvencii,
			vysledokS,
			[&](ds::amt::ImplicitSequence<Dopravca&>& vystupnyZoznam, Dopravca hodnota) {
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
