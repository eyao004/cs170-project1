#ifndef CELL_H
#define CELL_H

using namespace std;

class Cell {
 public:
	 Cell(int man_input, int cell_input);

	 int get_man_num() const;
	 int get_cell_num();
	 int get_recess_num();
	 int get_recess_man_num() const;

	 bool cell_empty();
	 bool recess_empty();
	 bool recess_exists() const;

	 void set_man_num(int);
	 void set_cell_num(int);
	 void set_recess_num(int);
	 void set_recess_man_num(int); // Initialize in State constructor

 private:
	 int man_num;
	 int cell_num;
	 int recess_num;
	 int recess_man_num;

};

#endif
