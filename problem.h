#ifndef PROBLEM_H
#define PROBLEM_H

#include <queue>
#include <vector>
#include <list>
#include <set>
#include "state.h"
#include "cell.h"
#include "cmpstateptrs.h"

using namespace std;

class Problem {
 public:
	 // Initialization
	 Problem(); // Default
	 Problem(int, int); // Specified number of cells
	 void initialize_heuristic(string);
	 // Parameters of puzzle: set to default in default initializer

	 // Variables
	 State *initial_state; // Edit at customization stage in main
	 State *goal_state;
	 // Stores nodes that are waiting to be expanded,
	 // Heuristic algorithm determines the priority of the nodes
	 priority_queue<State*, vector<State*>, CmpStatePtrs> queue;
	 // Stores all states ever discovered. For backtracking once solution is found
	 vector<State> state_list;
	 // For printing purposes only
	 int nodes_expanded;
	 int max_depth;

	 void expand_node(State*, string heuristic);
	 void print_solution(int index);

 private:
	 // Hashed set of states for seen states
	 set<string> seen_set;

	 // Search
	 bool already_seen(State);
	 string cell_list_to_string(State* current);
};

#endif
