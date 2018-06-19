#include <SqliteDatabase.hpp>
#include "ToDodao.hpp"
#include "ToDoItem.hpp"
#include "utils.hpp"
#include <ctime>
#include <iostream>
#include <list>
#include <conio.h>
#include <iomanip>

std::list<std::string> messages;

void addMessage(std::string message) {
	messages.push_back(message);
	if (messages.size() >= 15) {
		messages.pop_front();
	}
}

void showhelp() {
	addMessage("help - display this help.");
	addMessage("quit - exits the program.");
	addMessage("add - create a new to do.");
	addMessage("list - show all of the saved to do's.");
	addMessage("edit - add a note to a to do.");
	addMessage("delete - remove a to do.");
}

int main() {
	iDatabase * db = new SqliteDatabase("tododb.db");
	ToDodao dao(db);

	bool running = true;
	std::string lastCommand = "";
	int i = 0;
	showhelp();

	while (running) {
		if (_kbhit()) {
			char c = _getch();

			if (c == '\r') { // enter was pressed.
				// find commands.
				if (lastCommand.find("quit") == 0) {
					running = false;
				} else if (lastCommand.find("add") == 0) {
					system("cls");
					ToDoItem item;
					std::cout << "Enter Title: ";
					std::getline(std::cin, item.title);
					std::cout << "Description: ";
					std::getline(std::cin, item.description);
					std::cout << "Due date (DD/MM/YYYY): ";
					std::cin >> std::get_time(&item.due_date, "%d/%m/%Y");
					std::cout << "Priority (low = 0, meduim, high, extreme): ";
					int p;
					std::cin >> p;
					item.priority = (priority)p;

					dao.insert(item);
					system("cls");
				} else if (lastCommand.find("list") == 0) {
					std::vector<ToDoItem> v = dao.get_all();

					for (std::vector<ToDoItem>::iterator it = v.begin(); it != v.end(); ++it) {
						char * buffer = new char[80];
						snprintf(buffer, 80, "%d: %s (%s)", it->id, it->title.c_str(), it->priority_name().c_str());
						std::string s(buffer);
						addMessage(s);
					}
				} else if (lastCommand.find("edit") == 0 && lastCommand.length() > 5) {
					int id;
					std::string ids = lastCommand.substr(5);
					std::istringstream iss(ids);
					iss >> i;

					ToDoItem item = dao.get(i);
					system("cls");

					if (item.title.length() != 0) {
						std::cout << item.title << " (" << item.priority_name() << ")\n" << std::put_time(&item.due_date, "%d/%m/%Y") << std::endl;
						std::cout << "Add note:" << "\n" << item.description << std::endl;
						std::string note;
						std::getline(std::cin, note);

						item.description = item.description + "\n" + note;

						dao.update(item);
					} else {
						addMessage("Please enter a valid number with command.");
					}
					system("cls");
				} else if (lastCommand.find("delete") == 0 && lastCommand.length() > 5) {
					int id;
					std::string ids = lastCommand.substr(5);
					std::istringstream iss(ids);
					iss >> i;

					ToDoItem item = dao.get(i);
					system("cls");
					if (item.title.length() != 0) {
						std::cout << item.title << " (" << item.priority_name() << ")\n" << std::put_time(&item.due_date, "%d/%m/%Y") << std::endl;
						std::cout << item.description << std::endl;

						std::string option;
						std::cout << "\nAre you sure you want to remove this to do permanently (Y/N):";
						std::cin >> option;
						if (option[0] == 'y' || option[0] == 'Y') {
							dao.deletes(item.id);
						}
					} else {
						addMessage("Please enter a valid number with command.");
					} 
				} else if (lastCommand.find("help") == 0) {
					showhelp();
				} else {
					addMessage("Unknown command: '" + lastCommand + "'");
					addMessage("Type 'help' for help.");
				}
				lastCommand = "";
			} else {
				if (c == '\b') {
					// clean the string
					lastCommand.pop_back();
				} else if (c != '\0') {
					// add the pressed key to the buffer.
					lastCommand += c;
				}
			}
		}

		// write the text to the screen.
		for (std::list<std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
			util::print_text("%-75s", (*it).c_str());
		}
		util::print_text("%-75s", lastCommand.c_str());

		util::end_text();
	}
}