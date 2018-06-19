#include "SqliteDatabase.hpp"
#include <Result.hpp>
#include <sstream>
#include <typeinfo>

// Check windows (the pointer to a char pointer type changes based on version and typeid(char * *) does not match ever.
// ignoreing other compilers.
#if _WIN32 || _WIN64
	#if _WIN64
		#define CHAR_PTR_TYPE char * __ptr64
	#else
		#define CHAR_PTR_TYPE char * __ptr32
	#endif
#endif


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
	// blindly run the sql.
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
			oss << "INSERT INTO " << stmt.table() << " ("  << stmt.columns() << ")" << " VALUES (";
			for (int i = 1; i <= stmt.count(); ++i) {
				if (i != 1) oss << ", ";
				oss << "?" << i;
			}
			oss << ")";

			break;
		case Statement::action_t::updates:
			// UPDATE table SET column = ?1, ... WHERE column = ?2 [AND|OR] ...;
			oss << "UPDATE " << stmt.table() << " SET ";
			stmt.columns([&oss](int id, std::string column) {
				if (id != 1) oss << ", ";
				oss << column << " = ?" << id;
			});

			if (stmt.has_where()) {
				oss << " WHERE " << stmt.where();
			}

			break;
		case Statement::action_t::deletes:
			// DELETE FROM table WHERE column = ?1 [AND|OR] ...;
			if (stmt.count() == 0) {
				oss << "DELETE FROM " << stmt.table();
			}

			if (stmt.has_where()) {
				oss << " WHERE " << stmt.where();
			}

			break;
		case Statement::action_t::selects:
			// SELECT columns FROM table WHERE column = ?1 [AND|OR] ...;
			if (stmt.count() == 0) {
				oss << "SELECT * FROM " << stmt.table();
			} else {
				oss << "SELECT " << stmt.columns() << " FROM " << stmt.table();
			}

			if (stmt.has_where()) {
				oss << " WHERE " << stmt.where();
			}


			break;
	}
	oss << ";";

	// Use statment to populate the statment.
	sqlite3_stmt * s;
	sqlite3_prepare_v2(
		this->db,
		oss.str().c_str(),
		-1, // Length until first null terminator
		&s,
		NULL
	);

	//printf("SQLite error: %s\n", sqlite3_errmsg(db));

	stmt.populate([s] (int id, size_t type_hash, void * value) {
		if (type_hash == typeid(int).hash_code()) {
			int * v = (int *)value;
			sqlite3_bind_int(s, id, *v);
		} else if (type_hash == typeid(double).hash_code()) {
			double * v = (double *)value;
			sqlite3_bind_double(s, id, *v);
		} else if (type_hash == typeid(CHAR_PTR_TYPE).hash_code()) {
			char * * v = (char * *)value;
			sqlite3_bind_text(s, id, *v, -1, SQLITE_STATIC);
			                          // Length until first null terminator
			                              // No need to call a destructor
		}
		
	});
	//printf("SQLite error: %s\n", sqlite3_errmsg(db));

	// Execute and retrieve data.
	Result result;

	// while there are still rows in the result.
	// On insert, update, delete no rows are completed and step returns DONE immediately.
	while (sqlite3_step(s) != SQLITE_DONE) {
		result.add_row();
		int numcol = sqlite3_column_count(s);

		// for every column in the result.
		for (int i = 0; i < numcol; ++i) {
			switch (sqlite3_column_type(s, i)) {
				case SQLITE_INTEGER: {
					int * number = new int;
					*number = sqlite3_column_int(s, i);
					result.add_number(number);
					break;
				} case SQLITE_FLOAT: {
					double * decimal = new double;
					*decimal = sqlite3_column_double(s, i);
					result.add_decimal(decimal);
					break;
				} case SQLITE_TEXT: {
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
