#include <SqliteDatabase.hpp>
#include "ToDodao.hpp"
#include "ToDoItem.hpp"
#include <ctime>
#include <iostream>

int main() {
	iDatabase * db = new SqliteDatabase("tododb.db");
	ToDodao dao(db);

	ToDoItem item = dao.get(11);
	item.description = "beep boop!";

	dao.deletes(item.id);
}