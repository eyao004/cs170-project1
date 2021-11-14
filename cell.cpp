#include "cell.h"

using namespace std;

Cell::Cell(int man_input, int cell_input) {
	man_num = man_input;
	cell_num = cell_input;
	recess_man_num = -1;
	recess_num = -1; // -1 = does not exist
}

int Cell::get_man_num() const {
	return man_num;
}
int Cell::get_cell_num() {
	return cell_num;
}
int Cell::get_recess_num() {
	return recess_num;
}
int Cell::get_recess_man_num() const {
	return recess_man_num;
}
bool Cell::cell_empty() {
	if (man_num == 0) {
		return true;
	}
	else {
		return false;
	}
}
bool Cell::recess_empty() {
	if (recess_man_num == 0) {
		return true;
	}
	else {
		return false;
	}
}
bool Cell::recess_exists() const {
	if (recess_num == -1) {
		return false;
	}
	else {
		return true;
	}
}

void Cell::set_man_num(int input) {
	man_num = input;
}
void Cell::set_cell_num(int input) {
	cell_num = input;
}
void Cell::set_recess_num(int input) {
	recess_num = input;
}
void Cell::set_recess_man_num(int input) {
	recess_man_num = input;
}
