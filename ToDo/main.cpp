#include <SqliteDatabase.hpp>
#include "ToDodao.hpp"
#include "ToDoItem.hpp"
#include <ctime>

int main() {
	iDatabase * db = new SqliteDatabase("tododb.db");

	ToDodao dao(db);

	ToDoItem item;
	item.title = "Hello";
	item.description = "Do the thing";

	std::time_t t = std::time(0);   // get time now
	std::tm* now = std::localtime(&t);
	item.due_date = *now;

	item.priority = priority::low;


	dao.insert(item);
}