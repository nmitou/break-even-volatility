/*
	Simulating pseudo random numbers
*/

#include <iostream>
#include <cstdlib>
using namespace std;

int main() {

	cout << "Generating pseudo-random numbers:" << endl;
	// Set the seed
	srand(123);
	// Generate random numbers in loop
	for (int i = 0; i < 5; i++)
		cout << rand() << " ";

	cout << '\n';

	// Generating random numbers within range
	int lb = 10, ub = 20;
	cout << "Generating pseudo-random numbers in [" << lb << ", " << ub << "]:" << endl;
	for (int i = 0; i < 30; i++)
		cout << (rand() % (ub - lb + 1)) + lb << " ";

	cout << '\n';
	
	// Generating array of random numbers
	int array_size = 10;
	int random_array[array_size];
	for (int i = 0; i < array_size; i++)
		random_array[i] = (rand() % (ub - lb + 1)) + lb;
	cout << "Reading from array of random numbers:" << endl;
	for (int i = 0; i < array_size; i++) 
		cout << random_array[i] << " ";

	return 0;
}