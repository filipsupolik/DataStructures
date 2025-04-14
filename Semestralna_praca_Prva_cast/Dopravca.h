#pragma once
#include <string>
class Dopravca
{
public:
	std::string street;
	std::string manicipality;
	size_t indexObce;
	size_t indexUlice;
	size_t indexZastavky;
	double latitude;
	double longitude;
	int stopId;
	Dopravca()
	{
		this->indexObce = -1;
		this->indexUlice = -1;
		this->indexZastavky = -1;
		this->street = "";
		this->manicipality = "";
		this->latitude = 0.0;
		this->longitude = 0.0;
		this->stopId = 0;
	}

	Dopravca(const Dopravca& other)
	{
		this->indexObce = other.indexObce;
		this->indexUlice = other.indexUlice;
		this->stopId = other.stopId;
		this->manicipality = other.manicipality;
		this->street = other.street;
		this->latitude = other.latitude;
		this->longitude = other.longitude;
	};

	std::string FullNameBusStop()
	{
		return	+"\nStopId:\t\t" + std::to_string(this->stopId)
				+ "\nObec:\t" + this->manicipality
				+ "\nUlica:\t" + this->street
				+ "\nZemepisna Sirka:\t" + std::to_string(this->latitude)
				+ "\nZemepisna Dlzka:\t\t" + std::to_string(this->longitude);
	};

	std::string toString(size_t index = -1) const 
	{
		std::string vystup = " ";
		if (this->indexObce != -1)
		{
			return vystup + std::to_string(index) + ": \x1B[36m" + this->manicipality+ "\033[0m";
		}
		else if (this->indexUlice != -1)
		{
			return vystup + std::to_string(index) + ": \x1B[36m" + this->street + "\033[0m";
		}
		else
		{
			return vystup + std::to_string(index) + ": \x1B[36m" + this->manicipality + "\033[0m";
		}
	}
};