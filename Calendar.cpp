#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

// Global Variables
enum Keycode{
	Left=75, Right=77, Up=72, Down=80, Enter=13, Escape=27, Backspace=8, Tab=9, Tild=96
};
int mon_day[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
const char* month_name[12] = {
	"January", "February", "March", "April", "May", "June", "July",
	"August", "September", "October", "November", "December"
};
enum Calendar {
	Days = 0,
	Months = 1,
	Years = 2,
	None = 3
};

// Function Declarations
void print_days_of_month(tm& date);
void print_months_of_year(tm& date, int steps=4);
void print_years(tm& date, int steps = 5, int range = 8);
bool isLeapYear(int year);void AddDate(tm& date, int dayCount, int monthCount = 0, int yearCount = 0);

int main()
{
	time_t now = time(nullptr);

	tm date;
	localtime_s(&date, &now);

	bool quit = false;
	int calendar_mod = Days;
	int month_step = 4;
	int year_step = 5;

	// Application loop
	while (!quit)
	{
		// set no of days in Feb for the give year
		int year = date.tm_year + 1900;
		mon_day[1] = (isLeapYear(year) ? 29 : 28);

		// clear screen
		system("cls");

		// print calendar
		switch (calendar_mod)
		{
		case Days:
			print_days_of_month(date);
			break;
		case Months:
			print_months_of_year(date, month_step);
			break;
		case Years:
			print_years(date, year_step);
			break;
		}

		// print instructions
		printf(
			"\n\n--------------Instructions----------"
			"\nPress Escape(Esc) to quit."
			"\nPress Tab to change Calendar-Mod"
			"\n"
		);

		// print info
		const char* week_day[7] = { "Sunday", "Monday", "Tuesday", "Wednessday", "Thursday", "Friday", "Saturday" };
		printf("\n\nDate: %2d:%2d:%2d, Week-Day:(%d) %s", date.tm_mday, date.tm_mon + 1, date.tm_year+1900,
			date.tm_wday, week_day[date.tm_wday]);

		// process input
		int keycode = _getch();
		switch (keycode)
		{
		case Left:	// Left Arrow-key pressed
			switch (calendar_mod)
			{
			case Days:
				AddDate(date, -1, 0, 0);
				break;
			case Months:
				AddDate(date, 0, -1, 0);
				break;
			case Years:
				AddDate(date, 0, 0, -1);
				break;
			}
			break;
		case Right:	// Right arrow-key pressed
			switch (calendar_mod)
			{
			case Days:
				AddDate(date, 1, 0, 0);
				break;
			case Months:
				AddDate(date, 0, 1, 0);
				break;
			case Years:
				AddDate(date, 0, 0, 1);
				break;
			}
			break;
		case Up:	// Up arrow-key pressed
			switch (calendar_mod)
			{
			case Days:
				AddDate(date, -7, 0, 0);
				break;
			case Months:
				AddDate(date, 0, -month_step, 0);
				break;
			case Years:
				AddDate(date, 0, 0, -year_step);
				break;
			}
			break;
		case Down:	// Down arrow-key pressed
			switch (calendar_mod)
			{
			case Days:
				AddDate(date, 7, 0, 0);
				break;
			case Months:
				AddDate(date, 0, month_step, 0);
				break;
			case Years:
				AddDate(date, 0, 0, year_step);
				break;
			}
			break;
		case Tab:	// Tab pressed
			if (++calendar_mod == None)
			{
				calendar_mod = Days;
			}
			break;
		case Escape:	// Escape Pressed
			quit = true;
			break;
		}
	}
	// End of application

	return 0;
}

// ------------------------------Function Definitions-----------------------------
void print_days_of_month(tm& date)
{

	// day of the week
	int week_day = 0;	// sunday

	// print month's calender
	printf("\n--------%s-%d----------\n", month_name[date.tm_mon], 1900 + date.tm_year);
	printf("Sun Mon Tue Wed Thu Fri Sat\n");

	// week day of first day of the month
	int first_day = (date.tm_wday - date.tm_mday % 7) + 1;
	if (first_day < 0)
	{
		first_day += 7;
	}
	// fill empty days
	while (week_day < first_day)
	{
		printf("    ");
		week_day++;
	}

	// print the rest of the month
	for (int month_day = 1u; month_day <= mon_day[date.tm_mon]; month_day++, week_day++)
	{
		if (week_day == 7)	// if its sunday
		{
			printf("\n");			// means its next week's
			week_day = 0;			// first  day
		}

		const char curdate_formatting = (month_day == date.tm_mday ? '|' : ' ');

		printf("%c%2d%c", curdate_formatting, month_day, curdate_formatting);	// print the date on the calendar
	}
}

void print_months_of_year(tm& date, int steps)
{
	// formatting
	const int x = (12 * steps - 4) / 2;

	// ---------------------------Header-----------------------------

	printf("\n");
	for (int i = 0; i < x; i++) printf("-");
	// print year
	printf("%4d", 1900+date.tm_year);

	for (int i = 0; i < x; i++) printf("-");
	printf("\n");

	// ---------------------------Body---------------------------------
	for (int month = 0; month < 12; month++)
	{
		if (month % steps == 0)
		{
			printf("\n\n");
		}
		const char formatting = (month==date.tm_mon ? '|' : ' ');
		printf("%c%10s%c", formatting, month_name[month], formatting);
	}
}

void print_years(tm& date, int steps, int range)
{
	// formatting
	const int x = (6 * steps - 13) / 2;

	const int startYear = 1900 + date.tm_year - range;

	// -----------------------------Header----------------------------
	printf("\n");
	for (int i = 0; i < x; i++) printf("-");
	// print range
	printf("(%4d - %4d)", startYear, startYear + 2*range - 1);
	
	for (int i = 0; i < x; i++) printf("-");
	printf("\n");

	// -----------------------------Body--------------------------------
	// print years
	for (int year = 0; year < 2*range ; year++)
	{
		if (year % steps == 0)
		{
			printf("\n\n");
		}
		const char formatting = (startYear + year == 1900+date.tm_year) ? '|' : ' ';
		printf("%c%4d%c", formatting, startYear + year, formatting);
	}
}

// utility functions
bool isLeapYear(int year)
{
	if (year % 100 != 0 || year % 4 == 0 || year % 400 == 0)
	{
		return true;
	}

	return false;
}

void AddDate(tm& date, int dayCount, int monthCount, int yearCount)
{
	date.tm_mday += dayCount;
	date.tm_mon += monthCount;
	date.tm_year += yearCount;
	mktime(&date);
}
