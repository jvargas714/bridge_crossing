#ifndef SCALA_COMPUTING_BRIDGECROSSINGHANDLER_H
#define SCALA_COMPUTING_BRIDGECROSSINGHANDLER_H
#include <cstdint>
#include <vector>
#include <string>

/*
 * Base implementation of this handler will implement the fastest time for each crossing given the hiker team
 *
 * This Handler if for implementing the Bridge Crossing algorithm and generating a report of th results found.
 *
 *  generateReport() can be overridden to write to a file, output to stdout, or send via networking interface, or
 *  even database
 *
 *  crossBridge(...) can also be overriden if this handler is to be extended implementing a different experimental
 *  implementation of this algorithm.
 */

class Hiker;

class BridgeCrossingHandler {
protected:
	uint64_t totalNumberOfCrossing;

	// total time of all crossings processed by this handler
	double totalTime;

	// each time a crossing occurs a report is generated, index 0: represents first crossing, index 1: second crossing ....
	std::vector<std::string> crossingReports;
	static const std::string genOutFileName();

public:
	BridgeCrossingHandler(): totalNumberOfCrossing(0), totalTime(0.0) { }
	virtual ~BridgeCrossingHandler()=default;
	virtual std::string generateReport(const std::vector<Hiker>& hikers, int length, float timeTaken) const;
	void saveCrossingReport(const std::vector<Hiker>& hikers, int length, float timeTaken);
	virtual void crossBridge(std::vector<Hiker>& hikers, int crossingLength);
	virtual bool saveMasterCrossingReport() const;
};
#endif //SCALA_COMPUTING_BRIDGECROSSINGHANDLER_H
