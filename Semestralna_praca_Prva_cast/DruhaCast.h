#pragma once
#include <libds/amt/explicit_hierarchy.h>
#include "Data.h"

class DruhaCast : public Data
{
private:
	using Hierarchy = ds::amt::MultiWayExplicitHierarchy<NodeDopravca>;
	using HierarchyBlockType = ds::amt::MultiWayExplicitHierarchyBlock<NodeDopravca>*;
	Hierarchy hierarchy;
	class DopravcaIterator : public Hierarchy::PreOrderHierarchyIterator
	{
	public:
		DopravcaIterator(Hierarchy* hierarchy, HierarchyBlockType userCurrentNode) :
			Hierarchy::PreOrderHierarchyIterator(hierarchy, userCurrentNode)
		{

		}

		auto chodNaSyna(size_t poradieSyna)
		{
			for(auto son: *this->currentPosition_->currentNode_->sons_)
			{
				if (son->data_.poradieNode_ == poradieSyna)
				{
					this->currentPosition_->currentNode_ = son;
					break;
				}
			}
			return this->currentPosition_->currentNode_;
		};

		auto chodNaRodica()
		{
			if (this->currentPosition_->currentNode_ != this->hierarchy_->accessRoot()) {
				this->currentPosition_->currentNode_ = this->hierarchy_->accessParent(*this->currentPosition_->currentNode_);
			}
			return this->currentPosition_->currentNode_;
		}

		auto dajSynov()
		{
			if (this->currentPosition_->currentNode_->data_.indexUrovne_ < 2)
			{
				return this->currentPosition_->currentNode_->sons_;
			}
			else
			{
				std::cout << "Nevies ziskat synov aktualnej pozicie";
			}
		}

		auto dajZastavkyNaUlici()
		{
			return this->currentPosition_->currentNode_->data_.getZastavky();
		}
	};

public:
	DruhaCast(const Data& data);
	void vytvorHierarchiu();
	void IteratorInterface();
	void VypisAktualnuPoziciuIteratora(NodeDopravca& dp);
	void VypisSynovNaAktualnejPozicii(DopravcaIterator& dp);
	void VypisZastavkyNaAkutalnejPozicii(NodeDopravca& dp);
	void FiltrujZoznamZastavok(DopravcaIterator& it);
	bool VstupOdUzivatela(DopravcaIterator& it);
};

