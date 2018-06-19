#include "utils.hpp"
#include <string>

HANDLE util::hStdout;
CONSOLE_CURSOR_INFO util::lpCursor;
COORD util::coord = { 0,0 };
int util::max_number_of_rows = 10;
char util::buffer[1024];

void util::start_text() {
	GetConsoleCursorInfo(hStdout, &lpCursor);
	lpCursor.bVisible = false;
	SetConsoleCursorInfo(hStdout, &lpCursor);

	coord.Y = 0;
}
void util::end_text() {
	coord.Y = 0;
}

//Improve this funciton as needed....
void util::print_text(const char* text, ...) {
	va_list argptr;
	va_start(argptr, text);
	vsprintf(buffer, text, argptr);
	va_end(argptr);

	int len = strlen(buffer);

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleCursorPosition(hStdout, coord);
	coord.Y++;
	WriteConsoleA(hStdout, "", 1, NULL, NULL);
	WriteConsoleA(hStdout, buffer, len, NULL, NULL);

	//memset(&buffer[0], 0, sizeof(buffer));
}