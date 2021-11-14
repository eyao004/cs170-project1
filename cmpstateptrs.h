#ifndef CMPSTATEPTRS_H
#define CMPSTATEPTRS_H

using namespace std;

struct CmpStatePtrs {
	bool operator()(const State* left, const State* right) const {
		return left->get_f() > right->get_f();
	}
};

#endif