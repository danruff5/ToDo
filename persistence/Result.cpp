#include "Result.hpp"

int Result::number(int column, int row) {
	return *((int *)data[row][column]);
}
double Result::decimal(int column, int row) {
	return *((double *)data[row][column]);
}
std::string Result::string(int column, int row) {
	char * * v = (char * *)data[row][column];
	return std::string(*v);
}

void Result::add_number(int * value) {
	data[row].push_back((void *)value);
}

void Result::add_decimal(double * value) {
	data[row].push_back((void *)value);
}

void Result::add_string(std::string & value) {
	char * v = new char[value.length()];
	strcpy(v, value.c_str());

	char * * v2 = new char *;
	*v2 = v;

	data[row].push_back((void *)v2);
}

void Result::add_row() {
	data.push_back(std::vector<void *>());
	++row;
}