#include "SqliteDatabase.hpp"

#include <sstream>

SqliteDatabase::~SqliteDatabase() {
	sqlite3_close(db);
}

SqliteDatabase::SqliteDatabase(std::string filename) {
	int result = sqlite3_open(filename.c_str(), &db);
	if (result) {
		// TODO: better error handling...
		printf("SQLite error: %s\n", sqlite3_errmsg(db));
		printf("Failed to open the sqlite database!\n");
	}
}

// TODO: make a better callback to collect the data into a response...
int SQLiteCallback(void * v, int argc, char * * argv, char * * azColName) {
	for (int i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

void SqliteDatabase::execute(std::string sql) {
	char * errMsg = 0;
	if (sqlite3_exec(db, sql.c_str(), SQLiteCallback, 0, &errMsg) != 0) {
		// TODO: better error handling.
		printf("%s\n", errMsg);
	}
}

void SqliteDatabase::execute(Statement stmt) {
	// Use statement to generate the sql.

	std::ostringstream oss;

	switch (stmt.action()) {
		//case Statement::action_t::selects:
		// SELECT columns, ... FROM table WHERE column = ?1 [AND|OR] ...;
		case Statement::action_t::inserts:
			// INSERT INTO table (columns) VALUES (?1, ...);
			oss << "INSERT INTO " << stmt.table() << stmt.columns() << " VALUES (";
			for (int i = 0; i < stmt.count(); ++i) {
				if (i != 0) oss << ", ";
				oss << "?" << i;
			}
			oss << ");";
		case Statement::action_t::updates:
			// UPDATE table SET column = ?1, ... WHERE column = ?2 [AND|OR] ...;

		case Statement::action_t::deletes:
			// DELETE FROM table WHERE column = ?1 [AND|OR] ...;

	}

	// Use statment to populate the statment.

	// Execute (and retrieve data....).
}
