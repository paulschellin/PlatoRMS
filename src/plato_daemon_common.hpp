#pragma once



#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>

#include <boost/interprocess/allocators/allocator.hpp>


#include <functional>
#include <utility>

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>

#include <memory>



namespace PlatoDaemon {
	namespace bi = boost::interprocess;

	typedef bi::managed_mapped_file ManagedSharedMemoryT;
	//typedef bi::managed_shared_memory ManagedSharedMemoryT;

	typedef ManagedSharedMemoryT::segment_manager SegmentManagerT;

	typedef bi::allocator<char, ManagedSharedMemoryT::segment_manager> SharedCharAllocator;

	typedef bi::allocator<void, ManagedSharedMemoryT::segment_manager> SharedVoidAllocator;

	typedef bi::basic_string<
						  char
						, std::char_traits<char>
						, SharedCharAllocator
						> SharedStringT;


	typedef const SharedStringT KeyType;
	typedef SharedStringT MappedType;


	typedef std::pair<KeyType, MappedType> ValueType;
	//typedef std::pair<const KeyType, MappedType> ValueType;

	typedef bi::allocator<ValueType, ManagedSharedMemoryT::segment_manager> SharedValueTypeAllocator;

	typedef bi::map<KeyType, MappedType, std::less<KeyType>, SharedValueTypeAllocator> MyMap;





}
