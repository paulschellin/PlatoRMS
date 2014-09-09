
#include <algorithm>
#include <iterator>
#include <functional>
#include <iostream>
#include <string>

#include <vector>
#include <utility>

#include <utilities.hpp>


bool test_string_literal (void)
{
	std::string a ("Hello, world!");

	auto b = "Hello, world!"_s;

	return (a == b);
}


bool test_advance_copy (void)
{
	std::vector<int> vec {5,2,1,3,-1,0};

	auto it = advance_copy(vec.begin(), 4);

	return (*it == -1);
}

bool test_iter_at (void)
{
	std::vector<int> vec {5,2,1,3,-1,0};

	auto it = iter_at(vec, 4);

	return (*it == -1);
}

bool test_get_equivalent_iterator (void)
{
	std::vector<int> vec1 {5,2,1,3,-1,0};
	std::vector<int> vec2 {5,2,1,3,-22,0};

	auto it1 = iter_at(vec1, 4);

	auto it2 = get_equivalent_iterator(vec1.begin(), it1, vec2.begin());

	return (*it2 == -22);
}

bool test_pair_first_equal_to (void)
{

	std::vector< std::pair< int, int > > vecPairs {
							{0,99},
							{1,98},
							{2,97},
							{3,96},
							{4,95},
							{5,94}
						};
	
	auto it = std::find_if(vecPairs.begin(), vecPairs.end(), pair_first_equal_to<std::pair<int,int>>(4));

	return (it->second == 95);
}

bool test_pair_second_equal_to (void)
{

	std::vector< std::pair< int, int > > vecPairs {
							{0,99},
							{1,98},
							{2,97},
							{3,96},
							{4,95},
							{5,94}
						};
	
	auto it = std::find_if(vecPairs.begin(), vecPairs.end(), pair_second_equal_to<std::pair<int,int>>(97));

	return (it->first == 2);
}


/*
bool test_make_ostream_iterator (void)
{
	std::vector<int> vec {5,2,1,3,-22,0};

	std::copy(vec.begin(),vec.end(), make_ostream_iterator(std::cout, ", "));
	return true;
}
*/


bool test_pair_printer (void)
{

	std::vector< std::pair< int, int > > vecPairs {
							{0,99},
							{1,98},
							{2,97},
							{3,96},
							{4,95},
							{5,94}

		};
	
	std::cout << make_pair_printer(vecPairs.at(3), "index", "value") << std::endl;

	return true;
}

bool test_container_printer (void)
{
	/*
	std::vector< std::pair< int, int > > vecPairs {
							{0,99},
							{1,98},
							{2,97},
							{3,96},
							{4,95},
							{5,94}

		};
	*/
	std::vector<int> vec {5,2,1,3,-22,0};

	std::cout << make_container_printer(vec) << std::endl;

	return true;
}



int main ()
{
	std::cout << "test_string_literal" << ": " << test_string_literal() << std::endl;
	std::cout << "test_advance_copy" << ": " << test_advance_copy() << std::endl;
	std::cout << "test_iter_at" << ": " << test_iter_at() << std::endl;
	std::cout << "test_get_equivalent_iterator" << ": " << test_get_equivalent_iterator() << std::endl;
	std::cout << "test_pair_first_equal_to" << ": " << test_pair_first_equal_to() << std::endl;
	std::cout << "test_pair_second_equal_to" << ": " << test_pair_second_equal_to() << std::endl;
	//std::cout << "test_make_ostream_iterator" << ": " << test_make_ostream_iterator() << std::endl;

	std::cout << "test_pair_printer" << ": " << test_pair_printer() << std::endl;
	std::cout << "test_container_printer" << ": " << test_container_printer() << std::endl;



	return 0;
}
