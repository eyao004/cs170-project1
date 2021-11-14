#ifndef STATE_H
#define STATE_H

#include <vector>
#include "cell.h"
#include <string>

using namespace std;



class State {
 public:
	State(); // Default
	State(int, int); // Specified number of cells/men
	State(State* parent, int position, string move, string heuristic); // Non-initial
	/*~State();*/

	vector<Cell> cell_list;
	// For backtracking steps when solution is found
	int parent_idx_in_statelist;
	int idx_in_statelist;

	void set_recess_numbers();
	void edit_node(int position, int man_input);

	// Set
	void set_man_num(int position, int num);
	void set_recess_man_num(int position, int num);
	// Get
	int get_cell_count(); // TOTAL
	int get_man_num(int position) const;
	int get_recess_num(int position);
	int get_recess_man_num(int position) const;
	// Check
	bool cell_empty(int position);
	// Recesses
	bool recess_exists(int position) const;
	bool recess_empty(int position);

	// Heuristic
	void set_g(int);
	void set_h(int);
	void set_f(int);
	int get_g() const;
	int get_h() const;
	int get_f() const;
	// Manhattan
	void compute_manhattan();
	int man_displacement(int position);
	int recess_man_displacement(int position);
	// Misplaced
	void compute_misplaced();
	// UCS
	void compute_ucs();
	// Search
	bool is_goal();

	// Print
	void print_state();
	
 private:
	int cell_count;
	int man_count;
	// Heuristic (calculate on initialization)
	int g;
	int h;
	int f;

	// Overloaded comparison
	friend bool operator<(State a, State b);
	friend bool operator==(State a, State b);
};

#endif
