#ifndef _Statement_HG_
#define _Statement_HG_

#include <string>
#include <vector>
#include <sstream>
#include <functional>

class Statement {
public:
	enum action_t {
		inserts,
		updates,
		deletes,
		selects
	};

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
		void * value;
	};

	std::vector<item> _columns;
	action_t _action;
	std::string _table;


public:
	action_t action() { return this->_action; }
	std::string table() { return this->_table; }
	unsigned int count() { return this->_columns.size(); }
	void populate(std::function<void (int, std::string, void *)>f) {
		for (int i = 1; i <= this->_columns.size(); ++i) {
			f(i, this->_columns[i].type_name, this->_columns[i].value);
		}
	}

	std::string columns() { 
		std::ostringstream oss;
		bool first = true;

		for (item i : this->_columns) {
			if (!first) {
				oss << ", ";
				first = false;
			}
			oss << i.column;
		}

		return oss.str();
	}
};

#endif // !_Statement_HG_
