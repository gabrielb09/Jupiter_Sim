#include <ctime>
#include <string>
#include <iostream>


static char date[8];

void getDate(){
	// get the time in seconds
	time_t now = time(0);
	// create a time object to store day month year
	tm *dateTime = localtime(&now);
	// get the appropriate day month and year
	int year = 1900 + dateTime->tm_year;
	int month = 1 + dateTime->tm_mon;
	int day = dateTime->tm_mday;
	// format a date string
	sprintf(date, "%02d_%02d", month, day);
}
