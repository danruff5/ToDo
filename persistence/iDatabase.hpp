#ifndef _iDatabase_HG_
#define _iDatabase_HG_

#include "Statement.hpp"
#include <string>

class iDatabase {
public:
	virtual ~iDatabase() { }

	virtual void execute(std::string sql) = 0;
	virtual void execute(Statement stmt) = 0;
};

#endif // !_iDatabase_HG_
