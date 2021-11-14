#include "cell.h"
#include "state.h"
#include "problem.h"
#include <vector>
#include <iostream>

using namespace std;

Problem::Problem() { // Default puzzle: no heuristics, initial men unset
	initial_state = new State();
	goal_state = new State();
	nodes_expanded = 0;
	max_depth = -1;

	for (int i = 0; i < 9; ++i) {
		goal_state->edit_node(i + 1, i + 1);
	}
}

Problem::Problem(int cell_count, int man_count) {
	initial_state = new State(cell_count, man_count);
	goal_state = new State(cell_count, man_count); // Modify during customization
	nodes_expanded = 0;
	max_depth = -1;

	for (int i = 0; i < man_count; ++i) {
		goal_state->edit_node(i + 1, i + 1);
	}
}

string Problem::cell_list_to_string(State* current) {
	string result = "";
	for (int i = 0; i < current->cell_list.size(); ++i) {
		result.append(to_string(current->cell_list.at(i).get_man_num()));
		if (current->cell_list.at(i).recess_exists()) {
			result.append(to_string(current->cell_list.at(i).get_recess_man_num()));
		}
	}
	return result;
}

// Search
void Problem::expand_node(State* current, string heuristic) {
	// Find valid moves, add to queue
	for (int i = 0; i < current->cell_list.size(); ++i) {
		// Check left: O X
		if (i != 0) {
			if (current->cell_list.at(i - 1).cell_empty()) {
				if (!current->cell_list.at(i).cell_empty()) {
					State *new_move = new State(current, i + 1, "left", heuristic);
					// Prevent repeated states
					if (!already_seen(*new_move)) {
						string s = cell_list_to_string(new_move);
						seen_set.insert(s);
						new_move->idx_in_statelist = state_list.size();
						state_list.push_back(*new_move);
						queue.push(new_move);
						++nodes_expanded;
					}
				}
			}
		}
		// Check right: X O
		if (i != current->cell_list.size() - 1) {
			if (!current->cell_list.at(i).cell_empty()) {
				if (current->cell_list.at(i + 1).cell_empty()) {
					State *new_move = new State(current, i + 1, "right", heuristic);
					if (!already_seen(*new_move)) {
						string s = cell_list_to_string(new_move);
						seen_set.insert(s);
						new_move->idx_in_statelist = state_list.size();
						state_list.push_back(*new_move);
						queue.push(new_move);
						++nodes_expanded;
					}
				}
			}
		}
		// Check up
		if (current->cell_list.at(i).recess_exists()) {
			if (current->cell_list.at(i).recess_empty()) {
				if (!current->cell_list.at(i).cell_empty()) {
					State *new_move = new State(current, i + 1, "up", heuristic);
					if (!already_seen(*new_move)) {
						string s = cell_list_to_string(new_move);
						seen_set.insert(s);
						new_move->idx_in_statelist = state_list.size();
						state_list.push_back(*new_move);
						queue.push(new_move);
						++nodes_expanded;
					}
				}
			}
		}
		// Check down
		if (current->cell_list.at(i).cell_empty()) {
			if (current->cell_list.at(i).recess_exists()) {
				if (!current->cell_list.at(i).recess_empty()) {
					State *new_move = new State(current, i + 1, "down", heuristic);
					if (!already_seen(*new_move)) {
						string s = cell_list_to_string(new_move);
						seen_set.insert(s);
						new_move->idx_in_statelist = state_list.size();
						state_list.push_back(*new_move);
						queue.push(new_move);
						++nodes_expanded;
					}
				}
			}
		}
	}
}

bool Problem::already_seen(State current) {
	if (seen_set.empty()) {
		return false;
	}
	string s = cell_list_to_string(&current);
	if (seen_set.find(s) == seen_set.end()) {
		return false;
	}
	return true;

}

void Problem::initialize_heuristic(string heuristic) {
	// g(initial)
	initial_state->set_g(0);

	// h(initial)
	int initial_h = 0;
	if (heuristic == "Manhattan") {
		for (int i = 0; i < initial_state->cell_list.size(); ++i) {
			// Check cell for displaced man
			if (!initial_state->cell_list.at(i).cell_empty()) {
				initial_h = initial_h + initial_state->man_displacement(i + 1);
			}
			// Check recess for displaced man
			if (initial_state->cell_list.at(i).recess_exists()) {
				if (!initial_state->cell_list.at(i).recess_empty()) {
					initial_h = initial_h + initial_state->recess_man_displacement(i + 1);
				}
			}
		}
	}
	else if (heuristic == "Misplaced") {
		for (int i = 0; i < initial_state->cell_list.size(); ++i) {
			if (!initial_state->cell_list.at(i).cell_empty()) {
				if (initial_state->cell_list.at(i).get_man_num() != i + 1) {
					initial_h = initial_h + 1;
				}
			}
			if (initial_state->cell_list.at(i).recess_exists()) {
				if (!initial_state->cell_list.at(i).recess_empty()) {
					initial_h = initial_h + 1;
				}
			}
		}
	}
	else if (heuristic == "UCS") {
		initial_h = 0;
	}
	initial_state->set_h(initial_h);

	// f(initial)
	initial_state->set_f(initial_h);
	return;
}

void Problem::print_solution(int index) {
	if (state_list.at(index).parent_idx_in_statelist != -1) {
		print_solution(state_list.at(index).parent_idx_in_statelist);
	}
	cout << endl;
	cout << "Step " << state_list.at(index).get_g() << ":" << endl;
	state_list.at(index).print_state();
	cout << endl;
}
