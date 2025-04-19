#pragma once
#include <string>
#include <libds/amt/implicit_sequence.h>

class NodeDopravca
{
public:
	std::string dataNode_;
	size_t indexUrovne_;
	size_t poradieNode_;
	ds::amt::ImplicitSequence<Dopravca> zastavky;
	NodeDopravca() : dataNode_(""), indexUrovne_(0),poradieNode_(0) {}
	NodeDopravca(const std::string& dataNodeP, size_t indexUrovneP, size_t poradieNodeP) : dataNode_(dataNodeP), indexUrovne_(indexUrovneP), poradieNode_(poradieNodeP) {}
	NodeDopravca(const NodeDopravca& other) : dataNode_(other.dataNode_), indexUrovne_(other.indexUrovne_), poradieNode_(other.poradieNode_), zastavky(other.zastavky) {}
	~NodeDopravca() {}
	ds::amt::ImplicitSequence<Dopravca> getZastavky(){return this->zastavky;}
	std::string toString(size_t index)
	{
		std::string vystup = " ";
		return vystup + std::to_string(index) + ": \x1B[36m" + dataNode_ + "\033[0m";
	}

	void vypisZastavky()
	{
		for (auto it = this->zastavky.begin(); it != this->zastavky.end(); ++it)
		{
			std::cout << (*it).FullNameBusStop() << std::endl;
		}
	}


};