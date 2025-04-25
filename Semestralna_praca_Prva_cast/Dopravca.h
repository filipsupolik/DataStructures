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
		const std::string green = "\033[32m";
		const std::string reset = "\033[0m";

		std::ostringstream oss;
		oss << green;
		oss << std::left
			<< std::setw(22) << "StopId:" << this->stopId << "\n"
			<< std::setw(22) << "Obec:" << this->manicipality << "\n"
			<< std::setw(22) << "Ulica:" << this->street << "\n"
			<< std::setw(22) << "Zemepisna Sirka:" << this->latitude << "\n"
			<< std::setw(22) << "Zemepisna Dlzka:" << this->longitude << "\n";
		oss << reset;

		return oss.str();
	}
};