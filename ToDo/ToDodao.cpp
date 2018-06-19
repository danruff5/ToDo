#include "ToDodao.hpp"
#include <iomanip>

ToDodao::ToDodao(iDatabase * database) : db(database) {
	// ensure the table is in the database.
	std::string sql = R"sql(
CREATE TABLE IF NOT EXISTS todo (
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

	std::ostringstream oss;
	oss << std::put_time(&item.due_date, "%d/%m/%Y");
	std::string s(oss.str());
	
	stmt.string("title", item.title)
		.string("description", item.description)
		.string("due_date", s)
		.number("priority", item.priority);

	db->execute(stmt);
}

void ToDodao::update(ToDoItem & item) {
	Statement stmt(Statement::action_t::updates, "todo");

	std::ostringstream oss;
	oss << std::put_time(&item.due_date, "%d/%m/%Y");
	std::string s(oss.str());

	stmt.string("title", item.title)
	    .string("description", item.description)
		.string("due_date", s)
	    .number("priority", item.priority)
		.where("id", item.id);

	db->execute(stmt);
}

void ToDodao::deletes(int id) {
}

ToDoItem ToDodao::get(int id) {
	Statement stmt(Statement::action_t::selects, "todo");

	stmt.where("id", id);
	Result result = db->execute(stmt);

	ToDoItem item;
	item.id = id;
	item.title = result.string(1);
	item.description = result.string(2);
	std::istringstream iss(result.string(3));
	iss >> std::get_time(&item.due_date, "%d/%m/%Y");
	item.priority = (priority)result.number(4);

	return item;
}

std::vector<ToDoItem> ToDodao::get_all() {
	Statement stmt(Statement::action_t::selects, "todo");
	Result result = db->execute(stmt);

	std::vector<ToDoItem> v;

	for (int i = 0; i < result.row_count(); ++i) {
		ToDoItem item;
		item.id = result.number(0, i);
		item.title = result.string(1, i);
		item.description = result.string(2, i);
		std::istringstream iss(result.string(3, i));
		iss >> std::get_time(&item.due_date, "%d/%m/%Y");
		item.priority = (priority)result.number(4, i);

		v.push_back(item);
	}

	return v;
}
