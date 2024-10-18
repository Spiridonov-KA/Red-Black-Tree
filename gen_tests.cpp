#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
#include <string>

int range_query(const std::set<int> &s, int l, int r) {
	using itt = std::set<int>::iterator;
	itt start = s.lower_bound(l);
	itt fin = s.upper_bound(r);
	return std::distance(start, fin);
}

bool cmd_option_exists(char** begin, char** end, const std::string& option) {
    return std::find(begin, end, option) != end;
}

// Generate tests
// n_el - number of commands "k n"
// n_q - number of commands "q l r"
void gen_test(int n_el, int n_q, 
			  std::string &file_name, int n_test) {
	std::string max_num_test = "999";
	std::string cur_test = std::to_string(n_test);
	std::string suffix = ".dat";
	std::string suffix_out = ".ans";
	std::string test_name = file_name + 
		std::string(max_num_test.size() - std::min(max_num_test.size(), cur_test.size()), '0') 
		+ cur_test;
	std::ofstream test(test_name + suffix);
	std::ofstream test_out(test_name + suffix_out);
	int res = 0;
	std::set<int> s;
	while (n_el > 0 or n_q > 0) {
		if (n_el > 0 and std::rand() % 2 == 0) {
			int num = std::rand();
			test << "k " << num << " ";
			s.insert(num);
			--n_el;
		}
		else if (n_q > 0) {
			int l = std::rand();
			int r = l + std::rand() % (RAND_MAX - l) + 1;
			assert(l < r);
			test << "q " << l << ' ' << r << " ";
			--n_q;
			res = range_query(s, l, r);
			test_out << res << " ";
		}
	}
	test.close();
	test_out.close();
}

int main(int argc, char **argv) {
	// file which contains params for generation
	std::string file_name = "gen_params.txt";
	std::string out_file = "tests/test_";

	if (cmd_option_exists(argv, argv + argc, "-b")) {
		file_name = "gen_params_benchmark.txt";
		out_file = "tests/benchmark/test_";
	}

	// attach an input stream to the wanted file
	std::ifstream data(file_name);
	
	// check stream status
	if (!data) {
		std::cerr << "Can't open input file!";
		return 1;
	}

	int n_line = 1;
	int n_el, n_q;
	std::srand(0);
	while (data >> n_el) {
		data >> n_q;
		assert(std::cin.good());
		if (n_el < 0 or n_q < 0) {
			std::cerr << "Incorrect input in line: " << n_line << "\n";
			return 1;
		}
		gen_test(n_el, n_q, out_file, n_line);
		++n_line;
	}

}
