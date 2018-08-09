#include "event_calendar_interface_factory.h"
#include "event_calendar_control.h"

EventCalendarInterfaceFactory::~EventCalendarInterfaceFactory()
{
}

std::unique_ptr<EventCalendarInterface> EventCalendarInterfaceFactory::getInstance()
{
	std::unique_ptr<EventCalendarInterface> inst(new EventCalendarControl());
	return inst;
}
