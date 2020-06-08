// visual studio community 2019, windows 7 32-bit

// bibliography: stackoverflow, msdn on setTextAttribute

#define _CRT_SECURE_NO_WARNINGS

#define COLORED

#include <stdio.h>	// has functions: printf
#include <time.h>	// has functions: localtime and struct tm
#include <conio.h>	// has functions: _getch
#include <stdlib.h>	// has functions: system("cls")
#include <Windows.h>// has functions: SetConsoleTextAttribute for color
#include <string.h>	// has functions: strcmp, strcpy

// Global Variables
HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

enum Keycode{
	Left=75, Right=77, Up=72, Down=80, Enter=13, Esc=27, Backspace=8, Tab=9, Tild=96, Space=' ',
	Delete=83
};
int mon_day[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
const char* month_name[12] = {
	"January", "February", "March", "April", "May", "June", "July",
	"August", "September", "October", "November", "December"
};
const char* day_name[7] = { "Sunday", "Monday", "Tuesday", "Wednessday", "Thursday", "Friday", "Saturday" };
const char* instruction[] ={
	"  Days ",
	" Months",
	" Years ",
	" Notes ",
	" Exit  "
};

enum Calendar {
	Days=0,
	Months,
	Years,
	Notes,
	EXIT
};

// Note structure (Linked List)
struct Note {
	char header[36] = "";
	char body[101] = "";
	tm date;
	Note* next = NULL;
}*note = NULL;
// number of notes
int num_notes = 0;

// note utilities
bool input_note(tm& date);
void output_note(tm& date);
void add_note(Note& n);
void delete_note(tm& date);
const Note* get_note(tm& date);
bool has_a_note(tm& date);
bool has_a_note(int d, int m, int y);
//void save_notes();

// Function Declarations
int print_instructions(int input, bool has_focus, bool& option_flag);
void instructions_screen(int pointed_instruction, int selected_instruction, bool has_focus);

void print_days(tm& date, int input);
void days_screen(tm& date);

void print_months(tm& date, int input);
void months_screen(tm& date);

void print_years(tm& date, int input);
void years_screen(tm& date, int steps = 5, int range = 8);

bool isLeapYear(int year);
void AddDate(tm& date, int dayCount, int monthCount = 0, int yearCount = 0);
int cmpdate(tm& date1, tm& date2);
// coloring accessories
inline void setColor(WORD color)
{
#ifdef COLORED
	SetConsoleTextAttribute(console_handle, color);
#endif
}
inline void fillLine(int length, char ch=' ')
{
#ifdef COLORED
	for (int i = 0;i < length; i++)
		printf("%c",ch);
#endif
	printf("\n");
}
#define DEFAULT_COLOR (FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE)
#define CALENDAR_COLOR (FOREGROUND_RED | FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)
#define HIGHLIGHT_COLOR (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_GREEN)

// file read/write print utilities
void print_notes(int input);
bool read_json(FILE* file);
bool write_json(FILE* file);






// today's date
tm today;
int calendar_mod = Days;

int main()
{
	// ----------------------------Initail Setup------------------------------

	FILE* note_file;
	fopen_s(&note_file, "calendar_notes.json", "r+");
	// if we have no note saved so that file fails to open
	if (note_file == NULL)
	{
		// create new file for saving notes
		fopen_s(&note_file, "calender_notes.json", "w+");
	}
	read_json(note_file);

	time_t now = time(NULL);
	localtime_s(&today, &now);
	tm date = today;

	//bool quit = false;
	//int calendar_mod = Days;
	int input = 0;
	bool option_selected = false;
	bool quit_app = false;
	// -----------------------------------Application loop----------------------------
	while(!quit_app)
	{
		int option_input = (option_selected ? input : 0);
		int calendar_input = (option_selected ? 0 : input);

		//// set no of days in Feb for the give year
		//int year = date.tm_year + 1900;
		//mon_day[1] = (isLeapYear(year) ? 29 : 28);
		//
		// clear screen
		//system("cls");
		// print calendar
		//setColor(CALENDAR_COLOR);

		// clear screen
		system("cls");

		// print screen
		calendar_mod = print_instructions(option_input, (option_selected ? true : false), option_selected);

		switch ( calendar_mod )
		{
		case Days:
			print_days(date, calendar_input);
			break;
		case Months:
			print_months(date, calendar_input);
			break;
		case Years:
			print_years(date, calendar_input);
			break;
		case Notes:
			print_notes(calendar_input);
			break;
		case EXIT:
			quit_app = true;
			continue;
		}

		// print instructions
		//setColor(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE|BACKGROUND_BLUE);
		//fillLine(35);
		//printf(
		//	"           Instructions            "
		//	"\n%-35s\n%-35s\n%-35s\n%-35s",
		//	"Escape(Esc) -> Quit",
		//	"Tab -> change Calendar-Mode",
		//	"Arrow-Keys -> Navigation",
		//	(
		//		calendar_mod==Days ?
		//		(has_a_note(date)?"Enter-> See Notes ":"Enter -> Add Notes") :
		//		"Enter -> Change Calendar Mode Back"
		//	)
		//);
		//printf("\n");
		//fillLine(35);
		//fillLine(35);
		//setColor(DEFAULT_COLOR);

		// process input
		//if(input != Esc)
		switch (input = _getch())
		{
		case Tab:
			option_selected = !option_selected;
			//printf("Tab Pressed: options %sselected", (option_selected ? "" : "not "));
			//_getch();
			break;
		case Esc:	// Escape Pressed
			if (!write_json(note_file))
			{
				printf("\nFailed to save notes!");
				printf("\nPress Enter to exit...");
				if (_getch() != Enter)
				{
					continue;
				}
			}
			quit_app = true;
			break;
		}
		// process input
		//input = _getch();
		//switch (input)
		//{
		//case Left:	// Left Arrow-key pressed
		//	switch (calendar_mod)
		//	{
		//	//case Days:
		//	//	AddDate(date, -1, 0, 0);
		//		//break;
		//	//case Months:
		//	//	AddDate(date, 0, -1, 0);
		//	//	break;
		//	case Years:
		//		AddDate(date, 0, 0, -1);
		//		break;
		//	}
		//	break;
		//case Right:	// Right arrow-key pressed
		//	switch (calendar_mod)
		//	{
		//	//case Days:
		//	//	AddDate(date, 1, 0, 0);
		//		//break;
		//	//case Months:
		//	//	AddDate(date, 0, 1, 0);
		//	//	break;
		//	case Years:
		//		AddDate(date, 0, 0, 1);
		//		break;
		//	}
		//	break;
		//case Up:	// Up arrow-key pressed
		//	switch (calendar_mod)
		//	{
		//	//case Days:
		//	//	AddDate(date, -7, 0, 0);
		//		//break;
		//	//case Months:
		//	//	AddDate(date, 0, -month_step, 0);
		//	//	break;
		//	case Years:
		//		AddDate(date, 0, 0, -year_step);
		//		break;
		//	}
		//	break;
		//case Down:	// Down arrow-key pressed
		//	switch (calendar_mod)
		//	{
		//	//case Days:
		//	//	AddDate(date, 7, 0, 0);
		//	//	break;
		//	//case Months:
		//	//	AddDate(date, 0, month_step, 0);
		//	//	break;
		//	case Years:
		//		AddDate(date, 0, 0, year_step);
		//		break;
		//	}
		//	break;
		//case Tab:	// Tab pressed
		//	if (++calendar_mod == None)
		//	{
		//		calendar_mod = Days;
		//	}
		//	break;
		//case Enter:
		//	switch (calendar_mod)
		//	{
			//case Days:
			//	if (has_a_note(date))
			//	{
			//		output_note_screen(date);
			//	}
			//	else
			//	{
			//		input_note_screen(date);
			//	}
			//	break;
			//case Months:
			//	calendar_mod = Days;
			//	break;
			//case Years:
			//	calendar_mod = Months;
			//	break;
			//}
			//break;
		//case Esc:	// Escape Pressed
		//	if (!write_json(note_file))
		//	{
		//		printf("\nFailed to save notes!");
		//		printf("\nPress Enter to exit...");
		//		if (_getch() != Enter)
		//		{
		//			continue;
		//		}
		//	}
		//	//quit = true;
		//	break;
		//}
	}
	// End of application
	if(note_file) fclose(note_file);

	return 0;
}





// ------------------------------Function Definitions-----------------------------
int print_instructions(int input, bool has_focus, bool& option_flag)
{
	static int selected_instruction = Notes;
	static int pointed_instruction = selected_instruction;
	const int max_instructions = sizeof(instruction) / sizeof(char*);

	// process input
	switch (input)
	{
	case Up:
	case Left:
		if (pointed_instruction > 0)
		{
			pointed_instruction--;
		}
		break;
	case Down:
	case Right:
		if (pointed_instruction < max_instructions - 1)
		{
			pointed_instruction++;
		}
		break;
	case Enter:
		selected_instruction = pointed_instruction;
		option_flag = !option_flag;
		has_focus = false;
		break;
	}

	// show screen
	instructions_screen(pointed_instruction, selected_instruction, has_focus);
	return selected_instruction;
}
void instructions_screen(int pointed_instruction, int selected_instruction, bool has_focus)
{
	const int num_instructions = sizeof(instruction) / sizeof(char*);

	//setColor((FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) | BACKGROUND_BLUE);
	
	//printf("               Menu                \n");
	
	//setColor((FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) | BACKGROUND_BLUE);

	WORD focus_formatting = ( has_focus ? FOREGROUND_INTENSITY | BACKGROUND_INTENSITY : 0 );

	for (int i = 0; i < num_instructions; i++)
	{
		// set color
		if (i == pointed_instruction) { setColor(focus_formatting | (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) | (BACKGROUND_GREEN | BACKGROUND_BLUE)); }
		else if (i == selected_instruction) { setColor(focus_formatting | (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE) | FOREGROUND_BLUE);	}
		else { setColor(focus_formatting | (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) | BACKGROUND_BLUE); }

		// print instruction
		printf("%s", instruction[i]);
	}
	printf("\n");

	setColor(DEFAULT_COLOR);
}

void print_days(tm& date, int input)
{
	// set no of days in Feb for the give year
	int year = date.tm_year + 1900;
	mon_day[1] = (isLeapYear(year) ? 29 : 28);

	// process input
	switch (input)
	{
	case Up:
		AddDate(date, -7, 0, 0);
		break;
	case Down:
		AddDate(date, 7, 0, 0);
		break;
	case Left:
		AddDate(date, -1, 0, 0);
		break;
	case Right:
		AddDate(date, 1, 0, 0);
		break;
	case Enter:
		if (has_a_note(date))
		{
			output_note(date);
			system("cls");

		}
		else
		{
			input_note(date);
			system("cls");
		}
		break;
	}

	// show screen
	days_screen(date);
}
void days_screen(tm& date)
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
			printf(" %c%2d%c", formatting, month_day, formatting);
			setColor(CALENDAR_COLOR);
		}
		else if (has_a_note(month_day, date.tm_mon, date.tm_year))
		{
			setColor((FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE) | (BACKGROUND_RED|BACKGROUND_BLUE));
			printf(" %c%2d%c", formatting, month_day, formatting);
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
	// ALL DAYS ARE PRINTED AND ALIGNED
	if (has_a_note(date))
	{
		printf("%-35s\n", get_note(date)->header);
	}
	else
	{
		fillLine(35);
	}

	setColor(DEFAULT_COLOR);
}

void print_months(tm& date, int input)
{
	// process input
	switch (input)
	{
	case Up:
		AddDate(date, 0, -3, 0);
		break;
	case Down:
		AddDate(date, 0, 3, 0);
		break;
	case Left:
		AddDate(date, 0, -1, 0);
		break;
	case Right:
		AddDate(date, 0, 1, 0);
		break;
	}

	// draw screen
	months_screen(date);
}
void months_screen(tm& date)
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
	for (int i = 0; i < 6; i++)
	{
		fillLine(35);
	}

	setColor(DEFAULT_COLOR);
}

void print_years(tm& date, int input)
{
	// process input
	switch (input)
	{
	case Up:
		AddDate(date, 0, 0, -4);
		break;
	case Down:
		AddDate(date, 0, 0, 4);
		break;
	case Left:
		AddDate(date, 0, 0, -1);
		break;
	case Right:
		AddDate(date, 0, 0, 1);
		break;
	}

	// draw screen
	years_screen(date, 4);
}
void years_screen(tm& date, int steps, int range)
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
	for (int i = 0; i < 6; i++)
	{
		fillLine(35);
	}

	setColor(DEFAULT_COLOR);
}

// --------------------------------File Read/Write/Show---------------------------
bool write_json(FILE* file)
{
	//Note n;
	//n.date.tm_mday = 0;
	//n.date.tm_mon = -1;
	//n.date.tm_year = -1900;
	//strcpy(n.header, "Dummy-Header");
	//strcpy(n.body, "Dummy-Body");
	//add_note(n);

	fseek(file, 0, SEEK_SET);

	if (file != NULL)
	{
		fseek(file, 0, SEEK_SET);
		fprintf(file, "{");
		for (Note* note_node = ::note; note_node != NULL; note_node = note_node->next)
		{
			// write date
			fprintf(file, "\n    \"%02d-%02d-%04d\":",
				note_node->date.tm_mday, note_node->date.tm_mon + 1, 1900 + note_node->date.tm_year
			);

			// make sure to terminate the header
			note_node->header[35] = '\0';
			// write header
			fputc('\"', file);
			for (char* ch = note_node->header; *ch != '\0'; ch++)
			{
				if (*ch == ' ') fputc((int)'-', file);
				else fputc((int)*ch, file);
			}
			fputc(':', file);

			// make sure to terminate the body
			note_node->body[100] = '\0';
			// write body
			for (char* ch = note_node->body; *ch != '\0'; ch++)
			{
				if (*ch == ' ') fputc((int)'-', file);
				else fputc((int)*ch, file);
			}
			fputc('\"', file);

			// add ',' at the end if not the last note
			if (note_node->next != NULL)
			{
				fputc(',', file);
			}
		}
		fprintf(file, "\n}");
		fprintf(file,
			"\n\n// comment area--------------------------------------"
			"\n// the key must be of the form dd-mm-yyyy where"
			"\n// dd is day, mm is months - 1 and yyyy is year - 1900"
			"\n// and value must be of the form sssss*:ssssss*"
		);

		//fclose(file);

		return true;
	}
	
	return false;
}
void print_notes(int input)
{
	static int line = 0;
	const int note_limit = 2;

	Note* note_node = ::note;

	// process input
	switch (input)
	{
	case Backspace:
		break;
	case Up:
		if (line > 0)
		{
			line--;
		}
		break;
	case Down:
		if (line < num_notes - 2)
		{
			line++;
		}
		break;
	}

	for (int i = 0; i < line; i++, note_node = note_node->next);

	// ----------------------------------Header------------------------------------
	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_RED);

	fillLine(35);
	printf("---------------Notes---------------\n");
	fillLine(35);

	// ----------------------------------Body--------------------------------------

	for (int i = 0; i < note_limit && note_node != NULL; i++, note_node = note_node->next)
	{
		// print date
		setColor(CALENDAR_COLOR);
		fillLine(35);
		printf("Date:%02d-%02d-%04d--------------------\n",
			note_node->date.tm_mday, note_node->date.tm_mon + 1, 1900 + note_node->date.tm_year);
		fillLine(35);
			
		// print note-header
		setColor((FOREGROUND_GREEN) | (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE));
		printf("%-35s\n", note_node->header);
		setColor(CALENDAR_COLOR);
			
		// print note-body
		fillLine(35);
		setColor((FOREGROUND_BLUE) | (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE));
		//printf("%-35s\n", note_node->body);
		for (int i = 0; i < 35; i++) _putch(' ');
		for (int i = 0; i < 35; i++) _putch('\b');
		
		unsigned int cursor_x = 0;
		for (unsigned int i = 0; i < strlen(note_node->body); i++, cursor_x++)
		{
			// if its the end of the line - move to the next line
			if (cursor_x == 35)
			{
				unsigned int j = i-1;
				// if a word extends beyond line
				if (note_node->body[j-1] != ' ')
				{
					// move back until we find a space or the beginning of the line
					for (; j > 0u; j--)
					{
						if (note_node->body[j] == ' ')
							break;
					}
					// if a space is found
					if (j != 0u)
					{
						// replace the extending word with empty spaces
						for (unsigned int k = 0;k < i - j; k++)	_putch('\b');
						for (unsigned int k = 0;k < i - j; k++)	_putch(' ');
						// and print the word in the next line
						_putch('\n');
						while (j < i) _putch(note_node->body[j++]);
						// update cursor position
						// cursor_x = i - j - 1;
						cursor_x = 0;
					}
					// if its a line without spaces
					else
					{
						_putch('\n');
						cursor_x = 0;
					}
				}
				else // move to next line
				{
					_putch('\n');
					cursor_x = 0;
				}
				//for (int s = cursor_x+1; s < 35; s++) _putch(' ');	// apply color to the line
				//for (int s = cursor_x+1; s < 35; s++) _putch('\b');	// place cursor at the beginning of the line
			}
			// print body of the note word by word
			_putch(note_node->body[i]);
		}
		// fill line
		//while (cursor_x++ < 35) _putch(' ');
		printf("%d", cursor_x);

		printf("\n");
		setColor(CALENDAR_COLOR);
		fillLine(35);
	}

	// input processing
		//input = _getch();
		//switch (input)
		//{
		//case Backspace:
		//	break;
		//case Up:
		//	if (line > 0)
		//	{
		//		line--;
		//	}
		//	break;
		//case Down:
		//	if (note_node != NULL)
		//	{
		//		line++;
		//	}
		//}
	//} while (input != Backspace);

	setColor(DEFAULT_COLOR);
}
bool read_json(FILE* file)
{
	Note n;
	char string[100] = "";

	//FILE* file = fopen("calendar_notes.json", "r");
	fseek(file, 0, SEEK_SET);

	if (file != NULL)
	{
		fseek(file, 0, SEEK_SET);

		char ch;
		fscanf(file, "%c", &ch);
		if (ch != '{')
		{
			return false;
		}
		bool file_read = false;
		while (!file_read)
		{
			fscanf(file, "\n\"%d-%d-%d\":%s",
				&n.date.tm_mday, &n.date.tm_mon, &n.date.tm_year, string
			);
			n.date.tm_mon--;
			n.date.tm_year -= 1900;
			// segregate header and body
			char* reader = n.header;
			for (char* ch = string; *ch != '\0'; ch++)
			{
				switch (*ch)
				{
				case ':':
					*reader = '\0';	// terminate header
					reader = n.body;
					break;
				case '"':
					break;
				default:
					if (*ch == '-')
					{
						*reader = ' ';
					}
					else
					{
						*reader = *ch;
					}
					reader++;
				}
			}
			if (*(reader - 1) != ',')
			{
				*reader = '\0';
				file_read = true;
			}
			else
			{
				*(reader - 1) = '\0';
			}
			add_note(n);
		}

		//fclose(file);

		return true;
	}
	
	return false;
}

// ------------------------------date utility functions------------------------------
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
int cmpdate(tm& date1, tm& date2)
{
	// compare years
	if (date1.tm_year < date2.tm_year) return -1;
	else if (date1.tm_year > date2.tm_year) return 1;
	// compare months
	else if (date1.tm_mon < date2.tm_mon) return -1;
	else if (date1.tm_mon > date2.tm_mon) return 1;
	// compare days
	else if (date1.tm_mday < date2.tm_mday) return -1;
	else if (date1.tm_mday > date2.tm_mday) return 1;
	// both dates are equal
	else return 0;
}

// ------------------------------------note utilities------------------------------

bool input_note(tm& date)
{
	Note new_note;
	new_note.date = date;
	int input, inch;// input character
	bool quit = false;

	// clear screen
	system("cls");

	// -------------------------------------Header--------------------------------------
	
	setColor(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE|BACKGROUND_RED|FOREGROUND_INTENSITY);

	fillLine(35);
	printf("-------------Add Note--------------\n");
	fillLine(35);
	printf("Date : %9s %2d, %4d          \n", month_name[date.tm_mon], date.tm_mday, 1900+date.tm_year);
	fillLine(35);
	setColor(CALENDAR_COLOR);
	printf("Enter Header(no whitespaces-max35):\n");
	fillLine(35);
	// color the input line
	for (int i = 0; i < 35; i++) printf(" ");
	for (int i = 0; i < 35; i++) printf("\b");
	// input header
	inch = 0; quit = false;
	while (!quit)
	{
		// take input
		input = _getch();
		// process input
		switch (input)
		{
		case Up: case Down: case Left: case Right: break;
		case Enter:
			if (inch == 0)
			{
				setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_GREEN);
				printf("< enter-something >");
				_getch();
				setColor(CALENDAR_COLOR);
				printf(
					"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
					"                   "
					"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
				);
				break;
			}
			quit = true;
			printf("\n");
			break;
		//case Space:
		//case Tab:
		//	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_RED);
		//	printf("<no whitespaces>");
		//	_getch();
		//
		//	printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
		//	setColor(CALENDAR_COLOR);
		//	for (int i = 0; i < 16; i++)
		//	{
		//		if (i > 35 - inch - 1)	setColor(DEFAULT_COLOR);
		//		printf(" ");
		//	}
		//	printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
		//	setColor(CALENDAR_COLOR);

		//	break;
		case Backspace:
			if (inch > 0)
			{
				printf("\b \b");
				inch--;
			}
			break;
		case Esc:
			setColor(DEFAULT_COLOR);
			return false;
		default:
			if (inch >= 35)
			{
				setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_RED);
				printf("<max-limit-reached>");
				setColor(DEFAULT_COLOR);
				_getch();
				printf(
					"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
					"                   "
					"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
				);
				setColor(CALENDAR_COLOR);
				break;
			}
			printf("%c", (char)input);
			new_note.header[inch++] = (char)input;
		}
	}
	new_note.header[inch] = '\0';

	fillLine(35);
	printf("Enter Note Body ( max - 100 ):     \n");
	fillLine(35);

	// color the input line
	for (int i = 0; i < 35; i++) printf(" ");
	for (int i = 0; i < 35; i++) printf("\b");
	// input body
	inch = 0;
	quit = false;
	while (!quit)
	{
		// take input
		input = _getch();

		// process input
		switch (input)
		{
		case Enter:
			if (inch == 0)
			{
				setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_GREEN);
				printf("< enter-something >");
				_getch();
				setColor(CALENDAR_COLOR);
				printf(
					"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
					"                   "
					"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
				);
				break;
			}
			printf("\n");
			quit = true;
			break;
		case Backspace:
			if (inch > 0)
			{
				printf("\b \b");
				inch--;
			}
			break;
		case Esc:
			setColor(DEFAULT_COLOR);
			return false;
		default:
			if (!(inch < 100))
			{
				printf("<max-limit-reached>");
				_getch();
				printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
				printf("    ");
				setColor(DEFAULT_COLOR);
				printf("               ");
				printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
				setColor(CALENDAR_COLOR);
				break;
			}
			if (inch == 34 || inch == 69)
			{
				printf("\n                                   ");
				printf(
					"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
					"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
				);
			}
			printf("%c", (char)input);
			new_note.body[inch++] = (char)input;
		}
	}
	new_note.body[inch] = '\0';
	
	fillLine(35);

	add_note(new_note);
	printf("Note Added! Press Enter to return..\n");
	while ((input = _getch()) != Enter);

	fillLine(35);

	setColor(DEFAULT_COLOR);
	
	return true;
}
void output_note(tm& date)
{
	auto my_note = get_note(date);
	if (my_note == NULL)
	{
		printf("Failed to load note");
		return;
	}

	// clear screen
	system("cls");

	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED);

	fillLine(35);
	printf("----------------Note---------------\n");
	fillLine(35);

	printf("Date : %9s %2d, %4d          \n", month_name[date.tm_mon], date.tm_mday, 1900 + date.tm_year);
	fillLine(35);
	setColor(CALENDAR_COLOR);
	fillLine(35);

	// print header
	printf("%-35s\n", my_note->header);

	// print body
	for (int i = 0;i < (int)strlen(my_note->body); i++)
	{
		if (i == 0 || (i+1)%35 == 0)
		{
			fillLine(35);
			//printf("                                   ");
			//for (int i = 0; i < 35; i++) printf("\b");
		}
		printf("%c", my_note->body[i]);
	}
	printf("\n");
	fillLine(35);
	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_BLUE);
	fillLine(35);
	printf("Press Backspace to delete...       \n");
	fillLine(35);
	printf("Press Enter to return...           \n");
	fillLine(35);
	for (int i = 0; i < 35; i++) printf("\b");

	int input;
	while ((input = _getch()) != Enter)
	{
		if (input == Backspace)
		{
			printf("Press Enter to confirm delete:    ");
			input = _getch();
			for (int i = 0; i < 35; i++) printf("\b");
			
			if (input == Enter)
			{
				delete_note(date);
				fillLine(35);
				printf("Note Deleted Successfully!        \n");
				_getch();
				break;
			}
		}
	}

	setColor(DEFAULT_COLOR);
}

void add_note(Note& n)
{

	// update notes-list size
	num_notes++;
	
	Note* newNote = new Note;
	strcpy_s(newNote->header, n.header);
	strcpy_s(newNote->body, n.body);
	newNote->date = n.date;

	if (::note == NULL)
	{
		::note = newNote;
		newNote = NULL;
	}
	else
	{
		Note* noteNode = ::note;
		while (noteNode->next != NULL)
		{
			noteNode = noteNode->next;
		}
		noteNode->next = newNote;
		newNote = NULL;
	}
}
void delete_note(tm& date)
{
	// update notes-list size
	if (num_notes > 0) num_notes--;

	// if the list of notes is already empty then there's nothing to delete
	if (::note == NULL)
		return;
	// if the first note is the one we want to delete
	else if (cmpdate(::note->date, date) == 0)
	{
		Note* note_to_delete = ::note;	// take out the first note
		::note = ::note->next;			// make second note as the first note

		delete note_to_delete;			// delete the required note
		// return;
	}
	else {
		Note* noteBefore = ::note;		// note that is before the note that we want to delete
		Note* noteNode = ::note;		// note that we want to delete

		// stop looping when we find the note we want to delete or when
		// there is nothing to loop (the list is exhausted)
		while (noteNode != NULL && cmpdate(noteNode->date, date) != 0)
		{										// in every loop,
			noteBefore = noteNode;				// noteBefore becomes the noteNode and
			noteNode = noteNode->next;			// noteNode moves ahead
		}

		// if we found the note that we want to delete
		if (noteNode != NULL)
		{
			Note* note_to_delete = noteNode;	// take out the note
			// make the note before 'to-be-deleted' note to point to the node after 'to-be-deleted' note
			// thus excluding our note from the note-list
			noteBefore->next = noteNode->next;

			// delete the note
			delete note_to_delete;
		}
	}
	// return;
}
const Note* get_note(tm& date)
{
	for (Note* noteNode = ::note; noteNode != NULL; noteNode = noteNode->next)
	{
		if (cmpdate(date, noteNode->date) == 0)
		{
			return noteNode;
		}
	}
	return NULL;
}
bool has_a_note(tm& date)
{
	for (Note* noteNode = ::note; noteNode != NULL; noteNode = noteNode->next)
	{
		if (cmpdate(date, noteNode->date) == 0)
		{
			return true;
		}
	}

	return false;
}
bool has_a_note(int d, int m, int y)
{
	tm date;
	date.tm_mday = d;
	date.tm_mon = m;
	date.tm_year = y;

	return has_a_note(date);
}