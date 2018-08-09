# EventCalendarProject
A simple app that retrieves events close to the address you have provided for a given date range from Eventful.com web service. Display those events in a scrolling view.
Search Fields

Address
•	Full US address
•	Required field
•	No default
•	Non-valid inputs:
o	Address not found in web service result (Google Geocode)
	Address lookup happens when the user focuses to another field.
o	Empty address field
•	In case of non-valid input:
o	Indicate visually next to Address that it is not valid.
o	Disable Search Button
•	Address is entered in one text box as shown in wireframe, it is not divided into city etc.

Radius
•	Radius is treated as a float
•	The unit is in km.
•	Default value is 1km and it is not required field.
•	Non-valid inputs are 
o	Less than 0
o	More than 300km
o	Non-numeric characters
•	In case of non-valid input:
o	Indicate visually next to Radius that it not valid
o	Disable Search Button
Date Start/Date End
•	Date entered in dd/mm/yyyy format
•	Textbox + Date picker as shown in the wireframe
•	Default values:
o	Date Start -> Today
o	Date End -> 1 day after date start
o	DatePicker defaults to the above values.
o	TextField defaults to the above values.
•	Required field
•	Non-valid inputs:
o	Empty
o	Not matching the format
o	Non-numeric dd, mm, year
o	Date Start > Date End
o	Maximum number of days to search for is 28.
o	Date Start < current date
o	Year < current year, Year > current Year + 1
o	Date not valid day of the month (including leap year calculation)
•	Validate date when input focus changes
•	The user can either type in the dates or select them using the date picker calendar control.
•	In case of non-valid input:
o	Indicate visually next to Date that it not valid, as shown in wireframe
o	Disable Search Button

Event Category
•	The event category as a drop-down list
•	The values are:
o	Music, Sports, Performing Arts
•	Default value is music
•	Non required field

Search Button
•	When enabled, run the search with the set parameters.

Results
The results are shown in a list format, as shown in the wireframe. For each result, display:
•	The main image for the event,
•	The event title,
•	The venue for the event,
•	The artists, teams depending on the event type,
•	Date for the event in the form of “mm/dd/year, Day of Week” E.g. 05/03/12, Thursday

Data and Web Services
•	The user input is defined as (see section before for details on validation)
o	Address, radius, date start, date end and Event category
•	The output is defined as a list of events of a given event category with following details for each event:
o	Event title, url, begin date/time, stop date/time, price
o	Venue name, url, description, type, address, latitude, longitude
o	Images associated with the event (the full image file also downloaded not just a link)
•	For Google-geocode web service, you need to implement for address verification and retrieving longitude/latitude.
o	Use the lon/lat information in Eventful Web Service as well.
o	Handle error cases of not valid address based on what is required by the UI.
•	For Eventful Web Service, use only the public access levels, do not authenticate as a user. You will only be retrieving public data.
o	Notice that the above information that needs to be retrieved from Eventful will require multiple API calls. 
o	The full event list needs to handle batching. I.e. you need to retrieve the full list of events as specified in the API, not just the first page.
•	Data that is returned from Eventful needs to be properly modeled for the UI layers to be used. 
o	Your design here is important! We will not specify what it should look like, you need to do the design of the API the UI will use to get this domain specific data.

2.	Console App Option
A simple console application that would accept search parameters in a vertical-bar seperated format, in the following form:
	executablename {Address} | {Radius} | {Start Date} | {End Date} | {Category}”

Data validation errors should be reflected in the program output via meaningful messages that indicate how the user may fix the problems with the inputs.
The output, given a valid set of inputs, should be a vertical-bar separated list of the required results and should adhere to the following format:
	{event title} | {event venue} | {event artists, teams, etc} | {event date}
	{event title} | {event venue} | {event artists, teams, etc} | {event date}
…

Output should omit the image, but follow the formats specified in the UI specification.  An arbitrary amount of whitespace padding may be included along with vertical bars in order to make output more readable.
3.	Further notes
•	Used the following Web APIs:
o	Events list -> http://api.eventful.com/
o	Google Geocode -> https://developers.google.com/maps/documentation/geocoding/#JSON
