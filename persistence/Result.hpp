#ifndef _Result_HG_
#define _Result_HG_

#include <string>
#include <vector>

class Result {
	std::vector<std::vector<void *>> data;
	size_t row = -1; // add row first then index becomes 0.
public:
	int number(int column, int row = 0);
	double decimal(int column, int row = 0);
	std::string string(int column, int row = 0);

	void add_number(int * value);
	void add_decimal(double * value);
	void add_string(std::string & value);
	void add_row();
};

#endif // !_Result_HG_
