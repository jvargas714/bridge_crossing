#ifndef _HIKER_H
#define _HIKER_H
#include <string>
#include <ostream>

class Hiker {
	float speed;
	std::string name;

public:
	Hiker(): speed(0) {}

	Hiker(const Hiker& hkr) {
		speed = hkr.speed;
		name = hkr.name;
	}
	Hiker& operator = (const Hiker& hkr) {
		speed = hkr.speed;
		name = hkr.name;
		return *this;
	}

	Hiker(int speed, std::string name): speed(speed), name(std::move(name)) { }
	inline void setSpeed(float spd) { speed = spd; }
	inline float getSpeed() const { return speed; }
	inline void setName(std::string nm) { name = std::move(nm); }
	inline void clearData() { name.clear(); speed = 0; }
	inline
	bool operator < (const Hiker& hkr);
	bool operator > (const Hiker& hkr);
	bool operator == (const Hiker& hkr);
	friend std::ostream& operator << (std::ostream& os, const Hiker& hkr);
};
#endif //_HIKER_H
