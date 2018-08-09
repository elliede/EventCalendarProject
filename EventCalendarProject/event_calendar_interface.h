#pragma once
#include "event_calendar_entity.h"

class EventCalendarInterface {
public:
	virtual ~EventCalendarInterface();
	virtual std::vector<std::string> event_search(const std::string &param) = 0;
	virtual bool validate_item(Item item_name, const std::string &value) = 0;
	virtual bool can_search(const std::string &param) = 0;
};
