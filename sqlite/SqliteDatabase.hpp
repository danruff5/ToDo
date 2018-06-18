#ifndef _SqliteDatabase_HG_
#define _SqliteDatabase_HG_

#include "../external/headers/sqlite/sqlite3.h"
#include <iDatabase.hpp>
#include <string>

class SqliteDatabase : public iDatabase {
	sqlite3 * db;
public:
	virtual ~SqliteDatabase();

	SqliteDatabase(std::string filename);

	virtual void execute(std::string sql);
	virtual Result execute(Statement stmt);
};

#endif // !_SqliteDatabase_HG_
