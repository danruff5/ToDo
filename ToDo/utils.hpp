#ifndef _utils_HG_
#define _utils_HG_

#include <stdio.h>
#include <Windows.h>
#include <fstream>
#include <string>

namespace util {

	extern HANDLE hStdout;
	extern CONSOLE_CURSOR_INFO lpCursor;
	extern COORD coord;
	extern int max_number_of_rows;
	extern char buffer[];


	void start_text();
	void end_text();
	void print_text(const char* text, ...);
}

#endif // !_utils_HG_