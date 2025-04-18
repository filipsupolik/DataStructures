#pragma once
#include <string>
class Dopravca
{
public:
	std::string street;
	std::string manicipality;
	double latitude;
	double longitude;
	int stopId;
	Dopravca()
	{
		this->street = "";
		this->manicipality = "";
		this->latitude = 0.0;
		this->longitude = 0.0;
		this->stopId = 0;
	}

	Dopravca(const Dopravca& other)
	{
		this->stopId = other.stopId;
		this->manicipality = other.manicipality;
		this->street = other.street;
		this->latitude = other.latitude;
		this->longitude = other.longitude;
	};

	std::string FullNameBusStop() const
	{
		return	"\nStopId:\t\t" + std::to_string(this->stopId)
				+ "\nObec:\t" + this->manicipality
				+ "\nUlica:\t" + this->street
				+ "\nZemepisna Sirka:\t" + std::to_string(this->latitude)
				+ "\nZemepisna Dlzka:\t\t" + std::to_string(this->longitude);
	};
};