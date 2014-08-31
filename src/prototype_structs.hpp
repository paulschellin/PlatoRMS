#pragma once

//////////////////////////////////////////////////////////////////////////////
//					Boost Interprocess Library Headers
//////////////////////////////////////////////////////////////////////////////

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/deque.hpp>
#include <boost/interprocess/containers/list.hpp>
#include <boost/interprocess/containers/slist.hpp>

#include <boost/interprocess/containers/set.hpp>
#include <boost/interprocess/containers/flat_set.hpp>
#include <boost/interprocess/containers/flat_map.hpp>


#include <boost/interprocess/smart_ptr/shared_ptr.hpp>
#include <boost/interprocess/smart_ptr/deleter.hpp>



#include <string>
#include <array>


struct RNode {

	std::size_t uuid;
	//std::atomic_size_t reference_count;
	RNode(std::size_t in_uuid) : uuid(in_uuid) { }
};




template <typename StringT>
struct TagDef {
	StringT name;
	StringT type;
	StringT description;

	TagDef(StringT in_name, StringT in_type, StringT in_description) : name(in_name), type(in_type), description(in_description) { }

	friend
	bool
	operator< (const TagDef& lhs, const TagDef& rhs)
	{
		return ( lhs.name < rhs.name );
	}
};




template <typename StringT>
struct TagVal {

	StringT value;

	TagVal(StringT in_value) : value(in_value) { }

};


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


//Definition of the map holding a string as key and char_string as mapped type
typedef std::pair<const char_string, char_string>                      map_value_type;
typedef std::pair<char_string, char_string>                            movable_to_map_value_type;
typedef allocator<map_value_type, segment_manager_t>                    map_value_type_allocator;
typedef map< char_string, char_string
           , std::less<char_string>, map_value_type_allocator>          shared_map_type;





	//	TagDef Array container
	typedef TagDef<char_string> TagDefT;


	typedef allocator<TagDefT, segment_manager_t> TagDefTAlloc;

	typedef list<TagDefT, TagDefTAlloc> ListTagDefT;

	//ListTagDefT* tagArray = segment->construct<ListTagDefT>("TagDefArray")(alloc_inst);

	

	//	RNode Array container
	typedef allocator<RNode, segment_manager_t> RNodeAlloc;

	typedef list<RNode, RNodeAlloc> ListRNodeT;

	//ListRNodeT* rnodeArray = segment->construct<ListRNodeT>("RNodeArray")(alloc_inst);

	
	
	//	TagVal Array container
	typedef TagVal<char_string> TagValT;

	typedef allocator<TagValT, segment_manager_t> TagValTAlloc;

	typedef list<TagValT, TagValTAlloc> ListTagValT;

	//ListTagValT* tagValArray = segment->construct<ListTagValT>("TagValArray")(alloc_inst);

	

	//	RNode Set container
	typedef std::pair<offset_ptr<RNode>, offset_ptr<TagValT> > RNodeValPairT;

	typedef std::pair<offset_ptr<TagDefT>, offset_ptr<TagValT> > TagDefValPairT;

	
	typedef allocator<RNodeValPairT, segment_manager_t> RNodeValPairTAlloc;
	
	typedef allocator<TagDefValPairT, segment_manager_t> TagDefValPairTAlloc;

	typedef list<RNodeValPairT, RNodeValPairTAlloc> ListRNodeValPairT;
	
	typedef list<TagDefValPairT, TagDefValPairTAlloc> ListTagDefValPairT;


