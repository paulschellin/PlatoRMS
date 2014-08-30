#include <iostream>

#include <typeinfo>
#include <typeindex>
#include <type_traits>

#include <unordered_map>
#include <string>

#include <vector>

#include <iterator>

#include <algorithm>





struct C {};

int
main()
{
	std::unordered_map<std::type_index, std::string> mytypes;

	mytypes[typeid(int)] = "Integer type";
	mytypes[typeid(double)] = "Floating-point type";

	mytypes[typeid(C)] = "Custom class named C";

	std::cout
		<< "int: " << mytypes[typeid(int)] 	<< "\t" << typeid(int).name() << std::endl
		<< "double: " << mytypes[typeid(double)] << "\t" << typeid(double).name() << std::endl
		<< "C: " << mytypes[typeid(C)] << "\t" << typeid(C).name() << std::endl;

	
	//std::type_index myTypeIndex;

	//std::cout << 

	int i;
 	int * pi;
	std::cout << "int is: " << typeid(int).name() << std::endl
			  << "  i is: " << typeid(i).name() << std::endl
			  << " pi is: " << typeid(pi).name() << std::endl
			  << "*pi is: " << typeid(*pi).name() << std::endl;

/*
	std::cout << "int is: " << typeid(int).hash_code() << std::endl
			  << "  i is: " << typeid(i).hash_code() << std::endl
			  << " pi is: " << typeid(pi).hash_code() << std::endl
			  << "*pi is: " << typeid(*pi).hash_code() << std::endl;
*/
	return 0;
}
