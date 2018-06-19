#include <SqliteDatabase.hpp>
#include "ToDodao.hpp"
#include "ToDoItem.hpp"
#include <ctime>
#include <iostream>

int main() {
	iDatabase * db = new SqliteDatabase("tododb.db");
	ToDodao dao(db);

	std::vector<ToDoItem> v = dao.get_all();

	for (ToDoItem i : v) {
		std::cout << i.title << "\n" << i.description << "\n" << std::endl;
	}
}