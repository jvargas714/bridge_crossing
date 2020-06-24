#include "BridgeCrossingHandler.h"
#include "hiker.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <iostream>

static float calcTime(const Hiker& hkr, int crossingLength) {
	return (crossingLength / hkr.getSpeed());
}

static float _crossBridge(std::vector<Hiker> &hikers, int crossingLength, int offset) {
	if (hikers.empty()) return 0;
	size_t n = hikers.size() - offset;
	if (n == 3)
		return calcTime(hikers[2], crossingLength) +
		calcTime(hikers[0], crossingLength) +
		calcTime(hikers[1], crossingLength);
	else if (n == 2)
		return calcTime(hikers[1], crossingLength);
	else if (n == 1)
		return calcTime(hikers[0], crossingLength);

	//	take best outcome between these two scenario, the recursive work on the next n-2 people
	// t_0 --> t1 + 2t2 + tN    --> case where second fastest comes back to escort fastest and slowest
	// t_1 --> 2t1 + tN−1 + tN  --> case where fastest escorts persons n-1 and n-2
	float t_0 = calcTime(hikers[0], crossingLength) +
		2*calcTime(hikers[1], crossingLength) +
		calcTime(hikers[n-1], crossingLength);

	float t_1 = 2 * calcTime(hikers[0], crossingLength) +
		calcTime(hikers[n-2], crossingLength) +
		calcTime(hikers[n-1], crossingLength);

	float timeTaken = std::min(t_0, t_1);

	// recursively calculate for remaining n-2 people
	timeTaken += _crossBridge(hikers, crossingLength, offset+2);
	return timeTaken;
}

void BridgeCrossingHandler::crossBridge(std::vector<Hiker> &hikers, int crossingLength) {
	std::sort(hikers.begin(), hikers.end(), std::greater<>());
	float timeTaken = _crossBridge(hikers, crossingLength, 0);
	totalNumberOfCrossing++;
	totalTime += timeTaken;
	saveCrossingReport(hikers, crossingLength, timeTaken);
}

std::string BridgeCrossingHandler::generateReport(const std::vector<Hiker>& hikers, int length, float timeTaken) const {
	std::stringstream ss;
	ss << "*=*=*=*=*=*=*=Crossing " << totalNumberOfCrossing << "*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n";
	ss << "Crossing Length: " << length << "\n\n";
	ss << "--Current Hikers--\nteam size:" << hikers.size() << "\n";
	for (const auto& hkr : hikers)
		ss << hkr << "\n";
	ss << "-----------------\n";
	ss << "Time Taken: " << timeTaken << " minutes" << "\n";
	ss << "Cumulative Time After " << totalNumberOfCrossing << " bridge crossings: " << totalTime << "\n";
	return ss.str();
}

void BridgeCrossingHandler::saveCrossingReport(const std::vector<Hiker> &hikers, int length, float timeTaken) {
	// generate a report for the crossing at the end that run
	crossingReports.push_back(generateReport(hikers, length, timeTaken));
}

const std::string BridgeCrossingHandler::genOutFileName() {
	std::stringstream ss;
	std::time_t tm = std::time(nullptr);
	ss << std::put_time(std::localtime(&tm), "CROSSING_REPORT_%H%M__%a_%b_%d_%Y.dat");  // LOG_1640__Fri_Dec_12_2018
	return ss.str();
}

bool BridgeCrossingHandler::saveMasterCrossingReport() const {
	std::string outfile = genOutFileName();
	std::ofstream of(outfile);
	if (of.is_open()) {
		std::cout << __FUNCTION__ << "(): generating crossing report in cwd. filename: " << outfile << std::endl;
		for (const auto &rep: crossingReports)
			of << rep << "\n";
	} else
		return false;
	of << "========================SUMMARY========================\n";
	of << "TOTAL CROSSINGS: " << totalNumberOfCrossing << "\n";
	of << "TOTAL ELAPSED TIME: " << totalTime << std::endl;
	of << "=======================================================\n";
	std::cout << __FUNCTION__ << "(): crossing report file successfully generated" << std::endl;
	return true;
}
