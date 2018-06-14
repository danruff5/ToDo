#ifndef _Statement_HG_
#define _Statement_HG_

#include <string>
#include <vector>
#include <sstream>

class Statement {
public:
	enum action_t {
		inserts,
		updates,
		deletes
	};

public:
	Statement(action_t action, std::string table);

	Statement & string(std::string column, std::string value);
	Statement & number(std::string column, long long value);
	Statement & decimal(std::string column, double value);

private:
	struct item {
		template<class T>
		item(std::string _column, T * _value) :
			column(_column),
			type_name(typeid(T).name()),
			value((void *)_value)
		{ }

		std::string column;
		std::string type_name;
		typename void * value;
	};

	std::vector<item> _columns;
	action_t _action;
	std::string _table;

public:
	action_t action() { return this->_action; }
	std::string table() { return this->_table; }
	unsigned int count() { return this->_columns.size(); }

	std::string & columns() { 
		std::ostringstream oss;
		oss << "(";
		bool first = true;

		for (item i : this->_columns) {
			if (!first) {
				oss << ", ";
				first = false;
			}
			oss << i.column;
		}

		oss << ")";
	}
};

#endif // !_Statement_HG_
