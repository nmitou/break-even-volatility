/*
	Using std::partial_sum to cumulatively sum an array.
*/

#include <iostream>
#include <array>
#include <numeric>

void cumsum(std::array<int, 5>& p);

int main() {
	std::array<int, 5> p {1,2,3,4,5};

	for (int i=0; i<5; i++)
		std::cout << p[i] << " ";

	std::cout << '\n';

	// std::partial_sum(p.begin(), p.end(), p.begin());
	cumsum(p);

	for (int i=0; i<5; i++)
		std::cout << p[i] << " ";	

	return 0;
}

void cumsum(std::array<int, 5>& p) {
	std::partial_sum(p.begin(), p.end(), p.begin());
}