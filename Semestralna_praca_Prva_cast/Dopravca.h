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

	bool jeLatitudeVacsia(double minLat) {
		return this->latitude > minLat;
	}

	bool jeLatitudeMensia(double maxLat) {
		return this->latitude < maxLat;
	}

	bool jeLatitudeRovna(double lat) {
		return this->latitude == lat;
	}

	bool jeLatitudeVacsiarovna(double minLat) {
		return this->latitude >= minLat;
	}

	bool jeLatitudeMensiarovna(double maxLat) {
		return this->latitude <= maxLat;
	}

	bool jeLongitudeVacsia(double minLong) {
		return this->longitude > minLong;
	}

	bool jeLongitudeMensia(double maxLong) {
		return this->longitude < maxLong;
	}

	bool jeLongitudeRovna(double lon) {
		return this->longitude == lon;
	}

	bool jeLongitudeVacsiarovna(double minLong) {
		return this->longitude >= minLong;
	}

	bool jeLongitudeMensiarovna(double maxLong) {
		return this->longitude <= maxLong;
	}
};