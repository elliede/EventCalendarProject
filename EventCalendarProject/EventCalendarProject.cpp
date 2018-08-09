// EventCalendarProject.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "event_calendar_interface_factory.h"
#include <conio.h>

void module_test();
void print_output(std::vector<std::string> in);

int main()
{
	module_test();
	_getch();
    return 0;
}

void module_test()
{
	bool result = false;

	// --->Correct
	result = EventCalendarInterfaceFactory::getInstance()->validate_item(Item::kAddress, "103, 4111, Hastings, V5C6T7, Burnaby");
	std::cout << std::endl << "1-Address Validation Result :" << result << std::endl;
	// --->Incorrect
	result = EventCalendarInterfaceFactory::getInstance()->validate_item(Item::kAddress, "ghghf, abc, us");
	std::cout << std::endl << "2-Address Validation Result :" << result << std::endl;
	// --->Incorrect
	result = EventCalendarInterfaceFactory::getInstance()->validate_item(Item::kAddress, "");
	std::cout << std::endl << "3-Address Validation Result :" << result << std::endl;

	// --->Correct
	result = EventCalendarInterfaceFactory::getInstance()->validate_item(Item::kRadius, "0");
	std::cout << std::endl << "1-Radius Validation Result :" << result << std::endl;
	// --->Incorrect
	result = EventCalendarInterfaceFactory::getInstance()->validate_item(Item::kRadius, "445");
	std::cout << std::endl << "2-Radius Validation Result :" << result << std::endl;

	// --->Correct
	result = EventCalendarInterfaceFactory::getInstance()->validate_item(Item::kStartDate, "07/07/2016");
	std::cout << std::endl << "1-Start Date Validation Result :" << result << std::endl;
	// --->Incorrect
	result = EventCalendarInterfaceFactory::getInstance()->validate_item(Item::kStartDate, "07/30/2016");
	std::cout << std::endl << "2-Start Date Validation Result :" << result << std::endl;
	// --->Correct
	result = EventCalendarInterfaceFactory::getInstance()->validate_item(Item::kStartDate, "25/05/2016");
	std::cout << std::endl << "3-Start Date Validation Result :" << result << std::endl;

	// --->Correct
	result = EventCalendarInterfaceFactory::getInstance()->validate_item(Item::kEndDate, "07/09/2016");
	std::cout << std::endl << "1-End Date Validation Result :" << result << std::endl;
	// --->Correct
	result = EventCalendarInterfaceFactory::getInstance()->validate_item(Item::kEndDate, "25/01/2016");
	std::cout << std::endl << "2-End Date Validation Result :" << result << std::endl;
	// --->Inorrect
	result = EventCalendarInterfaceFactory::getInstance()->validate_item(Item::kEndDate, "38/09/2016");
	std::cout << std::endl << "3-End Date Validation Result :" << result << std::endl;

	// --->Correct
	result = EventCalendarInterfaceFactory::getInstance()->validate_item(Item::kCategory, "music");
	std::cout << std::endl << "1-Category Validation Result :" << result << std::endl;
	// --->Incorrect
	result = EventCalendarInterfaceFactory::getInstance()->validate_item(Item::kCategory, "dance");
	std::cout << std::endl << "2-Category Validation Result :" << result << std::endl;

	// --->Correct
	std::string param1("1600 Amphitheatre Parkway, Mountain View, CA|0|25/07/2016|26/09/2016|music");
	result = EventCalendarInterfaceFactory::getInstance()->can_search(param1);
	std::cout << std::endl << "1- Can Search Result :" << result << std::endl;
	// --->Correct
	std::string param2("103, 4111, Hastings, V5C6T7, Burnaby|0|25/07/2016|26/09/2016|music");
	result = EventCalendarInterfaceFactory::getInstance()->can_search(param2);
	std::cout << std::endl << "2- Can Search Result :" << result << std::endl;
	// --->Inorrect
	std::string param3("103, 4111, Hastings, V5C6T7, Burnaby|0|26/09/2016|25/07/2016|music");
	result = EventCalendarInterfaceFactory::getInstance()->can_search(param3);
	std::cout << std::endl << "3- Can Search Result :" << result << std::endl;

	// --->Correct
	std::vector<std::string> output1 = EventCalendarInterfaceFactory::getInstance()->event_search(param1);
	std::cout << std::endl << "1- Event Search result:" << std::endl;
	std::cout << "*************************************************************" << std::endl;
	print_output(output1);
	// --->Inorrect
	std::vector<std::string> output2 = EventCalendarInterfaceFactory::getInstance()->event_search(param2);
	std::cout << std::endl << "2- Event Search result:" << std::endl;
	std::cout << "*************************************************************" << std::endl;
	print_output(output2);
}

void print_output(std::vector<std::string> in)
{
	for (auto it = in.begin(); it != in.end(); ++it)
	{
		std::cout << std::endl << *it << std::endl;
	}
}
