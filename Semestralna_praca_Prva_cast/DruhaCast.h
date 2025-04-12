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
	HierarchyBlockType& dajObec(std::string nazovObce);
	HierarchyBlockType& dajUlicu(std::string nazovObce, std::string nazovUlice);
};

