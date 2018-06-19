#include <SqliteDatabase.hpp>
#include "ToDodao.hpp"
#include "ToDoItem.hpp"
#include <ctime>
#include <iostream>

int main() {
	iDatabase * db = new SqliteDatabase("tododb.db");
	ToDodao dao(db);

	ToDoItem i = dao.get(11);

	std::cout << i.title << "\n" << i.description << std::endl;
}