#include <iostream>
#include <csignal>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <yaml.h>
#include "BridgeCrossingProcessor.h"
#include "bridge_crossing_types.h"
#include "bridge_crossing_exceptions.h"
#include "yaml.h"

#define LOG_ERR std::cerr << __FUNCTION__ << "() ::E:: "
#define ENDL std::endl

static Hiker g_currHiker;

BridgeCrossingProcessor::BridgeCrossingProcessor(const std::string &cfgFileName):
m_cfgFileName(cfgFileName) {
	m_crossingHandler = std::make_shared<BridgeCrossingHandler>();
}


BridgeCrossingProcessor::BridgeCrossingProcessor(const std::string &cfgFileName,
                                                 std::shared_ptr<BridgeCrossingHandler> hdlr)
                                                 : m_cfgFileName(cfgFileName),
                                                 m_crossingHandler(hdlr) { }

bool BridgeCrossingProcessor::processCrossings() {
	FILE* fd = fopen(m_cfgFileName.c_str(), "r");
	yaml_event_t evt;
	yaml_parser_t parser;

	/* Initialize parser and open file */
	if(fd == NULL)
		throw FailedToOpenFile(std::string(FAILED_TO_OPEN_FILE) + " " + m_cfgFileName);

	if(!yaml_parser_initialize(&parser))
		throw FailedParserInit();

	yaml_parser_set_input_file(&parser, fd);

	while (m_state.m_pstatus != PARSER_STATUS::END) {
		if (!yaml_parser_parse(&parser, &evt)) {
			break;
		}
		processYamlEvent(evt);
		if (m_state.m_pstatus == PARSER_STATUS::ERROR) {
			LOG_ERR << "parser is in error state, aborting operation" << ENDL;
			break;
		}
	}
	return (m_state.m_pstatus == PARSER_STATUS::END);
}

void BridgeCrossingProcessor::addHiker(const Hiker &hkr) {
	m_hikers.push_back(hkr);
}

void BridgeCrossingProcessor::addHiker(const std::string &name, int speed) {
	m_hikers.emplace_back(speed, name);
}

bool BridgeCrossingProcessor::processYamlEvent(yaml_event_t& evt) {
	switch(m_state.m_pstatus) {
		case PARSER_STATUS::PARSING_EVENT_TYPE:
			processEventType(evt);
			break;
		case PARSER_STATUS::PARSING_EVENT_ENCOUNTER:
			processEncounterEvent(evt);
			break;
		case PARSER_STATUS::PARSING_EVENT_CROSSING:
			processCrossingEvent(evt);
			break;
		case PARSER_STATUS::PARSING_HIKERS:
			processHikerData(evt);
			break;
		case PARSER_STATUS::BEGIN:
		case PARSER_STATUS::ERROR:
		case PARSER_STATUS::END:
			break;
	}

	switch (evt.type) {
		case YAML_SCALAR_EVENT:
			if (eventValueIs(evt, SUB_KEY_EVENT_TYPE)) {
				m_state.m_pstatus = PARSER_STATUS::PARSING_EVENT_TYPE;
			}
			break;
		case YAML_STREAM_END_EVENT:
		case YAML_DOCUMENT_END_EVENT:
			m_state.m_pstatus = PARSER_STATUS::END;
			break;

		case YAML_NO_EVENT:break;
		case YAML_STREAM_START_EVENT:break;
		case YAML_DOCUMENT_START_EVENT:break;
		case YAML_ALIAS_EVENT:break;
		case YAML_SEQUENCE_START_EVENT:break;
		case YAML_SEQUENCE_END_EVENT:break;
		case YAML_MAPPING_START_EVENT:break;
		case YAML_MAPPING_END_EVENT:break;
	}
	return true;
}


void BridgeCrossingProcessor::processEventType(yaml_event_t& evt) {
	switch(evt.type) {
		case YAML_SCALAR_EVENT:
			if (eventValueIs(evt, EVENT_ENCOUNTER_TYPE)) {
				m_state.m_pstatus = PARSER_STATUS::PARSING_EVENT_ENCOUNTER;
			} else if (eventValueIs(evt, EVENT_CROSSING_TYPE)) {
				m_state.m_pstatus = PARSER_STATUS::PARSING_EVENT_CROSSING;
			} else {
				LOG_ERR << "error parsing event type, parser is in error condition" << ENDL;
				m_state.m_pstatus = PARSER_STATUS::ERROR;
			}
			break;
		default:
			LOG_ERR << "unknown event type for this parser status= " << (int)m_state.m_pstatus << ENDL;
			m_state.m_pstatus = PARSER_STATUS::ERROR;
	}
}

void BridgeCrossingProcessor::processEncounterEvent(yaml_event_t &evt) {
	switch(evt.type) {
		case YAML_SCALAR_EVENT:
			if (eventValueIs(evt, SUB_KEY_EVENT_ENCOUNTER_HIKERS)) {
				m_state.m_pstatus = PARSER_STATUS::PARSING_HIKERS;
			} else {
				LOG_ERR << "invalid event type for ENCOUNTER EVENT TYPE" << ENDL;
				m_state.m_pstatus = PARSER_STATUS::ERROR;
			}
			break;
		case YAML_NO_EVENT:break;
		case YAML_STREAM_START_EVENT:break;
		case YAML_STREAM_END_EVENT:break;
		case YAML_DOCUMENT_START_EVENT:break;
		case YAML_DOCUMENT_END_EVENT:break;
		case YAML_ALIAS_EVENT:break;
		case YAML_SEQUENCE_START_EVENT:break;
		case YAML_SEQUENCE_END_EVENT:break;
		case YAML_MAPPING_START_EVENT:break;
		case YAML_MAPPING_END_EVENT:break;
	}
}

void BridgeCrossingProcessor::processCrossingEvent(yaml_event_t &evt) {
	if (evt.type == YAML_SCALAR_EVENT) {
		switch(m_state.m_subStatus) {
			case PARSER_SUB_STATUS ::PARSING_CROSSING_LENGTH:
				// cross the bridge with current group of hikers
				m_crossingHandler->crossBridge(m_hikers, std::strtol((char*)evt.data.scalar.value, 0, 10));
				m_state.m_subStatus = PARSER_SUB_STATUS::CLEAR;
				break;
			case PARSER_SUB_STATUS::CLEAR:break;
			case PARSER_SUB_STATUS::PARSING_HIKER_SPEED:break;
			case PARSER_SUB_STATUS::PARSING_HIKER_NAME:break;
		}
	}

	switch(evt.type) {
		case YAML_SCALAR_EVENT:
			if (eventValueIs(evt, SUB_KEY_EVENT_CROSSING_LENGTH)) {
				m_state.m_subStatus = PARSER_SUB_STATUS::PARSING_CROSSING_LENGTH;
			}
			break;
		case YAML_NO_EVENT:break;
		case YAML_STREAM_START_EVENT:break;
		case YAML_STREAM_END_EVENT:break;
		case YAML_DOCUMENT_START_EVENT:break;
		case YAML_DOCUMENT_END_EVENT:break;
		case YAML_ALIAS_EVENT:break;
		case YAML_SEQUENCE_START_EVENT:break;
		case YAML_SEQUENCE_END_EVENT:break;
		case YAML_MAPPING_START_EVENT:break;
		case YAML_MAPPING_END_EVENT:break;
	}
}

void BridgeCrossingProcessor::processHikerData(yaml_event_t &evt) {
	switch (m_state.m_subStatus) {
		case PARSER_SUB_STATUS::PARSING_HIKER_NAME:
			g_currHiker.setName((char*)evt.data.scalar.value);
			m_state.m_subStatus = PARSER_SUB_STATUS ::CLEAR;
			break;
		case PARSER_SUB_STATUS::PARSING_HIKER_SPEED:
			g_currHiker.setSpeed(std::stof((char*)evt.data.scalar.value));
			m_state.m_subStatus = PARSER_SUB_STATUS ::CLEAR;
			break;
		case PARSER_SUB_STATUS::CLEAR:
		case PARSER_SUB_STATUS::PARSING_CROSSING_LENGTH:
			break;
	}

	switch(evt.type) {
	case YAML_MAPPING_START_EVENT:
		g_currHiker = Hiker();
		break;
	case YAML_SCALAR_EVENT:
		if (eventValueIs(evt, SUB_KEY_HIKER_NAME)) {
			m_state.m_subStatus = PARSER_SUB_STATUS::PARSING_HIKER_NAME;
		} else if (eventValueIs(evt, SUB_KEY_HIKER_SPEED)) {
			m_state.m_subStatus = PARSER_SUB_STATUS::PARSING_HIKER_SPEED;
		}
		break;
	case YAML_MAPPING_END_EVENT:
		m_hikers.push_back(g_currHiker);
		g_currHiker.clearData();
		break;
	case YAML_SEQUENCE_END_EVENT:
		m_state.m_pstatus = PARSER_STATUS ::BEGIN;
		break;
	case YAML_NO_EVENT:break;
	case YAML_STREAM_START_EVENT:break;
	case YAML_STREAM_END_EVENT:break;
	case YAML_DOCUMENT_START_EVENT:break;
	case YAML_DOCUMENT_END_EVENT:break;
	case YAML_ALIAS_EVENT:break;
	case YAML_SEQUENCE_START_EVENT:break;
	}
}

bool BridgeCrossingProcessor::eventValueIs(const yaml_event_t& evt, const std::string &val) {
	if (!evt.data.scalar.value) return false;
	return (std::strcmp((char*)evt.data.scalar.value, val.c_str()) == 0);
}
