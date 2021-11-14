#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <queue>
#include <chrono>
#include "cell.h"
#include "state.h"
#include "problem.h"

using namespace std;
using namespace std::chrono;

// Search
void generic_search(State* current, Problem&, string strategy);

// Print
void print_State_default(); // Temporary function

// Initialization
void set_Initial(Problem& game); // Main set initial function (set positions)
void set_Initial_Num(Problem& game, int& num_Men, int& num_Trenches, int& num_Recesses); // Set numbers (may not be implemented)

// Minor helper functions
bool is_Digit(string s);

int main() {
	string user_Input;
	bool valid_Input = false;
	Problem game;
	//int cell_count = 5;
	//int man_count = 3;
	int cell_count = 7; // Current default
	int man_count = 5;
	//int cell_count = 10;
	//int man_count = 9;

	cout << "This program solves the \"Nine Men in a Trench\" puzzle." << endl;
	cout << "The goal of this puzzle is to have each man positioned in the trench corresponding to his number." << endl << endl;
	print_State_default();
	cout << endl;

	cout << "The initial state of this puzzle can be customized." << endl;
	cout << "NOTE: Customization is not thoroughly tested for invalid inputs." << endl;
	while (valid_Input == false) {
		cout << "Proceed with default puzzle? (Y/N): ";
		cin >> user_Input;
		
		if (user_Input == "Y" || user_Input == "y") {
			// Set up tree as default
			if (cell_count == 10) {
				game = Problem();
			}
			else {
				game = Problem(cell_count, man_count);
			}
			// Set man numbers
			// 0 2 3 4 5 6 7 8 9 1
			for (int i = 2; i <= man_count; ++i) {
				game.initial_state->set_man_num(i, i);
			}
			game.initial_state->set_man_num(cell_count, 1);

			valid_Input = true;
		}
		else if (user_Input == "N" || user_Input == "n") {
			cout << "Entering setup..." << endl;
			set_Initial(game);
			valid_Input = true;
		}
		else {
			cout << "Invalid input. Try again." << endl;
		}
	}

	/*
	valid_Input = false;
	// Change man positions
	while (valid_Input == false) {
		cout << "Proceed with default positions? (Y/N): ";
		cin >> user_Input;

		if (user_Input == "Y" || user_Input == "y") {
			// Set man numbers
			// 0 2 3 4 5 6 7 8 9 1
			for (int i = 2; i <= man_count; ++i) {
				game.initial_state->set_man_num(i, i);
			}
			game.initial_state->set_man_num(cell_count, 1);

			// Short test: 0 1 2 3 4 5 6 7 8 9
			//for (int i = 0; i < 10; ++i) {
			//	game.initial_state->set_man_num(i + 1, i);
			//}
			
			valid_Input = true;
		}
		else if (user_Input == "N" || user_Input == "n") {
			cout << "Entering setup..." << endl;
			// Input
			cout << "Incomplete function" << endl;
			valid_Input = true;
		}
		else {
			cout << "Invalid input. Try again." << endl;
		}
	}
	*/

	cout << endl << "Puzzle created." << endl;
	cout << "Printing initial state:" << endl;
	game.initial_state->print_state();
	cout << endl << "Printing goal state:" << endl;
	game.goal_state->print_state();
	cout << endl;


	/*
	cout << endl << "Testing expanded nodes and their heuristic calculations." << endl;
	game.expand_node(game.initial_state, "Manhattan");
	cout << game.nodes_expanded << " nodes expanded." << endl;
	int iteration = 1;
	while (!game.queue.empty()) {
		cout << "Node " << iteration << ":" << endl;
		game.queue.top()->print_state();
		cout << endl;
		cout << "g(n) = " << game.queue.top()->get_g();
		cout << ", h(n) = " << game.queue.top()->get_h() << ", f(n) = " << game.queue.top()->get_f() << endl;
		cout << "Recess numbers: ";
		for (int i = 1; i < 11; ++i) {
			cout << game.queue.top()->get_recess_num(i) << ", ";
		}
		cout << endl << "Recess man numbers: ";
		for (int i = 1; i < 11; ++i) {
			cout << game.queue.top()->get_recess_man_num(i) << ", ";
		}
		cout << endl << endl;
		++iteration;
		game.queue.pop();
	}
	*/
	

	cout << endl;
	cout << "Choose heuristic to use (1: Manhattan Distance, 2: Misplaced Tiles, 3: Uniform Cost Search): ";
	cin >> user_Input;

	if (user_Input == "1") {
		// Manhattan Distance
		cout << "A* with Manhattan Distance heuristic:" << endl;
		game.initialize_heuristic("Manhattan");

		cout << "Initial state:" << endl;
		cout << "g(n): " << game.initial_state->get_g() << endl;
		cout << "h(n): " << game.initial_state->get_h() << endl;
		cout << "f(n): " << game.initial_state->get_f() << endl;

		auto start = high_resolution_clock::now();
		generic_search(game.initial_state, game, "Manhattan");
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start);
		double time_seconds = duration.count();
		time_seconds = time_seconds / 1000;
		cout << fixed << showpoint;
		cout << setprecision(3) << "Runtime: " << time_seconds << " seconds" << endl << endl;
	}

	else if (user_Input == "2") {
		// Misplaced Tiles
		cout << "A* with Misplaced Tiles heuristic:" << endl;
		game.initialize_heuristic("Misplaced");

		cout << "Initial state:" << endl;
		cout << "g(n): " << game.initial_state->get_g() << endl;
		cout << "h(n): " << game.initial_state->get_h() << endl;
		cout << "f(n): " << game.initial_state->get_f() << endl;

		auto start = high_resolution_clock::now();
		generic_search(game.initial_state, game, "Misplaced");
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start);
		double time_seconds = duration.count();
		time_seconds = time_seconds / 1000;
		cout << fixed << showpoint;
		cout << setprecision(3) << "Runtime: " << time_seconds << " seconds" << endl << endl;
	}

	else if (user_Input == "3") {
		// UCS
		cout << "Uniform Cost Search:" << endl;
		game.initialize_heuristic("UCS");

		cout << "Initial state:" << endl;
		cout << "g(n): " << game.initial_state->get_g() << endl;
		cout << "h(n): " << game.initial_state->get_h() << endl;
		cout << "f(n): " << game.initial_state->get_f() << endl;

		auto start = high_resolution_clock::now();
		generic_search(game.initial_state, game, "UCS");
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start);
		double time_seconds = duration.count();
		time_seconds = time_seconds / 1000;
		cout << fixed << showpoint;
		cout << setprecision(3) << "Runtime: " << time_seconds << " seconds" << endl << endl;
	}
}

/*
// Search
void generic_search(State* current, Problem& game, string strategy) {
	cout << "Searching at depth " << current->get_g() << "." << endl;
	cout << "Size of queue: " << game.queue.size() << endl;
	cout << "g(n): " << current->get_g() << ", h(n): " << current->get_h() << ", f(n): " << current->get_f();
	cout << endl;
	print_State(current);
	cout << endl;

	// Base case
	if (game.queue.empty()) {
		cout << "Failed to find solution. Game aborted." << endl;
		return;
	}
	game.queue.pop();
	// Check for goal state
	if (current->is_goal()) {
		cout << endl;
		cout << "Found solution at depth " << current->get_g() << "." << endl;
		cout << game.nodes_expanded << " nodes expanded." << endl;
		cout << "Solution:" << endl;
		print_State(current);
		return;
	}
	// Check if seen before
	// current = game.initial_state;
	if (game.already_seen(*current)) {
		cout << "Repeated node." << endl;
	}
	else {
		game.seen.push_back(*current);
		game.expand_node(game.queue.top(), strategy);
	}
	// Recurse
	generic_search(game.queue.top(), game, strategy);
}
*/

// Non-recursive search
void generic_search(State* current, Problem& game, string strategy) {
	// contains all the states ever discovered (for retrieving solution at the end)
	game.state_list.push_back(*current);
	// store the initial node
	game.queue.push(current);

	// Base case
	while (!game.queue.empty()) {
		current = game.queue.top();
		game.queue.pop();

		if( game.max_depth < current->get_g() ) {
			game.max_depth = current->get_g();
			cout << "Searching at depth: " << current->get_g() << ". Size of queue: " << game.queue.size() << ". Nodes expanded:" << game.nodes_expanded << ". h:" << current->get_h() << ". f:" << current->get_f() << endl;
		}
		game.expand_node(current, strategy);
		
		// For testing
//		cout << "Searching at depth " << current->get_g() << "." << endl;
//		cout << "Size of queue: " << game.queue.size() << endl;
//		cout << "Unique states seen: " << game.seen.size() << endl;
//		cout << "g(n): " << current->get_g() << ", h(n): " << current->get_h() << ", f(n): " << current->get_f();
//		cout << endl;
//		cout << game.nodes_expanded << " nodes expanded." << endl;
//		current->print_state();
//		cout << endl << endl;
		
		// Check for goal state
		if (current->is_goal()) {
			cout << "Found solution at depth " << current->get_g() << "." << endl;
			cout << game.nodes_expanded << " nodes expanded." << endl;
			cout << "Solution:" << endl;
			current->print_state();
			cout << endl;
			game.print_solution(current->idx_in_statelist);
			return;
		}
		free(current);
	}
	cout << "Failed to find solution. Game aborted." << endl;
	return;
}


void print_State_default() { // Test function, will use the real function instead
	cout << "                11        12        13" << endl;
	cout << "               [   ]     [   ]     [   ]" << endl;
	cout << "  1    2    3    4    5    6    7    8    9   10" << endl;
	cout << "[   ][ 2 ][ 3 ][ 4 ][ 5 ][ 6 ][ 7 ][ 8 ][ 9 ][ 1 ]" << endl;
}

void set_Initial(Problem &game) {
	int num_Men = 9;
	int num_Trenches = 10;
	int num_Recesses = 3;

	set_Initial_Num(game, num_Men, num_Trenches, num_Recesses);
	Problem new_game = Problem(num_Trenches, num_Men);
	game = new_game;

	int i = 1;
	int limit = num_Trenches + num_Recesses;
	string current_Input;
	bool valid_Input = false;
	// Counters during setting
	int num_Blanks = 0;
	int num_Filled = 0;
	vector<int> filled(limit, 0);

	// Set positions of recesses
	string recess_input;
	// Reset recesses (don't want to modify other code)
	for (i = 0; i < game.initial_state->cell_list.size(); ++i) {
		if (game.initial_state->cell_list.at(i).recess_exists()) {
			game.initial_state->cell_list.at(i).set_recess_num(-1);
		}
	}
	for (i = 0; i < game.goal_state->cell_list.size(); ++i) {
		if (game.goal_state->cell_list.at(i).recess_exists()) {
			game.goal_state->cell_list.at(i).set_recess_num(-1);
		}
	}
	cout << "Enter cell numbers above which to set recesses." << endl;
	game.initial_state->print_state();
	cout << endl;
	i = 0;
	while (i < num_Recesses) {
		cin >> recess_input;
		if (stoi(recess_input) > num_Trenches) {
			cout << "  Invalid input (trench doesn't exist)." << endl;
		}
		else {
			game.initial_state->cell_list.at(stoi(recess_input) - 1).set_recess_man_num(0);
			game.initial_state->cell_list.at(stoi(recess_input) - 1).set_recess_num(0);
			game.goal_state->cell_list.at(stoi(recess_input) - 1).set_recess_man_num(0);
			game.goal_state->cell_list.at(stoi(recess_input) - 1).set_recess_num(0);
			++i;
		}
	}
	game.initial_state->set_recess_numbers();
	game.goal_state->set_recess_numbers();

	cout << endl;
	cout << "Enter [0] for blank position." << endl;
	cout << "Enter [P] to see current setup." << endl << endl;
	// cout << "Enter [C] to clear setup and start over." << endl;
	i = 1; // Note i is position name
	while (i <= limit) {
		while (valid_Input == false) {
			cout << "Setting position " << i << ": ";
			cin >> current_Input;

			if (current_Input == "P" || current_Input == "p") {
				cout << endl;
				game.initial_state->print_state();
				cout << endl;
				valid_Input = false;
			}
			else if (!is_Digit(current_Input)) {
				cout << "  Invalid input (non-numeric input). Try again." << endl;
			}
			else if (!(stoi(current_Input) >= 0) || !(stoi(current_Input) <= num_Men)) {
				cout << "  Invalid input (out of range). Try again." << endl;
			}
			else { // In-range input
				if (stoi(current_Input) == 0) { // Check blanks
					if (num_Blanks >= ((num_Trenches + num_Recesses) - num_Men)) {
						cout << "  Invalid input (too many blanks). Try again." << endl;
					}
					else {
						++num_Blanks;
						valid_Input = true;
					}
				}
				else { // Check numbers
					// Check if limit is reached (not enough zeroes)
					if (num_Filled >= num_Men) {
						cout << "  Invalid input (too many filled positions). Try again." << endl;
						valid_Input = false;
					}

					// Search for duplicate
					else if (filled.at(stoi(current_Input) - 1) == stoi(current_Input)) {
						cout << "  Invalid input (duplicate). Try again." << endl;
						valid_Input = false;
					}

					else {
						// Add
						// If cell
						if (i <= num_Trenches) {
							game.initial_state->cell_list.at(i - 1).set_man_num(stoi(current_Input));
							filled.at(i - 1) = stoi(current_Input);
						}
						// If recess
						else {
							for (int j = 0; j < game.initial_state->cell_list.size(); ++j) {
								if (game.initial_state->cell_list.at(j).recess_exists()) {
									if (game.initial_state->cell_list.at(j).get_recess_num() == i) {
										game.initial_state->cell_list.at(j).set_recess_man_num(stoi(current_Input));
										filled.at(i - 1) = stoi(current_Input);
									}
								}
							}
						}
						++num_Filled;
						valid_Input = true;
					}
				}
			}
		}
		valid_Input = false;
		++i;
	}
}

void set_Initial_Num(Problem& game, int& num_Men, int& num_Trenches, int& num_Recesses) {
	bool valid_Input = false;

	while (valid_Input == false) {
		cout << "Set number of men (Default: 9): ";
		cin >> num_Men;
		cout << "Set number of trenches (Default: 10): ";
		cin >> num_Trenches;

		if (num_Men >= num_Trenches) {
			cout << "  Invalid input. Restarting setup." << endl;
			valid_Input = false;
		}
		else {
			valid_Input = true;
		}
	}
	valid_Input = false;
	while (valid_Input == false) {
		cout << "Set number of recesses (Default: 3): ";
		cin >> num_Recesses;
		if (num_Recesses > num_Trenches) {
			cout << "  Invalid input. Restarting setup." << endl;
			valid_Input = false;
		}
		else {
			valid_Input = true;
		}
	}
}

// Minor helper functions
bool is_Digit(string s) {
	int i = 0;
	for (i = 0; i < s.size(); ++i) {
		if (!isdigit(s.at(i))) {
			return false;
		}
	}
	return true;
}
