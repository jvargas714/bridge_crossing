#include <iostream>
#include <cassert>
#include <functional>
#include "BridgeCrossingProcessor.h"
#include "BridgeCrossingHandler.h"
#include "bridge_crossing_exceptions.h"

const std::string default_crossings = "../cfg/default_crossings.yaml";
const std::string invalid_cfg = "../cfg/test_invalid_cfg.yaml";
const std::string empty_cfg = "../cfg/test_empty_cfg.yaml";
const std::string large_cfg = "../cfg/test_large_cfg.yaml";

template<class T>
void display(const T& t) {
	for (const auto& el : t)
		std::cout << el << "\n";
}

void testInvalidConfig() {
	BridgeCrossingProcessor proc1(invalid_cfg);
	assert(proc1.processCrossings());

	// report should be generated but no data, total time would be zero
	proc1.saveCrossingReport();
}

void testEmptyConfig() {
	BridgeCrossingProcessor proc1(empty_cfg);
	assert(proc1.processCrossings());

	// report should be generated but no data, total time would be zero
	proc1.saveCrossingReport();
}

// expect failed to open file message
void testNoFile() {
	BridgeCrossingProcessor proc("");
	try {
		proc.processCrossings();
	} catch(const std::runtime_error& err) {
		std::cerr << err.what() << std::endl;
	}
}

void parseLargeCfg() {
	BridgeCrossingProcessor proc(large_cfg);
	proc.processCrossings();
	proc.saveCrossingReport();
}

// test < >, and == compare operators, << as well
void testHikers() {
	std::vector<Hiker> hkrs = {
	Hiker(2345, "1"),
	Hiker(100, "1"),
	Hiker(555, "1"),
	Hiker(66, "1"),
	Hiker(99, "1") };
	Hiker clone(hkrs[0]);
	std::sort(hkrs.begin(), hkrs.end(), std::greater<>());
	display(hkrs);
	assert(hkrs[0] == clone);
}

int main() {
	//vtestInvalidConfig();
	// testEmptyConfig();
	testNoFile();
	parseLargeCfg();
	testHikers();
	return 0;
}