#include "cell.h"
#include "state.h"
#include <vector>
#include <iostream>

using namespace std;

State::State() { // Blank game (default number of trenches)
	cell_count = 10;
	man_count = 9;
	for (int i = 0; i < 10; ++i) {
		Cell node = Cell(0, i + 1);
		cell_list.push_back(node);
	}
	// Cell 4
	cell_list.at(3).set_recess_num(0);
	// Cell 6
	cell_list.at(5).set_recess_num(0);
	// Cell 8
	cell_list.at(7).set_recess_num(0);
	// Set recess numbers
	this->set_recess_numbers();
	parent_idx_in_statelist = -1;
}

State::State(int cell_count_input, int man_count_input) {
	// Creates blank game with specified number of cells and recesses (can modify to be other positions)
	cell_count = cell_count_input;
	man_count = man_count_input;
	int recess_start = 3;
	if (cell_count <= 7) {
		recess_start = 2;
	}
	for (int i = 0; i < cell_count; ++i) {
		Cell node = Cell(0, i + 1);
		cell_list.push_back(node);
	}
	for (int i = recess_start; i < cell_count - 1; i = i + 2) {
		cell_list.at(i).set_recess_num(0);
	}
	this->set_recess_numbers();
	parent_idx_in_statelist = -1;
	idx_in_statelist = 0;
}

// 4 possible moves:
//   Left (left)
//   Right (right)
//   Into recess (up)
//   Out of recess (down)
State::State(State* parent, int position, string move, string heuristic) {
	// Copy over
	cell_list = parent->cell_list;
	cell_count = parent->cell_count;
	man_count = parent->man_count;
	g = parent->get_g() + 1;
	parent_idx_in_statelist = parent->idx_in_statelist;

	//parent_state = parent;

	// Move (note: cannot use recess num for position)
	if (move == "up") {
		int man_num = cell_list.at(position - 1).get_man_num();
		cell_list.at(position - 1).set_recess_man_num(man_num);
		cell_list.at(position - 1).set_man_num(0);
	}
	if (move == "down") {
		int man_num = cell_list.at(position - 1).get_recess_man_num();
		cell_list.at(position - 1).set_man_num(man_num);
		cell_list.at(position - 1).set_recess_man_num(0);
	}
	if (move == "left") {
		int man_num = cell_list.at(position - 1).get_man_num();
		cell_list.at(position - 2).set_man_num(man_num);
		cell_list.at(position - 1).set_man_num(0);
	}
	if (move == "right") {
		int man_num = cell_list.at(position - 1).get_man_num();
		cell_list.at(position).set_man_num(man_num);
		cell_list.at(position - 1).set_man_num(0);
	}
	// Compute h and f
	h = 0;
	f = 0;
	if (heuristic == "Manhattan") {
		compute_manhattan();
	}
	else if (heuristic == "Misplaced") {
		compute_misplaced();
	}
	else if (heuristic == "UCS") {
		compute_ucs();
	}
}

/*State::~State() {
	for (uint i = 0; i < cell_list.size(); ++i) {
		delete &cell_list.at(i);
	}
}*/

void State::set_recess_numbers() { // Only run once (end of customization)
	int j = 0;
	for (int i = 0; i < cell_list.size(); ++i) {
		if (cell_list.at(i).recess_exists() == true) {
			++j;
			cell_list.at(i).set_recess_num(cell_count + j);
			cell_list.at(i).set_recess_man_num(0);
		}
	}
}
void State::edit_node(int position, int man_input) {
	cell_list.at(position - 1).set_man_num(man_input);
}

void State::set_man_num(int position, int num) {
	cell_list.at(position - 1).set_man_num(num);
}
void State::set_recess_man_num(int position, int num) {
	cell_list.at(position - 1).set_recess_man_num(num);
}

int State::get_cell_count() {
	return cell_count;
}

int State::get_man_num(int position) const {
	return cell_list.at(position - 1).get_man_num();
}

bool State::cell_empty(int position) {
	return cell_list.at(position - 1).cell_empty();
}

bool State::recess_exists(int position) const {
	if (cell_list.at(position - 1).recess_exists()) {
		return true;
	}
	else {
		return false;
	}
}
bool State::recess_empty(int position) {
	if (cell_list.at(position - 1).get_recess_man_num() == 0) {
		return true;
	}
	else {
		return false;
	}
}
int State::get_recess_num(int position) {
	return cell_list.at(position - 1).get_recess_num();
}
int State::get_recess_man_num(int position) const {
	return cell_list.at(position - 1).get_recess_man_num();
}

void State::set_g(int input) {
	g = input;
}
void State::set_h(int input) {
	h = input;
}
void State::set_f(int input) {
	f = input;
}
int State::get_g() const {
	return g;
}
int State::get_h() const {
	return h;
}
int State::get_f() const {
	return f;
}

// Heuristics
// Set h (and f)
void State::compute_manhattan() {
	for (int i = 0; i < cell_list.size(); ++i) {
		// Check cell for displaced man
		if (!cell_list.at(i).cell_empty()) {
			h = h + man_displacement(i + 1);
		}
		// Check recess for displaced man
		if (cell_list.at(i).recess_exists()) {
			if (!cell_list.at(i).recess_empty()) {
				h = h + recess_man_displacement(i + 1);
			}
		}
	}
	f = g + h;
	return;
}
// Misplaced tiles: Count displaced men
void State::compute_misplaced() {
	for (int i = 0; i < cell_list.size(); ++i) {
		if (!cell_list.at(i).cell_empty()) {
			if (cell_list.at(i).get_man_num() != i + 1) {
				++h;
			}
		}
		if (cell_list.at(i).recess_exists()) {
			if (!cell_list.at(i).recess_empty()) {
				++h;
			}
		}
	}
	f = g + h;
	return;
}
// Uniform cost search: h(n) = 0
void State::compute_ucs() {
	h = 0;
	f = g + h;
	return;
}

int State::man_displacement(int position) {
	int man_num = cell_list.at(position - 1).get_man_num();
	// Man number doesn't match cell number
	if (man_num != position) {
		// Return distance (always positive)
		if (man_num > position) {
			return (man_num - position);
		}
		else {
			return (position - man_num);
		}
	}
	// Man in correct position
	else {
		return 0;
	}
}
int State::recess_man_displacement(int position) {
	// 1 step out
	int displacement = 1;

	int recess_man_num = cell_list.at(position - 1).get_recess_man_num();
	// Man number doesn't match cell number
	if (recess_man_num != position) {
		// Return distance (always positive)
		if (recess_man_num > position) {
			return (recess_man_num - position) + displacement;
		}
		else {
			return (position - recess_man_num) + displacement;
		}
	}
	return displacement;
}

bool State::is_goal() {
	// Can't check h because of UCS
	for (int i = 0; i < man_count; ++i) {
		if (cell_list.at(i).get_man_num() != i + 1) {
			return false;
		}
	}
	return true;
}


bool operator<(State a, State b) { // Not used in current code
	return a.get_f() < b.get_f();
}
bool operator==(State a, State b) {
	for (int i = 0; i < a.cell_list.size(); ++i) {
		if (a.cell_list.at(i).get_man_num() != b.cell_list.at(i).get_man_num()) {
			return false;
		}
		if (a.cell_list.at(i).recess_exists()) {
			if (a.cell_list.at(i).get_recess_man_num() != b.cell_list.at(i).get_recess_man_num()) {
				return false;
			}
		}
	}
	return true;
}

// Print
void State::print_state() {
	int cell_count = get_cell_count();
	// Print recesses, spaces where none
	for (int i = 0; i < cell_count; ++i) {
		if (recess_exists(i + 1)) {
			if (get_recess_num(i + 1) < 10) {
				cout << "  " << get_recess_num(i + 1) << "  ";
			}
			else {
				cout << " " << get_recess_num(i + 1) << "  ";
			}
		}
		else {
			cout << "     ";
		}
	}
	cout << endl;
	// Print recess slots
	for (int i = 0; i < cell_count; ++i) {
		if (recess_exists(i + 1)) {
			// If recess exists + is occupied
			if (!recess_empty(i + 1)) {
				// If single digit
				if (get_recess_man_num(i + 1) < 10) {
					cout << "[ " << get_recess_man_num(i + 1) << " ]";
				}
				else {
					cout << "[" << get_recess_man_num(i + 1) << " ]";
				}
			}
			else {
				cout << "[   ]";
			}
		}
		else {
			cout << "     ";
		}
	}
	cout << endl;
	// Print cell numbers
	for (int i = 0; i < cell_count; ++i) {
		if ((i + 1) < 10) {
			cout << "  " << (i + 1) << "  ";
		}
		else {
			cout << " " << (i + 1) << "  ";
		}
	}
	cout << endl;
	// Print cell slots
	for (int i = 0; i < cell_count; ++i) {
		if (!cell_empty(i + 1)) {
			if (get_man_num(i + 1) < 10) {
				cout << "[ " << get_man_num(i + 1) << " ]";
			}
			else {
				cout << "[" << get_man_num(i + 1) << " ]";
			}
		}
		else {
			cout << "[   ]";
		}
	}
	cout << endl;
}