#ifndef _ToDoItem_HG_
#define _ToDoItem_HG_

#include <string>
#include <chrono>

enum priority {
	low,
	meduim,
	high,
	extreme
};

struct ToDoItem {
	int id;
	std::string title;
	std::string description;
	time_t due_date;
	priority priority;
};

#endif // !_ToDoItem_HG_
