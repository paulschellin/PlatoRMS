#pragma once

//////////////////////////////////////////////////////////////////////////////
//					Boost Interprocess Library Headers
//////////////////////////////////////////////////////////////////////////////

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>

//////////////////////////////////////////////////////////////////////////////
//					
//////////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
#include <iterator>
#include <array>
#include <memory>
#include <exception>

namespace bi = boost::interprocess;


//http://stackoverflow.com/questions/25631388/loop-around-file-mapping-kills-performance

/*

struct ManagedMappedType {
	typedef bi::managed_mapped_file shared_memory_type;

};

struct ManagedSharedType {
	typedef bi::managed_shared_memory shared_memory_type;

};


struct FileMappingType {
	typedef bi::file_mapping shared_memory_type;

};

struct MemoryObjectType {
	typedef bi::shared_memory_object shared_memory_type;

};

struct MappedRegionType {
	typedef bi::mapped_region shared_memory_type;
};

*/

template <typename SharedMemoryT>
class GenericSharedMemory {

  public:
	
	typedef typename SharedMemoryT::segment_manager	segment_manager_t;

	typedef bi::allocator<void, segment_manager_t>		void_allocator;
	typedef bi::allocator<char, segment_manager_t>		char_allocator;

	typedef bi::basic_string<char, std::char_traits<char>, char_allocator>	char_string;
	

	std::string shared_region_name;
	bi::mode_t shared_region_access_mode;
	std::size_t shared_region_file_size;

	std::size_t shared_region_offset;

	std::size_t shared_region_size;

	std::shared_ptr<SharedMemoryT>	segment;


	GenericSharedMemory(std::string region_name, std::size_t region_size = 1048576)
		: shared_region_name(region_name)
		, shared_region_file_size(region_size)
		, shared_region_offset()
		, shared_region_size()
		, segment(nullptr)
	{
		try
		{
			segment.reset(new SharedMemoryT(bi::open_or_create, shared_region_name.c_str(), shared_region_file_size));
		}
		catch (const bi::interprocess_exception& e)
		{
			std::cout << "An exception occurred while trying to open the shared memory region \""
				<< shared_region_name << "\". The exception is: "
				<< e.what() << std::endl;
		}
		catch (...)
		{
			std::cout << "An unknown/unexpected exception occurred while trying to open the shared memory region \""
				<< shared_region_name << "\"." << std::endl;
		}

	}


	/*
	GenericSharedMemory()
	{
	}

	~GenericSharedMemory()
	{
	}
	*/


	friend
	std::ostream&
	operator<< (std::ostream& os, GenericSharedMemory& sh_memory)
	{
		auto segment = sh_memory.segment;
		os << std::endl;
		os << "! Basic Shared Memory Diagnostic Information for \"" << sh_memory.shared_region_name << "\":" << std::endl;
		os << "!\tSize of the memory segment: " << segment->get_size() << std::endl;
		os << "!\tFree bytes in the memory segment: " << segment->get_free_memory() << std::endl;
		os << "!\tNumber of named objects in the memory segment: " << segment->get_num_named_objects() << std::endl;
		os << "!\tNumber of unique objects in the memory segment: " << segment->get_num_unique_objects() << std::endl;
		os << std::endl;

		return os;
	}


};
