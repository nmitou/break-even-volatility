/*
	Simulating normal random variables with default random number generator or
	Mersenne Twister.
*/
#include <iostream>
#include <random>
#include <array>

using namespace std;

template <class t, size_t N> double mean(const std::array<t, N> &arr);
template <class t, size_t N> double variance(const std::array<t, N> &arr);

int main() {
	std::random_device rd{};
	// std::default_random_engine generator{rd()};
	std::mt19937 generator{rd()};
	std::normal_distribution <double> distribution(0, 1);

	const int n = 100000;
	std::array <double, n> p;

	cout << "Simulating " << n << " normal random numbers:" << endl;

	for (int i = 0; i < n; i++) 
		p[i] = distribution(generator);
	
	for (int i = 0; i < 10; i++)
		cout << p[i] << " ";

	cout << "...\n";

	cout << "Calculating sample statistics:" << endl;

	cout << "Average = " << mean(p) << endl 
			<< "Variance = " << variance(p);

	return 0;
}

template <class t, size_t N> 
double mean(const std::array<t, N> &arr) {
	double sum = 0;
	for (const auto& x : arr) 
		sum += x;
	// return sum/arr.size();
	return sum/N;
}

template <class t, size_t N>
double variance(const std::array<t, N> &arr) {
	double avg = mean(arr);
	double sum = 0;
	for (const auto& x : arr)
		sum += (x-avg)*(x-avg);
	return sum/(N-1);
}