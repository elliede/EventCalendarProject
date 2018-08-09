#pragma once
#include "event_calendar_entity.h"
#include "event_calendar_interface.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <vector>
#include <string>


class EventCalendarControl : public EventCalendarInterface {
  public:
	EventCalendarControl();
	~EventCalendarControl();
	std::vector<std::string> event_search(const std::string &param);
	bool can_search(const std::string &param);
	bool validate_item(Item item_name, const std::string &value);
	int check_search_items(const std::string &param);
	Location geocode(const std::string &param);
  private:
	std::vector<std::string> convert_to_output_format(std::vector<Event> &in);
	std::string output_date_format(const std::string date); 
	Address      *address_;
	Radius       *radius_;
	Date         *start_date_;
	Date         *end_date_;
	Category     *category_;
	SearchButton *search_button_;
};

class WebServiceControl {
  public:
	virtual ~WebServiceControl();
	int call_web_service(const std::string api, const std::string &param, const std::string xml_file_name);
	virtual std::string make_web_service_param(const std::string &param) = 0;
	static const std::string kResultXmlFile;
};


class EventFullWebService : public WebServiceControl {
 public:
	std::vector<Event> do_event_search(const std::string &param);
	std::string make_web_service_param(const std::string &param);
 private:
	void extract_events();
	std::string make_date_param(std::string date1, std::string date2);
	std::vector<Event> events_;
	static const std::string kEventXmlFile;
	static const std::string kEventPath;
	static const std::string kEventFullKey;
	static const std::string kEventFullApi;
};

class GoogleWebService : public WebServiceControl {
 public:
	Location do_geocoding(const std::string &address);
	std::string make_web_service_param(const std::string &param);
 private:
	Location extract_location();
	Location location_;
	static const std::string kGoogleXmlFile;
	static const std::string kLocationPath;
	static const std::string kLatitude;
	static const std::string kLongitude;
	static const std::string kGoogleKey;
	static const std::string kGoogleApi;
};