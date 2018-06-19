#ifndef _ToDoItem_HG_
#define _ToDoItem_HG_

#include <string>
#include <ctime>

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
	std::tm due_date;
	priority priority;

	std::string priority_name() {
		switch (priority) {
			case low:
				return "low";
			case meduim:
				return "medium";
			case high:
				return "high";
			case extreme:
				return "extreme";
		}
	}
};

#endif // !_ToDoItem_HG_
