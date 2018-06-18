#ifndef _Result_HG_
#define _Result_HG_

#include <string>

class Result {

public:
	long long number(int column, int row = 0);
	double decimal(int column, int row = 0);
	std::string string(int column, int row = 0);

	void add_number(long long value);
	void add_decimal(double value);
	void add_string(std::string value);
	void add_row();
};

#endif // !_Result_HG_
