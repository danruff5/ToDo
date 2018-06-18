#ifndef _iDatabase_HG_
#define _iDatabase_HG_

#include "Statement.hpp"
#include "Result.hpp"
#include <string>

class iDatabase {
public:
	virtual ~iDatabase() { }

	virtual void execute(std::string sql) = 0;
	virtual Result execute(Statement stmt) = 0;
};

#endif // !_iDatabase_HG_
