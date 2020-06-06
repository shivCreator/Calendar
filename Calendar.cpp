// visual studio community 2019, windows 7 32-bit
#define COLORED

#include <stdio.h>	// has functions: printf
#include <time.h>	// has functions: localtime and struct tm
#include <conio.h>	// has functions: _getch
#include <stdlib.h>	// has functions: system("cls")
#include <Windows.h>// has functions: SetConsoleTextAttribute for color
#include <string.h>	// 

// Global Variables
HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

enum Keycode{
	Left=75, Right=77, Up=72, Down=80, Enter=13, Esc=27, Backspace=8, Tab=9, Tild=96, Space=' '
};
int mon_day[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
const char* month_name[12] = {
	"January", "February", "March", "April", "May", "June", "July",
	"August", "September", "October", "November", "December"
};
const char* day_name[7] = { "Sunday", "Monday", "Tuesday", "Wednessday", "Thursday", "Friday", "Saturday" };
enum Calendar {
	Days = 0,
	Months = 1,
	Years = 2,
	None = 3
};

// Function Declarations
void print_days_of_month(tm& date);
void print_months_of_year(tm& date);
void print_years(tm& date, int steps = 5, int range = 8);
bool isLeapYear(int year);
void AddDate(tm& date, int dayCount, int monthCount = 0, int yearCount = 0);

// coloring accessories
inline void setColor(WORD color)
{
#ifdef COLORED
	SetConsoleTextAttribute(console_handle, color);
#endif
}
inline void fillLine(int length)
{
#ifdef COLORED
	for (int i = 0;i < length; i++)
		printf(" ");
#endif
	printf("\n");
}
#define DEFAULT_COLOR (FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE)
#define CALENDAR_COLOR (FOREGROUND_RED | FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)
#define HIGHLIGHT_COLOR (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_GREEN)

int main()
{
	time_t now = time(nullptr);

	tm today;
	localtime_s(&today, &now);
	tm date = today;

	bool quit = false;
	int calendar_mod = Days;
	int month_step = 3;
	int year_step = 4;

	// Application loop
	while (!quit)
	{
		// set no of days in Feb for the give year
		int year = date.tm_year + 1900;
		mon_day[1] = (isLeapYear(year) ? 29 : 28);

		// clear screen
		system("cls");

		// print calendar
		setColor(CALENDAR_COLOR);
		switch (calendar_mod)
		{
		case Days:
			print_days_of_month(date);
			break;
		case Months:
			print_months_of_year(date);
			break;
		case Years:
			print_years(date, year_step);
			break;
		}

		// print instructions
		setColor(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE|BACKGROUND_BLUE);
		fillLine(35);
		printf(
			"           Instructions            "
			"\n%-35s\n%-35s\n%-35s\n%-35s",
			"Escape(Esc) -> Quit",
			"Tab -> change Calendar-Mode",
			"Arrow-Keys -> Navigation",
			(
				calendar_mod==Days ?
				"Enter -> Add Notes" :
				"Enter -> Change Calendar Mode Back"
			)
		);
		printf("\n");
		fillLine(35);
		fillLine(35);
		setColor(DEFAULT_COLOR);

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
		case Enter:
			switch (calendar_mod)
			{
			case Days:
				calendar_mod = Years;
				break;
			case Months:
				calendar_mod = Days;
				break;
			case Years:
				calendar_mod = Months;
				break;
			}
			break;
		case Esc:	// Escape Pressed
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
	int no_of_weeks = 0;

	// week day of first day of the month
	int first_day = (date.tm_wday - date.tm_mday % 7) + 1;
	if (first_day < -1)
	{
		first_day += 7;
	}

	// print month's calender
	// -------------------------------------Header-----------------------------
	setColor(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE|BACKGROUND_RED);
	fillLine(35);
	printf("----------%9s-%4d-----------\n", month_name[date.tm_mon], 1900 + date.tm_year);
	fillLine(35);
	setColor(CALENDAR_COLOR);
	printf(" Sun  Mon  Tue  Wed  Thu  Fri  Sat \n");
	fillLine(35);
	// -------------------------------------Body-------------------------------
	// fill empty days
	while (week_day < first_day)
	{
		printf("     ");
		week_day++;
	}

	// print the rest of the month
	for (int month_day = 1u; month_day <= mon_day[date.tm_mon]; month_day++, week_day++)
	{
		if (week_day == 7)	// if its sunday
		{
			printf("\n");			// means its next week's
			week_day = 0;			// first  day
			no_of_weeks++;
			fillLine(35);
		}

		char formatting = ' ';
		if (month_day == date.tm_mday)
		{
#ifndef COLORED
			formatting = '|';
#endif
			setColor(HIGHLIGHT_COLOR);
			printf(" %c%2d%c",formatting, month_day, formatting);
			setColor(CALENDAR_COLOR);
		}
		else
			printf(" %c%2d%c",formatting, month_day, formatting);	// print the date on the calendar
	}
	// fill month end
	while (week_day < 7 && week_day != 0)
	{
		printf("     ");
		week_day++;
	}
	printf("\n");
	while (no_of_weeks < 6)
	{
		fillLine(35);
		fillLine(35);
		no_of_weeks++;
	}
}

void print_months_of_year(tm& date)
{
	// print year
	// ---------------------------Header-----------------------------
	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_RED);
	fillLine(35);
	printf("------------ ( %4d ) -------------", 1900+date.tm_year);
	printf("\n");
	fillLine(35);
	setColor(CALENDAR_COLOR);

	// ---------------------------Body---------------------------------
	for (int month = 0; month < 12; month++)
	{
		if (month % 3 == 0 && month != 0)
		{
			//printf("\n\n");
			printf("  \n");
			fillLine(35);
			fillLine(35);
		}
		char formatting = ' ';
		if (month == date.tm_mon)
		{
#ifndef COLORED
			formatting = '|';
#endif
			setColor(HIGHLIGHT_COLOR);
			printf("%c%9s%c", formatting, month_name[month], formatting);
			setColor(CALENDAR_COLOR);
		}
		else
			printf("%c%9s%c", formatting, month_name[month], formatting);
	}
	printf("  \n");
	for(int i = 0; i < 5; i++)
		fillLine(35);
}

void print_years(tm& date, int steps, int range)
{
	const int startYear = 1900 + date.tm_year - range;

	// -----------------------------Header----------------------------
	// print range
	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_RED);
	fillLine(35);
	printf("--------- ( %4d - %4d ) ---------\n", startYear, startYear + 2*range - 1);
	fillLine(35);
	setColor(CALENDAR_COLOR);

	// -----------------------------Body--------------------------------
	// print years
	for (int year = 0; year < 2*range ; year++)
	{
		if (year % steps == 0 && year != 0)
		{
			printf("   \n");
			fillLine(35);
			fillLine(35);
		}
		char formatting = ' ';
		if (startYear + year == 1900 + date.tm_year)
		{
#ifndef COLORED
			formatting = '|';
#endif
			setColor(HIGHLIGHT_COLOR);
			printf(" %c%4d%c ", formatting, startYear + year, formatting);
			setColor(CALENDAR_COLOR);
		}
		else
			printf(" %c%4d%c ", formatting, startYear + year, formatting);
	}
	printf("   \n");
	for (int i = 0; i < 5; i++)
		fillLine(35);
}

// utility functions
bool isLeapYear(int year)
{
	if (year % 400 == 0)
		return true;

	if (year % 100 == 0)
		return false;
	
	if (year % 4 == 0)
		return true;

	return false;
}

void AddDate(tm& date, int dayCount, int monthCount, int yearCount)
{
	date.tm_mday += dayCount;
	date.tm_mon += monthCount;
	date.tm_year += yearCount;
	if (date.tm_year < 0)
		date.tm_year = 0;
	mktime(&date);
}
