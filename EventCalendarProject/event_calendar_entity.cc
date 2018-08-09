#pragma once
#include "event_calendar_entity.h"
#include "event_calendar_control.h"
#include <boost/algorithm/string.hpp>

const float Radius::kRadiusUpperLimit = 300;
const std::string Category::kMusicCategory("music");
const std::string Category::kSportsCategory("sports");
const std::string Category::kPerformingArtsCategory("performing arts");

EventItemEntity::EventItemEntity()
{
}

EventItemEntity::~EventItemEntity()
{
}

std::string EventItemEntity::get_title()
{
	return title_;
}

void EventItemEntity::set_title(const std::string &title)
{
	this->title_ = title;
}

bool EventItemEntity::get_is_optional()
{
	return is_optiobal_;
}

void EventItemEntity::set_is_optional(const bool is_optional)
{
	this->is_optiobal_ = is_optional;
}

Address::Address()
{
}

Address::~Address()
{
}

bool Address::validate(const std::string &value)
{
	if (value.empty())
	{
		std::cerr << "Address Can Not Be Empty!!";
		return false;
	}
	GoogleWebService *gws = new GoogleWebService();
	Location loc = gws->do_geocoding(value);
	delete gws;
	if (loc.get_latitude() == 0 && loc.get_longitude() == 0)
	{
		std::cerr << "Address Is Not Valid!!";
		return false;
	}
	return true;
}

std::string Address::get_address()
{
	return address_;
}

void Address::set_address(const std::string &address)
{
	address_ = address;
}

bool Radius::validate(const std::string &value)
{
	bool result = false;
	if (!Utility::has_non_numeric_char(value))
	{
		std::cerr << "Radius cant Not Contain Non-numeric Charachters!!";
		return result;
	}
	else
	{
		result = true;
	}

	float float_value = std::stof(value);
	if (float_value < 0 || float_value > kRadiusUpperLimit)
	{
		std::cerr << "Radius cant not Be Negative Or More Than " << kRadiusUpperLimit << "!!";
		result = false;
	}

	return result;
}

double Radius::get_radius()
{
	return radius_;
}

void Radius::set_radius(const double radius)
{
	this->radius_ = radius;
}

bool Date::validate(const std::string &value)
{
	bool result = false;

	result = validate_date_string_format(value);
	if (result == false)
	{
		std::cerr << "Input Date String Is not Accoding to Application Format!!";
		return result;
	}

	date today = day_clock::local_day();
	switch (date_type_)
	{
	  case kStartDateType: {
		  if (date_ < today)
		  {
			  std::cerr << "Invalid Start Date: Less Than Today";
			  result = false;
		  }
		  break;
	  }
	  case kEndDateType: {
		  if (date_ < today)
		  {
			  std::cerr << "Invalid End Date: Less Than Today";
			  result = false;
		  }
		  break;
		   break;
	  }
	  default: {
		   std::cerr << "Invalid Date Type " << date_type_ << "!!";
		   result = false;
	  }
	};

	return result;
}

std::string Date::get_date()
{
	return str_date_;
}

void Date::set_date(const std::string &date)
{
	this->str_date_ = date;
	date_ = (ddmmyyyy_as_date(date));
}

int Date::duration_days_from_today()
{
	date today = day_clock::local_day();
	if (date_.is_not_a_date() == true)
		return EXIT_FAILURE;
	days duration = today - date_;
	return duration.days();
}

date Date::get_date_date()
{
	return date_;
}

bool Date::validate_date_string_format(const std::string &date)
{
	if (date.empty())
	{
		std::cerr << "Date Can not Be Empty!!";
		return false;
	}

	if (date.length() != 10)
	{
		std::cerr << "Date Length Is Not Valid!!";
		return false;
	}

	if (date.at(2) != '/' || date.at(5) != '/')
	{
		std::cerr << "Date Invalid Format!!";
		return false;
	}

	std::string str_day = date.substr(0, 2);
	if (!Utility::has_non_numeric_char(str_day))
	{
		std::cerr << "Date::Day cant Not Contain Non-numeric Charachters!!";
		return false;
	}
	int int_day = std::atoi(str_day.c_str());
	if (int_day < 1 || int_day > 31)
	{
		std::cerr << "Date::Day Is Out of Range!!";
		return false;
	}

	std::string str_month = date.substr(3, 2);
	if (!Utility::has_non_numeric_char(str_month))
	{
		std::cerr << "Date::Month cant Not Contain Non-numeric Charachters!!";
		return false;
	}

	int int_month = std::atoi(str_month.c_str());
	if (int_month < 1 || int_month > 12)
	{
		std::cerr << "Date::Month Is Out of Range!!";
		return false;
	}

	std::string str_year = date.substr(6, 4);
	if (!Utility::has_non_numeric_char(str_year))
	{
		std::cerr << "Date::Year cant Not Contain Non-numeric Charachters!!";
		return false;
	}

	int int_year = std::atoi(str_year.c_str());
	int current_year = get_current_year();
	if (int_year < current_year || int_year > current_year + 1)
	{
		std::cerr << "Date::Month Is Out of Range!!";
		return false;
	}

	return true;
}

int Date::get_current_year()
{
	date today = day_clock::local_day();
	date::ymd_type ymd = today.year_month_day();
	return ymd.year;
}

boost::gregorian::date Date::ddmmyyyy_as_date(const std::string & str)
{
	boost::gregorian::date date;

	try
	{
		const std::locale fmt2(std::locale::classic(), new boost::gregorian::date_input_facet("%d/%m/%Y"));
		std::istringstream is(str);
		is.imbue(fmt2);
		is >> date;
	}
	catch (std::exception& e) {
		std::cerr << "Exception For Bad Date : " << e.what() << std::endl;
	}

	return date;
}

std::string Event::get_title()
{
	return title_;
}

void Event::set_title(const std::string &title)
{
	this->title_ = title;
}

std::string Event::get_venue_name()
{
	return venue_name_;
}

void Event::set_venue_name(const std::string &name)
{
	this->venue_name_ = name;
}

std::string Event::get_venue_address()
{
	return venue_address_;
}

void Event::set_venue_address(const std::string &address)
{
	this->venue_address_ = address;
}

std::string Event::get_info()
{
	return info_;
}

void Event::set_info(const std::string &info)
{
	this->info_ = info;
}

std::string Event::get_price()
{
	return price_;
}

void Event::set_price(const std::string &price)
{
	this->price_ = price;
}

std::string Event::get_event_date()
{
	return event_date_;
}

void Event::set_event_date(const std::string &date)
{
	this->event_date_ = date;
}

std::string Event::get_performers_name()
{
	return performers_name_;
}

void Event::set_performers_name(const std::string &name)
{
	this->performers_name_ = name;
}

double Location::get_latitude()
{
	return latitude_;
}

void Location::set_latitude(const double latitude)
{
	this->latitude_ = latitude;
}

double Location::get_longitude()
{
	return longitude_;
}

void Location::set_longitude(const double longitude)
{
	this->longitude_ = longitude;
}

bool Utility::has_non_numeric_char(const std::string &input)
{
	bool result = std::all_of(input.begin(), input.end(), ::isdigit);
	return result;
}

std::vector<std::string> Utility::split(const std::string &str, const char delimiter)
{
		std::vector<std::string> splitted_str;
		std::stringstream ss(str);
		std::string token("0");

		while (std::getline(ss, token, delimiter)) {
			if (token.empty()) token = "0";
			splitted_str.push_back(token);
		}
		return splitted_str;
}

bool SearchButton::get_status()
{
	return status_;
}

void SearchButton::set_status(const bool status)
{
	this->status_ = status;
}

void Category::set_category(const std::string &category)
{
	this->category_ = category;
}

std::string Category::get_category()
{
	return category_;
}

bool Category::validate(const std::string & value)
{
	bool result = false;
	if (boost::iequals(value, Category::kMusicCategory) == 1 ||
		boost::iequals(value, Category::kSportsCategory) == 1 ||
		boost::iequals(value, Category::kPerformingArtsCategory) == 1)
	{
		result = true;
	}
	return result;
}
