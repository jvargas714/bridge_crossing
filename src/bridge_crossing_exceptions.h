#ifndef _BRIDGE_CROSSING_EXCEPTIONS_H
#define _BRIDGE_CROSSING_EXCEPTIONS_H
#include <exception>
#include <stdexcept>

constexpr char INVALID_CONF_ERR_MSG[] = "Invalid Configuration File";
constexpr char FAILED_PARSER_INIT[] = "Failed to initialize the YAML parser";
constexpr char FAILED_TO_OPEN_FILE[] = "Failed to open file";

class InvalidConfiguration : public std::runtime_error {
public:
	InvalidConfiguration(): runtime_error(INVALID_CONF_ERR_MSG) {};
	explicit InvalidConfiguration(const std::string& errMsg): std::runtime_error(errMsg){}
};

class FailedParserInit : public std::runtime_error {
public:
	FailedParserInit(): runtime_error(FAILED_PARSER_INIT) {}
	explicit FailedParserInit(const std::string& errMsg): std::runtime_error(errMsg){}
};

class FailedToOpenFile : public std::runtime_error {
public:
	FailedToOpenFile(): runtime_error(FAILED_TO_OPEN_FILE) {}
	explicit FailedToOpenFile(const std::string& errMsg): std::runtime_error(errMsg){}
};
#endif //_BRIDGE_CROSSING_EXCEPTIONS_H
