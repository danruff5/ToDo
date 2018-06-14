#ifndef _Statement_HG_
#define _Statement_HG_

#include <string>
#include <vector>

class Statement {
public:
	enum action {
		inserts,
		updates,
		deletes
	};

public:
	Statement(action action, std::string table);

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

	std::vector<item> columns;
	action action;
};

#endif // !_Statement_HG_
