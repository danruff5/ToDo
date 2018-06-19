#include "Statement.hpp"

Statement & Statement::string(std::string column, std::string & value) {
	char * v = new char[value.length()];
	strcpy(v, value.c_str());

	char * * v2 = new char *;
	*v2 = v;

	item i(column, v2);
	this->_columns.push_back(i);
	return *this;
}

Statement & Statement::number(std::string column, int value) {
	int * v = new int;
	*v = value;
	item i(column, v);
	this->_columns.push_back(i);
	return *this;
}

Statement & Statement::decimal(std::string column, double value) {
	double * v = new double;
	*v = value;
	item i(column, v);
	this->_columns.push_back(i);
	return *this;
}

Statement & Statement::where(std::string column, std::string value) {
	_where = std::pair<std::string, std::string>(column, value);
	return *this;
}

Statement & Statement::where(std::string column, int value) {
	std::ostringstream oss;
	oss << value;

	_where = std::pair<std::string, std::string>(column, oss.str());
	return *this;
}

Statement & Statement::where(std::string column, double value) {
	std::ostringstream oss;
	oss << value;

	_where = std::pair<std::string, std::string>(column, oss.str());
	return *this;
}

void Statement::populate(std::function<void(int, size_t, void *)>f) {
	for (int i = 0; i < this->_columns.size(); ++i) {
		f(i + 1, this->_columns[i].type_hash, this->_columns[i].value);
	}
}

std::string Statement::columns() {
	std::ostringstream oss;
	bool first = true;

	for (int i = 0; i < _columns.size(); ++i) {
		if (!first) {
			oss << ", ";
		}
		first = false;
		oss << _columns[i].column;
	}

	return oss.str();
}

std::string Statement::where() {
	std::ostringstream oss;
	// Change operator...
	oss << _where.first << " = " << _where.second;

	return oss.str();
}
