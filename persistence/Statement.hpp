#ifndef _Statement_HG_
#define _Statement_HG_

#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <typeinfo>

class Statement {
public:
	enum action_t {
		inserts,
		updates,
		deletes,
		selects
	};

	Statement(action_t action, std::string table) : _action(action), _table(table) {}

	Statement & string(std::string column, std::string & value);
	Statement & number(std::string column, int value);
	Statement & decimal(std::string column, double value);
	Statement & where(std::string column, std::string value);
	Statement & where(std::string column, int value);
	Statement & where(std::string column, double value);

private:
	struct item {
		template<class T>
		item(std::string _column, T * _value) :
			column(_column),
			value((void *)_value)
		{ 
			type_hash = typeid(T).hash_code();
		}

		std::string column;
		size_t type_hash;
		void * value;
	};

	std::vector<item> _columns;
	action_t _action;
	std::string _table;
	// Could make a vector to have multiple where's.
	// Add in operator...
	std::pair<std::string, std::string> _where;


public:
	action_t action() { return this->_action; }
	std::string table() { return this->_table; }
	unsigned int count() { return this->_columns.size(); }

	std::string columns();
	std::string where();
	void populate(std::function<void(int, size_t, void *)>f);
};



#endif // !_Statement_HG_
