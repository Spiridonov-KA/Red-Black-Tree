#include <algorithm>
#include <chrono>
#include <iostream>
#include <set>
#include <vector>

template<typename C, typename T>
int range_query(const C &s, T fst, T snd) {
	using itt = typename C::iterator;
	itt start = s.lower_bound(fst);
	itt fin = s.upper_bound(snd);
	return std::distance(start, fin);
}

bool cmd_option_exists(char** begin, char** end, const std::string& option) {
    return std::find(begin, end, option) != end;
}

int main(int argc, char **argv) {
	char cmd;
	int num, l, r;
	std::vector<int> arr;
	std::vector<int> results;
	int max_size = 200'000'000;
	results.reserve(max_size);
	std::set<int> s;
	auto start = std::chrono::steady_clock::now();
	while (std::cin >> cmd) {
		if (cmd == 'k') {
			std::cin >> num;
			s.insert(num);
			arr.push_back(num);
		}
		else {
			std::cin >> l >> r;
			int res = range_query(s, l, r);
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

	if (!cmd_option_exists(argv, argv + argc, "-h")) {
		for (int i = 0; i < results.size(); ++i)
			std::cout << results[i] << ' ';
		std::cout << std::endl;
	}
	if (cmd_option_exists(argv, argv + argc, "-t")) {
		double tm = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start).count();
		std::cout << tm << std::endl;
	}
}
