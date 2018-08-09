#pragma once
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <boost/optional/optional.hpp>
#include "event_calendar_control.h"
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <stdio.h>

using namespace utility;                    
using namespace web;                        
using namespace web::http;                  
using namespace web::http::client;          
using namespace concurrency::streams;       

const std::string GoogleWebService::kGoogleXmlFile("geocode_result.xml");
const std::string GoogleWebService::kLocationPath("GeocodeResponse.result.geometry.location");
const std::string GoogleWebService::kGoogleKey("AIzaSyAWW0tDkfrSpEtEmRhixXkWP6BBNU-13Vo");
const std::string GoogleWebService::kGoogleApi("https://maps.googleapis.com/maps/api/geocode/xml?");
const std::string GoogleWebService::kLatitude("lat");
const std::string GoogleWebService::kLongitude("lng");

const std::string EventFullWebService::kEventXmlFile("search_result.xml");
const std::string EventFullWebService::kEventPath("search.events");
const std::string EventFullWebService::kEventFullKey("wd6XbcJTKNvXXRzG");
const std::string EventFullWebService::kEventFullApi("http://api.eventful.com/rest/events/search?");

EventCalendarControl::EventCalendarControl()
{
	address_ = new Address();
	address_->set_is_optional(false);

	radius_ = new Radius();
	radius_->set_is_optional(true);

	start_date_ = new Date(kStartDateType);
	start_date_->set_is_optional(false);

	end_date_ = new Date(kEndDateType);
	end_date_->set_is_optional(false);

	category_ = new Category();
	category_->set_is_optional(false);

	search_button_ = new SearchButton();
	search_button_->set_status(false);
}

EventCalendarControl::~EventCalendarControl()
{
}

std::vector<std::string> EventCalendarControl::event_search(const std::string &param)
{
	std::vector<Event> searched_events;
	if (can_search(param))
	{
		EventFullWebService *ews = new EventFullWebService();
		searched_events =  ews->do_event_search(param);
		delete ews;
	}
	return convert_to_output_format(searched_events);
}

bool EventCalendarControl::can_search(const std::string &param)
{
	check_search_items(param);
	return search_button_->get_status();
}

bool EventCalendarControl::validate_item(Item item_name, const std::string &value)
{
	bool result = false;

	switch (item_name)
	{
	  case kAddress: {
		  result = address_->validate(value);
		  break;
		}
	  case kStartDate: {
		  result = start_date_->validate(value);
		  break;
	  }
	  case kEndDate: {
		  result = end_date_->validate(value);
		  break;
	  }
	  case kRadius: {
		  result = radius_->validate(value);
		  break;
	  }
	  case kCategory: {
		  result = category_->validate(value);
		  break;
	  }
	  default: {
		  std::cerr << "Invalid Item";
	  }
	};

	return result;
}

int EventCalendarControl::check_search_items(const std::string &param)
{
	bool result = true;
	std::vector<std::string> splitted_param = Utility::split(param, '|');
	if (splitted_param.size() != 5)
	{
		std::cerr << "Invalid Number Of Input Parameters!!";
		search_button_->set_status(false);
		return EXIT_FAILURE;
	}

	(!validate_item(Item::kAddress, splitted_param[0]) ? result = false : address_->set_address(splitted_param[0]));
	(!validate_item(Item::kRadius, splitted_param[1]) ? result = false : radius_->set_radius(std::atof(splitted_param[1].c_str())));
	(!validate_item(Item::kStartDate, splitted_param[2]) ? result = false : start_date_->set_date(splitted_param[2]));
	(!validate_item(Item::kEndDate, splitted_param[3]) ? result = false : end_date_->set_date(splitted_param[3]));
	(!validate_item(Item::kCategory, splitted_param[4]) ? result = false : category_->set_category(splitted_param[4]));
	
	if (end_date_->get_date_date() < start_date_->get_date_date())
	{
		std::cerr << "Start Date Can Not Be Greater Than End Date!!" << std::endl;
		result = false;
	}

	if (result)
	{
		search_button_->set_status(true);
		return EXIT_SUCCESS;
	}
	else
	{
		search_button_->set_status(false);
		return EXIT_FAILURE;
	}
}

Location EventCalendarControl::geocode(const std::string &param)
{
	GoogleWebService *gws = new GoogleWebService();
	Location loc = gws->do_geocoding(param);
	delete gws;
	return loc;
}

std::vector<std::string> EventCalendarControl::convert_to_output_format(std::vector<Event> &in)
{
	std::vector<std::string> output;
	std::string temp;

	for (int i = 0; i < in.size(); ++i)
	{
		temp.clear();
		temp += in[i].get_title() +"|";
		temp += in[i].get_venue_name() + "," + in[i].get_venue_address() + "|";
		temp += in[i].get_performers_name() + "," + in[i].get_info() + "|";
		temp += output_date_format((in[i].get_event_date()).substr(0, 10));

		output.push_back(temp);
	}
	return output;
}

//converts 'YYYY-MM-DD' into 'mm/dd/year, Day of Week'
std::string EventCalendarControl::output_date_format(const std::string date)
{
	std::string output;

	try
	{
		boost::gregorian::date d1(from_simple_string(date));
		greg_weekday wd = d1.day_of_week();
		date::ymd_type ymd = d1.year_month_day();
		output  = std::to_string(ymd.month);
		output += "/";
		output += std::to_string(ymd.day);
		output += "/";
		output += std::to_string(ymd.year);
		output += ",";
		output += wd.as_long_string();
	}
	catch (std::exception& e) {
		std::cerr << "Bad Date Exception: " << e.what() << std::endl;
	}

	return output;
}

std::vector<Event> EventFullWebService::do_event_search(const std::string &param)
{
	std::string ws_param = make_web_service_param(param);
	remove(EventFullWebService::kEventXmlFile.c_str());
	if (call_web_service(kEventFullApi, ws_param, EventFullWebService::kEventXmlFile) == EXIT_SUCCESS)
	{
		extract_events();
	}
	return events_;
}

std::string EventFullWebService::make_web_service_param(const std::string &param)
{
	std::string ws_param;
	std::vector<std::string> splitted_param = Utility::split(param, '|');

	GoogleWebService *gws = new GoogleWebService();
	Location loc = gws->do_geocoding(splitted_param[0]);
	delete gws;
	ws_param  = "where=" + std::to_string(loc.get_latitude()) + "," + std::to_string(loc.get_longitude());
	ws_param += "&within=" + splitted_param[1];
	ws_param += "&units=km";
	ws_param += "&date=" + make_date_param(splitted_param[2], splitted_param[3]);
	ws_param += "&category=" + splitted_param[4];
	ws_param += "&include=price";
	ws_param += "&app_key=" + kEventFullKey;

	return ws_param;
}

void EventFullWebService::extract_events()
{
	using boost::property_tree::ptree;
	ptree pt;
	std::string performers_name;
	std::string venue_addrress;

	try
	{
		read_xml(EventFullWebService::kEventXmlFile, pt);
		BOOST_FOREACH(const ptree::value_type &v, pt.get_child(EventFullWebService::kEventPath))
		{
			Event current_event;
			performers_name.clear();
			venue_addrress.clear();
			const ptree& child = v.second;

			current_event.set_title(child.get<std::string>("title"));
			current_event.set_venue_name(child.get<std::string>("venue_name"));
			current_event.set_event_date(child.get<std::string>("start_time"));
			current_event.set_price(child.get<std::string>("price"));
			//current_event.set_info(child.get<std::string>("description"));

			venue_addrress =  (child.get<std::string>("venue_address")) + ",";
			venue_addrress += (child.get<std::string>("city_name")) + ",";
			venue_addrress += (child.get<std::string>("region_name")) + ",";
			venue_addrress += (child.get<std::string>("country_name"));
			current_event.set_venue_address(venue_addrress);

			// extract the performers names
			BOOST_FOREACH(const ptree::value_type &p, child.get_child("performers"))
			{
				const ptree& pv = p.second;
				performers_name += pv.get<std::string>("name") + ",";
			}
			current_event.set_performers_name(performers_name);
			events_.push_back(current_event);
		}
	}
	catch (std::exception &e)
	{
		std::cout << "There is something wrong!!" << e.what();
	}
}

std::string EventFullWebService::make_date_param(std::string date1, std::string date2)
{
	int day, month, year;
	std::string result;

	sscanf_s(date1.c_str(), "%d/%d/%d;", &day, &month, &year);
	result = std::to_string(year) + std::to_string(month) + std::to_string(day) + "00-";
	sscanf_s(date2.c_str(), "%d/%d/%d;", &day, &month, &year);
	result += std::to_string(year) + std::to_string(month) + std::to_string(day) + "00";
	return result;
}

Location GoogleWebService::do_geocoding(const std::string &address)
{
	Location loc;
	std::string ws_param = make_web_service_param(address);
	remove(GoogleWebService::kGoogleXmlFile.c_str());
	if (call_web_service(kGoogleApi, ws_param, GoogleWebService::kGoogleXmlFile) == EXIT_SUCCESS)
	{
		loc = extract_location();
	}

	return loc;
}

std::string GoogleWebService::make_web_service_param(const std::string &param)
{
	std::string ws_param;

	ws_param = "address=";
	//ws_param += remove_space_form_begining_string(param);
	ws_param += param;
	std::replace(ws_param.begin(), ws_param.end(), ' ', '+');
	ws_param += "&key=";
	ws_param += kGoogleKey;
	return ws_param;
}

WebServiceControl::~WebServiceControl()
{
}

int WebServiceControl::call_web_service(const std::string api, const std::string &param, const std::string xml_file_name)
{
	std::wstring wstr_xml_file_name;
	wstr_xml_file_name.assign(xml_file_name.begin(), xml_file_name.end());

	std::string client_param = (api + param);
	std::wstring wstr_client_param;
	wstr_client_param.assign(client_param.begin(), client_param.end());

	auto fileStream = std::make_shared<ostream>();

	// Open stream to output file.
	pplx::task<void> requestTask = fstream::open_ostream(wstr_xml_file_name).then([=](ostream outFile)
	{
		*fileStream = outFile;
		http_client client(wstr_client_param);
		uri_builder builder(U(""));
		return client.request(methods::GET, builder.to_string());
	})
	// Handle response headers arriving.
	.then([=](http_response response)
	{
		return response.body().read_to_end(fileStream->streambuf());
	})

	// Close the file stream.
	.then([=](size_t)
	{
		return fileStream->close();
	});

	// Wait for all the outstanding I/O to complete and handle any exceptions
	try
	{
		requestTask.wait();
	}
	catch (const std::exception &e)
	{
		printf("Error exception:%s\n", e.what());
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

Location GoogleWebService::extract_location()
{
	using boost::property_tree::ptree;
	ptree pt;
	Location loc;

	try
	{
		read_xml(GoogleWebService::kGoogleXmlFile, pt);
		boost::optional<ptree& > child = pt.get_child_optional(GoogleWebService::kLocationPath);
		if (!child)
		{
			std::cout << "Adreress Is Not Valid!!";
			return loc;
		}

		for (const ptree::value_type &v : pt.get_child(GoogleWebService::kLocationPath))
		{
			if (v.first == kLatitude) loc.set_latitude(atof(v.second.data().c_str()));
			if (v.first == kLongitude) loc.set_longitude(atof(v.second.data().c_str()));
		}
	}
	catch (std::exception const&  ex)
	{
		std::cerr << "Can't Read XML File For GoogleWebService." << ex.what();
	}
	return loc;
}
