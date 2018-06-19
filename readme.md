# To Do Application
> By Daniel Krauskopf

This application will track the users list of todo's. Each to do item contains the following:
- Title
- Description
- Due Date
  - in DD/MM/YYYY format
- Priority
  - Low
  - Medium
  - High
  - Extreme

For the database implemetation it uses sqlite. The .db file is created in the /ToDo folder.

Visual studio projects:
- persistence 
  - Conains interfaces and abstract data objects to interact with a database.
- sqlite
  - Contains the specific implementation for the sqlite database library.
- ToDo
  - Contains the main application code. All the to do implementation is in here 


Comands:
|Command |Description                     |
|--------|--------------------------------|
| help   | display this help.             |
| quit   | exits the program.             |
| add    | create a new to do.            |
| list   | show all of the saved to do's. |
| edit   | add a note to a to do.         |
| delete | remove a to do.                |