#include "SqliteDatabase.hpp"
#include <Result.hpp>
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

Result SqliteDatabase::execute(Statement stmt) {
	// Use statement to generate the sql.

	std::ostringstream oss;

	switch (stmt.action()) {
		// SELECT columns, ... FROM table WHERE column = ?1 [AND|OR] ...;
		case Statement::action_t::inserts:
			// INSERT INTO table (columns) VALUES (?1, ...);
			oss << "INSERT INTO " << stmt.table() << "("  << stmt.columns() << ")" << " VALUES (";
			for (int i = 0; i < stmt.count(); ++i) {
				if (i != 0) oss << ", ";
				oss << "?" << i;
			}
			oss << ");";

			break;
		case Statement::action_t::updates:
			// UPDATE table SET column = ?1, ... WHERE column = ?2 [AND|OR] ...;

			break;
		case Statement::action_t::deletes:
			// DELETE FROM table WHERE column = ?1 [AND|OR] ...;

			break;
		case Statement::action_t::selects:
			// SELECT columns FROM table WHERE column = ?1 [AND|OR] ...;
			oss << "SELECT " << stmt.columns() << " FROM " << stmt.table();

			break;
	}

	// Use statment to populate the statment.
	sqlite3_stmt * s;
	sqlite3_prepare_v2(
		this->db,
		oss.str().c_str(),
		-1, // Length until first null terminator
		&s,
		NULL
	);

	stmt.populate([s] (int id, std::string type, void * value) {
		if (type == typeid(long long).name()) {
			sqlite3_bind_int(s, id, *((int *)value));
		} else if (type == typeid(double).name()) {
			sqlite3_bind_double(s, id, *((double *)value));
		} else if (type == typeid(std::string).name()) {
			sqlite3_bind_text(s, id, ((std::string *)value)->c_str(), -1, SQLITE_STATIC);
			                                                       // Length until first null terminator
			                                                           // No need to call a destructor
		}
	});

	int r = sqlite3_step(s);

	// Execute (and retrieve data....).
	Result result;

	// while there are still rows in the result.
	// On insert, update, delete no rows are completed and step returns DONE immediately.
	while (sqlite3_step(s) != SQLITE_DONE) {
		result.add_row();
		int numcol = sqlite3_column_count(s);

		// for every column in the result.
		for (int i = 0; i < numcol; ++i) {
			switch (sqlite3_column_type(s, i)) {
				case SQLITE_INTEGER:
					result.add_number(sqlite3_column_int(s, i));
					break;
				case SQLITE_FLOAT:
					result.add_decimal(sqlite3_column_double(s, i));
					break;
				case SQLITE_TEXT: {
					std::string temp(reinterpret_cast<const char *>(sqlite3_column_text(s, i)));

					result.add_string(temp);
					break;
				} case SQLITE_BLOB:
					// ignore for this implementation
					break;
				case SQLITE_NULL:
					// ignore for this implementation
					break;
			}
		}
	}

	// clean up the statement.
	sqlite3_finalize(s);

	return result;
}
