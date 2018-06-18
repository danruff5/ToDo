#include "ToDodao.hpp"

ToDodao::ToDodao(iDatabase * database) : db(database) {
	// ensure the table is in the database.
	std::string sql = R"sql(
CREATE TABLE todo IF NOT EXISTS (
	id INTEGER PRIMARY KEY AUTOINCREMENT, 
	title TEXT,
	description TEXT,
	due_date TEXT,
	priority INTEGER
);)sql";

	db->execute(sql);
}

void ToDodao::insert(ToDoItem & item) {
	Statement stmt(Statement::action_t::inserts, "todo");
	
	stmt.string("title", item.title);
	stmt.string("description", item.description);
	stmt.string("due_date", ctime(&item.due_date));
	stmt.number("priority", item.priority);

	db->execute(stmt);
}

void ToDodao::update(ToDoItem & item) {
	Statement stmt(Statement::action_t::updates, "todo");

	stmt.number("id", item.id);
	stmt.string("title", item.title);
	stmt.string("description", item.description);
	stmt.string("due_date", ctime(&item.due_date));
	stmt.number("priority", item.priority);

	db->execute(stmt);
}
