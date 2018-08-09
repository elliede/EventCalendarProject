#pragma once
#include "boost/date_time/gregorian/gregorian.hpp"
#include <string>
#include <vector>

using namespace boost::gregorian;

class EventItemEntity {
  public:
	  EventItemEntity();
	  virtual ~EventItemEntity();
	  virtual bool validate(const std::string &value) = 0;
	  std::string get_title();
	  void set_title(const std::string &title);
	  bool get_is_optional();
	  void set_is_optional(const bool is_optional);
  private:
	  std::string title_;
	  bool is_optiobal_;
};

class Address : public EventItemEntity {
 public:
	 Address();
	 ~Address();
	  bool validate(const std::string &value);
	  std::string get_address();
	  void set_address(const std::string &address);
 private:
	  std::string address_;
};

class Radius : public EventItemEntity {
 public:
	 Radius() : radius_(1) {}
	  bool validate(const std::string &value);
	  double get_radius();
	  void set_radius(const double radius);
	  static const float kRadiusUpperLimit;
 private:
	  double radius_;
};

class Date : public EventItemEntity {
  public:
	  Date(int type) : date_type_(type) {}
	  bool validate(const std::string &value);
	  std::string get_date();
	  void set_date(const std::string &date);
	  int duration_days_from_today();
	  date get_date_date();
	  inline int operator-(const Date &d) const {
		  days duration_days = date_ - d.date_;
		  return duration_days.days();
	  }
	  inline bool operator<(const Date &d) const {
		  if (date_ < d.date_)
			  return true;
		  else
			  return false;
	  }
  private:
	  bool validate_date_string_format(const std::string &date);
	  int get_current_year();
	  boost::gregorian::date ddmmyyyy_as_date(const std::string& str);
	  int date_type_; // 0 : start date, 1 : end date
	  std::string str_date_;
	  date date_;
};

class Event {
  public:
	std::string get_title();
	void set_title(const std::string &title);
	std::string get_venue_name();
	void set_venue_name(const std::string &name);
	std::string get_venue_address();
	void set_venue_address(const std::string &address);
	std::string get_info();
	void set_info(const std::string &info);
	std::string get_price();
	void set_price(const std::string &price);
	std::string get_event_date();
	void set_event_date(const std::string &date);
	std::string get_performers_name();
	void set_performers_name(const std::string &name);
  private:
	std::string image_file_name_;
	std::string image_file_url_;
	std::string title_;
	std::string venue_name_;
	std::string venue_address_;
	std::string info_;
	std::string price_;
	std::string event_date_;
	std::string performers_name_;
};

class Location {
  public:
	  Location() : latitude_(0), longitude_(0) {}
	  double get_latitude();
	  void set_latitude(const double latitude);
	  double get_longitude();
	  void set_longitude(const double longitude);
  private:
	double latitude_;
	double longitude_;
};

class SearchButton {
  public:
	  SearchButton() : status_(true) {}
	  bool get_status();
	  void set_status(const bool status);
  private:
	bool status_;
};

class Utility {
public:
	static bool has_non_numeric_char(const std::string &input);
	static std::vector<std::string> split(const std::string &str, const char delimiter);
};

class Category : public EventItemEntity {
public:
	void set_category(const std::string &category);
	std::string get_category();
	bool validate(const std::string &value);
	static const std::string kMusicCategory;
	static const std::string kSportsCategory;
	static const std::string kPerformingArtsCategory;
private:
	std::string category_;
};

enum DateType {
	kStartDateType = 0,
	kEndDateType 
};

enum Option {
	kMandatory = 0,
	kOptional 
};

enum Item {
	kAddress = 0,
	kRadius,
	kStartDate,
	kEndDate,
	kCategory
};
