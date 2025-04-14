#pragma once
#include <libds/amt/explicit_hierarchy.h>
#include "Data.h"

class DruhaCast: public Data
{
private:
	using Hierarchy = ds::amt::MultiWayExplicitHierarchy<Dopravca*>;
	Hierarchy hierarchy;
public:
	DruhaCast(const Data& data);
	void vytvorHierarchiu();
	void NacitajObce();
	void NacitajUlice();
	void NacitajZastavky();
	HierarchyBlockType& DajObec(std::string nazovObce);
	HierarchyBlockType& DajUlicu(std::string nazovObce, std::string nazovUlice);
	void IteratorInterface();
	void VypisAktualnuPoziciuIteratora(Dopravca* dp);
	void VypisSynovNaAktualnejPozicii(ds::amt::IS<HierarchyBlockType*>* sons);
	void FiltrujZoznamZastavok(ds::amt::Hierarchy<HierarchyBlockType>::PreOrderHierarchyIterator& it);
	bool VstupOdUzivatela(ds::amt::Hierarchy<HierarchyBlockType>::PreOrderHierarchyIterator& it);
};

