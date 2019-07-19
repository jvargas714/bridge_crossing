#ifndef _BRIDGECROSSINGPROCESSOR_H
#define _BRIDGECROSSINGPROCESSOR_H
#include <memory>
#include <vector>
#include "BridgeCrossingHandler.h"
#include "hiker.h"

struct yaml_event_s;

enum class PARSER_STATUS {
	BEGIN,
	PARSING_EVENT_TYPE,
	PARSING_EVENT_CROSSING,
	PARSING_EVENT_ENCOUNTER,
	PARSING_HIKERS,
	ERROR,
	END
};

enum class PARSER_SUB_STATUS {
	CLEAR,
	PARSING_HIKER_SPEED,
	PARSING_HIKER_NAME,
	PARSING_CROSSING_LENGTH
};

struct ParserState {
	ParserState():
	m_pstatus(PARSER_STATUS::BEGIN),
	m_subStatus(PARSER_SUB_STATUS::CLEAR) {}

	PARSER_STATUS m_pstatus;
	PARSER_SUB_STATUS m_subStatus;
};

class BridgeCrossingProcessor {
	std::string m_cfgFileName;
	ParserState m_state;
	std::vector<Hiker> m_hikers;
	std::shared_ptr<BridgeCrossingHandler> m_crossingHandler;

	bool processYamlEvent(yaml_event_s& evt);
	bool eventValueIs(const yaml_event_s& evt, const std::string& val);
	void processEventType(yaml_event_s &s);
	void processEncounterEvent(yaml_event_s &evt);
	void processCrossingEvent(yaml_event_s &evt);
	void processHikerData(yaml_event_s &evt);

public:
	BridgeCrossingProcessor()=delete;
	explicit BridgeCrossingProcessor(const std::string& cfgFileName);
	explicit BridgeCrossingProcessor(const std::string& cfgFileName, std::shared_ptr<BridgeCrossingHandler> hdlr);
	~BridgeCrossingProcessor()=default;
	inline void setCfgFile(const std::string& filename) { m_cfgFileName = filename; }
	bool processCrossings();
	void addHiker(const Hiker& hkr);
	void addHiker(const std::string& name, int speed);
	void saveCrossingReport() const { m_crossingHandler->saveMasterCrossingReport(); }
};
#endif //_BRIDGECROSSINGPROCESSOR_H
