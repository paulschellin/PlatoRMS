// Compile with:	
// g++ -std=c++11 -Wall -I src/ src/plato_client.cpp -o bin/plato_client

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

#include <functional>
#include <utility>

#include <algorithm>
#include <iterator>
#include <iostream>

#include <plato_daemon_common.hpp>

/*
#include <TagMap.hpp>
#include <FileMap.hpp>
*/

template <typename T>
std::ostream&
operator<<(std::ostream& os, const std::pair<T, T>& inPair)
{

	os << "{ " << inPair.first << " : " << inPair.second << " }";

	return os;
}

int main ()
{
	namespace bi = boost::interprocess;


	/*
	//	Remove shared memory on construction and destruction
	struct shm_remove {
		shm_remove() { bi::shared_memory_object::remove("PlatoDaemonSharedMemory"); }
		~shm_remove() { bi::shared_memory_object::remove("PlatoDaemonSharedMemory"); }
	} remover;

	const unsigned shm_region_size = 65536;
	
	bi::managed_shared_memory segment (bi::create_only
									, "PlatoDaemonSharedMemory"
									, shm_region_size);

	*/
	
	typedef bi::string KeyType;
	//typedef std::string KeyType;
	typedef bi::string MappedType;
	//typedef std::string MappedType;

	typedef std::pair<const KeyType, MappedType> ValueType;


	typedef bi::allocator<ValueType, bi::managed_shared_memory::segment_manager>	ShmemAllocator;


	typedef bi::map<KeyType, MappedType, std::less<KeyType>, ShmemAllocator> MyMap;

	/*
	//	Initialize the shared memory STL-compatible allocator
	ShmemAllocator alloc_inst (segment.get_segment_manager());


	//	Construct a shared memory map
	//	Note that the first parameter is the comparison function,
	//	and the second one is the allocator.
	//	This is the same signature as std::map's constructor taking an allocator

	MyMap* mymap = segment.construct<MyMap>("MyMap")	//	Object name
												(std::less<KeyType>(), alloc_inst);

	for (;;)
	{
		mymap->insert(ValueType("", ""));
	}

	*/

	bi::managed_shared_memory segment(bi::open_only, "PlatoDaemonSharedMemory");

	std::cout << "Shm region size is: " << segment.get_size() << std::endl;

	/*
	 	.find
		.find_no_lock
	 */

	MyMap* mymap = segment.find<MyMap>("MyMap").first;

	
	std::cout << "Distance: " << std::distance(mymap, segment.find<MyMap>("MyMap").second) << std::endl;

	
	//std::copy( mymap->begin(), mymap->end(), std::ostream_iterator<ValueType>(std::cout, "\n"));

	for (auto kvPair : *mymap)
	{
		std::cout << "{ " << kvPair.first << " : " << kvPair.second << " }" << std::endl;

	}

	std::cout.flush();

	return 0;
}

