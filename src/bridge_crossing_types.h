#ifndef _BRIDGE_CROSSING_TYPES_H
#define _BRIDGE_CROSSING_TYPES_H

// outer level keys
const std::string KEY_NUM_BRIDGES = "num_bridges";
const std::string KEY_STARTING_HIKERS = "starting_hikers";
const std::string KEY_EVENTS = "events";

// event sub-keys
const std::string SUB_KEY_EVENT_TYPE = "type";
const std::string SUB_KEY_EVENT_CROSSING_LENGTH = "length";
const std::string SUB_KEY_EVENT_ENCOUNTER_HIKERS = "hikers";

// hiker sub keys
const std::string SUB_KEY_HIKER_NAME = "name";
const std::string SUB_KEY_HIKER_SPEED = "speed";

// possible event types
const std::string EVENT_ENCOUNTER_TYPE = "encounter";
const std::string EVENT_CROSSING_TYPE = "crossing";

#endif //_BRIDGE_CROSSING_TYPES_H
