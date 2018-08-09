#pragma once
#include "event_calendar_interface.h"

class EventCalendarInterfaceFactory {
	virtual ~EventCalendarInterfaceFactory();
public:
	static std::unique_ptr<EventCalendarInterface> getInstance();
};
