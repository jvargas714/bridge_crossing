#include "hiker.h"

bool Hiker::operator < (const Hiker &hkr) {
	return this->speed < hkr.speed;
}

bool Hiker::operator > (const Hiker &hkr) {
	return this->speed > hkr.speed;
}

bool Hiker::operator==(const Hiker &hkr) {
	return this->speed == hkr.speed &&
	this->name == hkr.name;
}

std::ostream& operator << (std::ostream& os, const Hiker& hkr) {
	os << "name: " << hkr.name << "\nspeed: " << hkr.speed << "ft/minute\n";
	return os;
}
