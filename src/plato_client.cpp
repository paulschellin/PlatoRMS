// Compile with:	
// g++ -std=c++11 -Wall -I src/ src/plato_client.cpp -o bin/plato_client

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>

#include <vector>
#include <string>
#include <utility>

#include <iostream>

using namespace boost::interprocess;

//Typedefs of allocators and containers
typedef managed_shared_memory::segment_manager                       segment_manager_t;
typedef allocator<void, segment_manager_t>                           void_allocator;
typedef allocator<int, segment_manager_t>                            int_allocator;
typedef vector<int, int_allocator>                                   int_vector;
typedef allocator<int_vector, segment_manager_t>                     int_vector_allocator;
typedef vector<int_vector, int_vector_allocator>                     int_vector_vector;
typedef allocator<char, segment_manager_t>                           char_allocator;
typedef basic_string<char, std::char_traits<char>, char_allocator>   char_string;


/*
template <typename T>
std::ostream&
operator<<(std::ostream& os, const std::pair<T, T>& inPair)
{

	os << "{ " << inPair.first << " : " << inPair.second << " }";

	return os;
}
*/

//Definition of the map holding a string as key and complex_data as mapped type
typedef std::pair<const char_string, char_string>                      map_value_type;
typedef std::pair<char_string, char_string>                            movable_to_map_value_type;
typedef allocator<map_value_type, segment_manager_t>                    map_value_type_allocator;
typedef map< char_string, char_string
           , std::less<char_string>, map_value_type_allocator>          shared_map_type;


int main ()
{
	//namespace bi = boost::interprocess;

	//using namespace PlatoDaemon;


	managed_shared_memory segment(open_only, "PlatoDaemonSharedMemory");

	//std::cout << "Shm region size is: " << segment.get_size() << std::endl;

	/*
	 	.find
		.find_no_lock
	 */

	shared_map_type* mymap = segment.find<shared_map_type>("MyMap").first;


	
	//std::copy( mymap->begin(), mymap->end(), std::ostream_iterator<ValueType>(std::cout, "\n"));

	//for (auto kvPair : *mymap)
	for (auto iter = mymap->begin(); iter != mymap->end(); ++iter)
	{
		std::cout << "{ " << std::string(iter->first.begin(), iter->first.end())
				<< " : " << std::string(iter->second.begin(), iter->second.end())
				<< " }" << std::endl;

	}

	std::cout.flush();

	return 0;
}

