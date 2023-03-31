// Testing lambda functions

#include <iostream>
#include <functional>
#include "../utils.h"

auto invoke_func(double x, std::function<double(double)> func) {
	return func(x);
}

double parabola(double x) {
	return x*x - 4;
}

int main() {
	auto my_fn = [] () -> double { return 1.0; };
	std::cout << my_fn() << std::endl;

	auto my_fn2 = [] (int x) -> double {
		double y = (double) x;
		return y + 0.123;
	};

	std::cout << my_fn2(2) << std::endl;

	double factor = 2.2; // pounds per kg

	auto ConvertLbToKg = [factor] (double lbs) {
		double result = lbs / factor;
		return result;
	};

	std::cout << "2.2 pounds = " << ConvertLbToKg(2.2) << " kilograms" << std::endl;
	std::cout << "10.56 pounds = " << ConvertLbToKg(10.56) << " kilograms" << std::endl;

	auto ConvertKgToLb = [&] (double kgs) -> double {
		double result = kgs * factor;
		return result;
	};

	std::cout << "1 kilogram = " << ConvertKgToLb(1) << " pounds" << std::endl;
	std::cout << "4.8 kilograms = " << ConvertKgToLb(4.8) << " pounds" << std::endl;

	std::cout << "Invoking function:\n1 kilogram = " << invoke_func(1, ConvertKgToLb) << " pounds" << std::endl;
	std::cout << "Invoking function:\n1 pound = " << invoke_func(1, ConvertLbToKg) << " kilograms" << std::endl;

	std::cout << "Finding root of f(x) = x^2 - 4:" << std::endl <<
					"With starting point = 5: " << bev_utils::RootBySecantMethod(parabola, 5.0) << std::endl <<
					"With starting point = 3: " << bev_utils::RootBySecantMethod(parabola, 3.0) << std::endl <<
					"With starting point = 1: " << bev_utils::RootBySecantMethod(parabola, 1.0) << std::endl <<
					"With starting point = 0: " << bev_utils::RootBySecantMethod(parabola, 0) << std::endl <<
					"With starting point = -1: " << bev_utils::RootBySecantMethod(parabola, -1.0) << std::endl <<
					"With starting point = -3: " << bev_utils::RootBySecantMethod(parabola, -3.0) << std::endl <<
					"With starting point = -5: " << bev_utils::RootBySecantMethod(parabola, -5.0) << std::endl;

	return 0;
}