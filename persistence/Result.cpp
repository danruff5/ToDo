#include "Result.hpp"

int Result::number(int column, int row) {
	return *((int *)data[row][column]);
}
double Result::decimal(int column, int row) {
	return *((double *)data[row][column]);
}
std::string Result::string(int column, int row) {
	return std::string((const char *)data[row][column]);
}

void Result::add_number(int * value) {
	data[row].push_back((void *)value);
}

void Result::add_decimal(double * value) {
	data[row].push_back((void *)value);
}

void Result::add_string(std::string & value) {
	data[row].push_back((void *)value.c_str());
}

void Result::add_row() {
	data.push_back(std::vector<void *>());
	++row;
}