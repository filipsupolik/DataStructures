#pragma once
#include <string>
#include <libds/amt/implicit_sequence.h>

class NodeDopravca
{
public:
	std::string dataNode_;
	size_t indexUrovne_;
	size_t poradieNode_;
	ds::amt::ImplicitSequence<Dopravca>* zastavky;
	NodeDopravca() : dataNode_(""), indexUrovne_(0),poradieNode_(0) 
	{
		this->zastavky = new ds::amt::ImplicitSequence<Dopravca>();
	}
	NodeDopravca(const std::string& dataNode, int indexUrovne, int poradieNode)
		: dataNode_(dataNode), indexUrovne_(indexUrovne), poradieNode_(poradieNode)
	{
		zastavky = new ds::amt::ImplicitSequence<Dopravca>();
	}
	NodeDopravca(const NodeDopravca& other)
		: dataNode_(other.dataNode_), indexUrovne_(other.indexUrovne_), poradieNode_(other.poradieNode_)
	{
		zastavky = new ds::amt::ImplicitSequence<Dopravca>(*other.zastavky);
	}

	NodeDopravca& operator=(const NodeDopravca& other)
	{
		if (this != &other)
		{
			dataNode_ = other.dataNode_;
			indexUrovne_ = other.indexUrovne_;
			poradieNode_ = other.poradieNode_;

			delete zastavky;
			zastavky = new ds::amt::ImplicitSequence<Dopravca>(*other.zastavky);
		}
		return *this;
	}

	~NodeDopravca()
	{
		delete this->zastavky;
	}
	ds::amt::ImplicitSequence<Dopravca>* getZastavky(){return this->zastavky;}
	std::string toString(size_t index)
	{
		std::string vystup = " ";
		return vystup + std::to_string(index + 1) + ": \x1B[36m" + dataNode_ + "\033[0m";
	}

	void vypisZastavky()
	{
		for (auto it = this->zastavky->begin(); it != this->zastavky->end(); ++it)
		{
			std::cout << (*it).FullNameBusStop() << std::endl;
		}
	}

	bool operator==(const NodeDopravca& other) const
	{
		return dataNode_ == other.dataNode_
			&& indexUrovne_ == other.indexUrovne_
			&& poradieNode_ == other.poradieNode_;
	}

	
};