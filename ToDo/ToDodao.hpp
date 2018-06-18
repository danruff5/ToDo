#ifndef _ToDodao_HG_
#define _ToDodao_HG_

#include "ToDoItem.hpp"
#include <iDatabase.hpp>

class ToDodao {
	iDatabase * db;
public:
	ToDodao(iDatabase * database);

	void insert(ToDoItem & item);
	void update(ToDoItem & item);
	ToDoItem get(int id);
};

#endif // !_ToDodao_HG_
