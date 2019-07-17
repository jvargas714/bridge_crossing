#include <string>
#include <vector>
#include <cstdint>
#include <iostream>
#include <csignal>
#include "BridgeCrossingProcessor.h"

static std::string g_cfg_filename;

/*
 * arg: -f [yaml config filename containing crossings]
 * arg: -h [help menu]
 */

void handle_args(int argc, char** argv) {
	if (argc < 2) {
		std::cerr << "input error, must provide yaml cfg file containing bridge crossings, see -h option for details" << std::endl;
		exit(-1);
	}
	g_cfg_filename = std::string(argv[1]);
}

void handle_signal(int sig) {
	std::cerr << "signal: " << sig << std::endl;
	exit(sig);
}

int main(int argc, char** argv) {
	handle_args(argc, argv);
	std::signal(SIGABRT, handle_signal);

	// create cfg file reader instance read file in
	BridgeCrossingProcessor crossingProcessor(g_cfg_filename);
	if (crossingProcessor.processCrossings()) {
		std::cout << __FUNCTION__ << "(): yaml crossing document has been successfully processed" << std::endl;
	}
	crossingProcessor.saveCrossingReport();
	return 0;
}
