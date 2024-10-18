#include <algorithm>
#include <chrono>
#include <iostream>
#include <typeinfo>
#include <vector>

#include "rbtree.hpp"

// Find commands in argv
bool cmd_option_exists(char** begin, char** end, const std::string& option) {
    return std::find(begin, end, option) != end;
}

int main(int argc, char **argv) {
	Trees::RBTree<int> tree;
	char cmd;
	int num, l, r;
	std::vector<int> arr;
	std::vector<int> results;
	int max_size = 200'000'000;
	results.reserve(max_size);
	auto start = std::chrono::steady_clock::now();
	while (std::cin >> cmd) {
		if (cmd == 'k') {
			std::cin >> num;
			tree.insert(num);
			arr.push_back(num);
		}
		else {
			std::cin >> l >> r;
			if (l >= r) {
				std::cout << "Incorrect input. r has to be greater than l, i.e. r > l in command \"q l r\"\n";
				return 1;
			}
			int res = tree.distance(l, r);
			#ifdef DEBUG
			std::cout << l << ' ' << r << '\n';
			std::cout << "res = " << res << "\n";
			std::sort(arr.begin(), arr.end());
			for (int i = 0; i < arr.size(); ++i) {
				std::cout << arr[i] << ' ';
			}
			std::cout << std::endl;
			std::cout << std::endl;
			std::cout << std::endl;
			#endif
			results.push_back(res);
		}
	}
	auto finish = std::chrono::steady_clock::now();

	// to hide mode
	// hides results of command "q l r"
	if (!cmd_option_exists(argv, argv + argc, "-h")) { 
		for (int i = 0; i < results.size(); ++i)
			std::cout << results[i] << ' ';
		std::cout << std::endl;
	}

	// show time of execution
	if (cmd_option_exists(argv, argv + argc, "-t")) {
		double tm = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start).count();
		std::cout << tm << std::endl;
	}
}
