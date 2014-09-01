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


std::string operator "" _s(const char* str, size_t /*length*/)
{
	return std::string(str);
}

/*
char_string operator "" _sh(const char* str, size_t)
{
	return char_string(str);
}
*/



struct RNode {

	std::size_t uuid;
	//std::atomic_size_t reference_count;
	RNode(std::size_t in_uuid = 0) : uuid(in_uuid) { }
};




template <typename StringT>
struct TagDef {
	StringT name;
	StringT type;
	StringT description;

/*
	TagDef(StringT in_name, StringT in_type, StringT in_description, const void_allocator& void_alloc)
		: name(in_name, void_alloc)
		, type(in_type, void_alloc)
		, description(in_description, void_alloc)
	{ }
*/

	template <typename StringInputT>
	TagDef(StringInputT in_name, StringInputT in_type, StringInputT in_description, const void_allocator& void_alloc)
		: name(in_name.begin(), in_name.end(), void_alloc)
		, type(in_type.begin(), in_type.end(), void_alloc)
		, description(in_description.begin(), in_description.end(), void_alloc)
	{ }


	friend
	bool
	operator< (const TagDef& lhs, const TagDef& rhs)
	{
		return ( lhs.name < rhs.name );
	}

	friend
	std::ostream&
	operator<< (std::ostream& os, const TagDef& td)
	{
		os << "{name:\"" << td.name
			<< "\",type:\"" << td.type
			<< "\",description:\"" << td.description
			<< "\"}";
		return os;
	}
};




template <typename StringT>
struct TagVal {

	typedef StringT value_type;

	StringT value;

	TagVal(StringT in_value, const void_allocator& void_alloc)
		: value(in_value, void_alloc)
	{ }
};








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

	

	//	New pair typedefs (use iterators)
	typedef std::pair<ListRNodeT::iterator, ListTagValT::iterator> RNodeValPairT;

	typedef std::pair<ListTagDefT::iterator, ListTagValT::iterator>TagDefValPairT;


	//	Old pair typedefs (use offset_ptr)
	typedef std::pair<offset_ptr<RNode>, offset_ptr<TagValT> > RNodeValOffsetPairT;

	typedef std::pair<offset_ptr<TagDefT>, offset_ptr<TagValT> > TagDefValOffsetPairT;

	
	typedef allocator<RNodeValPairT, segment_manager_t> RNodeValPairTAlloc;
	
	typedef allocator<TagDefValPairT, segment_manager_t> TagDefValPairTAlloc;

	typedef list<RNodeValPairT, RNodeValPairTAlloc> ListRNodeValPairT;
	
	typedef list<TagDefValPairT, TagDefValPairTAlloc> ListTagDefValPairT;



//	The "list-of-lists" used to store the lists of tags/rnodes associated with the list of rnodes/tags, respectively.

typedef allocator<ListRNodeValPairT, segment_manager_t> ListRNodeValPairTAlloc;

typedef list<ListRNodeValPairT, ListRNodeValPairTAlloc> RNodeValPairTListList;

typedef allocator<ListTagDefValPairT, segment_manager_t> ListTagDefValPairTAlloc;

typedef list<ListTagDefValPairT, ListTagDefValPairTAlloc> TagDefValPairTListList;


template <typename T1, typename T2>
struct pair_first_equal_to {
	T2 value;

	pair_first_equal_to (const T2& val)
		: value(val)
	{}
	
	bool operator() (const T1& x) const {return x.first == value;}
	typedef T1 first_argument_type;
	
	typedef bool result_type;
};

template <typename T1, typename T2>
struct pair_second_equal_to {
	T2 value;

	pair_second_equal_to (const T2& val)
		: value(val)
	{}
	
	bool operator() (const T1& x) const {return x.second == value;}
	typedef T1 first_argument_type;
	
	typedef bool result_type;
};



//template <typename T>
class PlatoDB {
public:

	void_allocator			void_alloc;
	
	ListTagDefT*			allTagDefs;

	ListRNodeT*				allRNodes;

	ListTagValT*			allTagVals;

	RNodeValPairTListList*	allTagDefTagValPairSetsForAnRNode;

	TagDefValPairTListList*	allRNodeTagValPairSetsForATagDef;



public:

	PlatoDB () = delete;

	PlatoDB (managed_shared_memory& segment)
		: void_alloc(segment.get_segment_manager())
	{
		allTagDefs = segment.find<ListTagDefT>("TagDefArray").first;
		allRNodes = segment.find<ListRNodeT>("RNodeArray").first;
		allTagVals = segment.find<ListTagValT>("TagValArray").first;
		allTagDefTagValPairSetsForAnRNode = segment.find<RNodeValPairTListList>("RNodeValPairArray").first;
		allRNodeTagValPairSetsForATagDef = segment.find<TagDefValPairTListList>("TagDefValPairArray").first;
	}

	
	PlatoDB (managed_shared_memory& segment, const void_allocator& input_alloc)
		: void_alloc(input_alloc)
	{
		allTagDefs = segment.find_or_construct<ListTagDefT>("TagDefArray")(void_alloc);
		allRNodes = segment.find_or_construct<ListRNodeT>("RNodeArray")(void_alloc);
		allTagVals = segment.find_or_construct<ListTagValT>("TagValArray")(void_alloc);
		allTagDefTagValPairSetsForAnRNode = segment.find_or_construct<RNodeValPairTListList>("RNodeValPairArray")(void_alloc);
		allRNodeTagValPairSetsForATagDef = segment.find_or_construct<TagDefValPairTListList>("TagDefValPairArray")(void_alloc);
	}


	~PlatoDB ()
	{

	}

/*
	
	///////////////////////////////////////////////////////////////////////////
	//					Iterator Helpers
	///////////////////////////////////////////////////////////////////////////

	//	Given an iterator to an element in a list of tagdefs, return the appropriate set of {rnode,tagval} pairs
	RNodeValPairTListList::iterator
	get_rnode_set_for_tagdef (ListTagDefT::iterator tagdef)
	{
		return allRNodeTagValPairSetsForATagDef.begin() + std::distance(allTagDefs.begin(), tagdef);
	}

	//	Given an iterator to an element in a list of rnodes, return the appropriate set of {tagdef,tagval} pairs
	TagDefValPairTListList::iterator
	get_tagdef_set_for_rnode (ListRNodeT::iterator rnode)
	{
		return allTagDefTagValPairSetsForAnRNode.begin() + std::distance(allRNodes.begin(), rnode);
	}

	//	Inverse function of get_rnode_set_for_tagdef
	ListTagDefT::iterator
	get_tagdef_for_rnode_set (RNodeValPairTListList::iterator rnode_set)
	{
		return allTagDefs.begin() + std::distance(allTagDefTagValPairSetsForAnRNode.begin(), rnode_set);
	}

	//	Inverse function of get_tagdef_set_for_rnode
	ListRNodeT::iterator
	get_rnode_for_tagdef_set (TagDefValPairTListList::iterator tagdef_set)
	{
		return allRNodes.begin() + std::distance(allRNodeTagValPairSetsForATagDef.begin(), tagdef_set);
	}

	
	TagDefValPairTListList::iterator
	get_tagdef_pair_for_rnode_pair (RNodeValPairTListList::iterator rn_tv_pair)
	{
		//	Given an {rnode,tagvalue} pair, we have immediate access to the
		//	rnode and to the tagvalue.
		//	Indirectly, with the rnode we can access the tagdef_set for an
		//	rnode.
		//	The tagdef set is a set of {tagdef, tagvalue} pairs, so all we
		//	need to do is find the pair with the second element equal to
		//	the tagvalue we already have.
		
		//	First find tagval instance
		auto tagval = rn_tv_pair->second;

		//	Get the rnode
		auto rnode = rn_tv_pair->first;

		//	Get the tagdef instance
		auto td_tv_pair_set = get_tagdef_set_for_rnode (rnode);

		//	We need to find the td_tv_pair with the same tagval
		auto td_tv_pair = std::find_if (td_tv_pair_set->begin(), td_tv_pair_set->end()
										, pair_second_equal_to(tagval)
										);

		//	Return that pair
		return td_tv_pair;
	}


	//	Inverse function of get_tagdef_pair_for_rnode_pair
	RNodeValPairTListList::iterator
	get_rnode_pair_for_tagdef_pair (TagDefValPairTListList::iterator td_tv_pair)
	{
		auto tagval = td_tv_pair->second;

		auto tagdef = td_tv_pair->first;

		auto rn_tv_pair_set = set_rnode_set_for_tagdef (tagdef);

		auto rn_tv_pair = std::find_if (rn_tv_pair_set->begin(), rn_tv_pair_set->end()
										< pair_second_equal_to(tagval)
										);

		return rn_tv_pair;
	}


  public:
	*/

	template <typename StringInputT>
	ListTagDefT::iterator
	//create_tag (char_string name, char_string type, char_string description)
	create_tag (StringInputT name, StringInputT type, StringInputT description)
	{
		//return allTagDefs->emplace_back(name, type, description);
		//allTagDefs->emplace_back(name, type, description);
		//return allTagDefs->
	
		allTagDefs->emplace_front(name, type, description, void_alloc);
		return allTagDefs->begin();
	}

	
	std::size_t
	count_tags (void) const
	{
		return allTagDefs->size();
	}

	
	template <typename StringInputT>
	ListTagDefT::iterator
	get_tag (StringInputT comp_name)
	{
		auto td = std::find_if(allTagDefs->begin(), allTagDefs->end(),
			[&](TagDefT x){
				return std::equal(x.name.begin(), x.name.end(), comp_name.begin());
				//return x.name == comp_name;
			});
		return td;
	}

	/*
	get_tags (void) const
	{
		
	}
	*/

	/*
	void
	delete_tag (ListTagDefT::iterator tagDef)
	{
		//	First, check that tag still exists
		//	<to do>


		//	Need to find all tag instances which will be removed
		auto taggedFiles = get_rnode_set_for_tagdef (tagDef);
		

		//	Iterate through the references to tagval instances and erase
		//	them from allTagVals, then go through the tag sets within for
		//	each rnode and erase those elements
		for (auto rnodeTagValPair : *taggedFiles)
		{
			auto val_instance = rnodeTagValPair.second;
			
			allTagVals.erase(val_instance);

			auto tagdef_set = get_tagdef_set_for_rnode (rnodeTagValPair.first);

			//	Remove the element which has the same tagdef as what we're trying to remove
			//	This could be done more efficiently without list::remove_if,
			//	but we can change that later.
			tagdef_set.remove_if (
				[](auto val) {
					return val.first == tagDef;
				}
			);
		}


		//	Erase the set of {rnode, tagval} pairs for the tagdef we're deleting
		allRNodeTagValPairSetsForATagDef.erase( taggedFiles );

		//	Finally, erase the tagdef
		allTagDefs.erase(tagDef);
	}

	ListTagDefT::iterator
	modify_tag_all (ListTagDefT::iterator tagdef, StringT name, StringT type, StringT description)
	{
		tagdef->name = name;
		tagdef->type = type;
		tagdef->description = description;
		return tagdef;
	}

	ListTagDefT::iterator
	modify_tag_name (ListTagDefT::iterator tagdef, StringT name)
	{
		tagdef->name = name;
		return tagdef;
	}

	ListTagDefT::iterator
	modify_tag_type (ListTagDefT::iterator tagdef, StringT type)
	{
		tagdef->type = type;
		return tagdef;
	}

	ListTagDefT::iterator
	modify_tag_description (ListTagDefT::iterator tagdef, StringT description)
	{
		tagdef->description = description;
		return tagdef;
	}

	*/
	
	ListRNodeT::iterator
	create_rnode ()
	{
		//return allRNodes->emplace_back();
		allRNodes->emplace_front();
		return allRNodes->begin();
	}

	std::size_t
	count_rnodes (void) const
	{
		return allRNodes->size();
	}

	/*

	void
	delete_rnode (ListRNodeT::iterator rnode)
	{
		//	Get the set of all {TagDef, tagval} pairs for the rnode we're deleting
		auto td_pair_set = get_tagdef_set_for_rnode (rnode);


		//	Iterate through the refences to tagval instances and erase them from
		//	allTagVals.
		//	Then go through the tagdefs and remove the references to that tagval
		for (auto td_pair : *td_pair_set)
		{
			//	Erase the tagval instance
			allTagVals->erase(td_pair.second);

			//	
			auto rnode_set = get_rnode_set_for_tagdef (td_pair.first);

			rnode_set.remove_if(
				[](auto val){
					return val.first == rnode;
				}
			);

			//rnode_set->remove_if(pair_first_equal_to(rnode));

		}


		//	Erase the set of {tagdef, tagval} pairs for the rnode we're deleting
		allTagDefTagValPairSetsForAnRNode.erase( td_pair_set );

		//	Finally, erase the rnode
		allRNodes.erase(rnode);
	}




	ListTagValT::iterator
	add_tag_to_rnode (ListTagDefT::iterator tagdef, ListRNodeT::iterator rnode, StringT value)
	{
		auto tagval = allTagVals->emplace_back(tagdef, value);


		auto rnode_set = get_rnode_set_for_tagdef (tagdef);

		auto tagdef_set = get_tagdef_set_for_rnode (rnode);

		rnode_set->emplace_back(std::make_pair(rnode, tagval));

		tagdef_set->emplace_back(std::make_pair(tagdef, tagval));

		return tagval;
	}


	void
	remove_tag_from_rnode (
							ListTagDefT::iterator	tagdef
						  , ListRNodeT::iterator	rnode
						  )
	{
		//	First get the iterator to the tagval instance

		//	Get the {tagdef, tagval} pair set for the specified rnode
		auto td_val_pair_set = get_tagdef_set_for_rnode(rnode);

		//	Find the {tagdef, tagval} pair which matches the tagdef
		auto td_val_pair = std::find_if(  td_val_pair_set->begin()
										, td_val_pair_set->end()
										//, [](auto this_td_val_pair){
										//		return this_td_val_pair.first == tagdef;
										//	}
										, pair_first_equal_to(tagdef)
										);

		//	Remove the tagval instance from allTagVals
		allTagVals->erase(td_val_pair->second);

		//	Now remove the {tagdef, tagval} pair from the rnode's set of them
		td_val_pair_set->erase(td_val_pair);


		//	Get the {rnode, tagval} pair set for the specified tagdef
		auto rn_tv_pair_set = get_rnode_set_for_tagdef(tagdef);

		//	Find the {rnode, tagval} pair which matches the rnode
		auto rn_tv_pair = std::find_if( rn_tv_pair_set->begin()
										, rn_tv_pair_set->end()
										//, [](auto this_rn_tv_pair){
										//		return this_rn_tv_pair.first == rnode;
										//	}
										, pair_first_equal_to(rnode)
										);

		//	Now remove the {rnode, tagval} pair
		rn_tv_pair_set->erase(rn_tv_pair);

		//	Or we could use remove_if:
		//rn_tv_pair_set->remove_if(pair_first_equal_to(rnode));
	}


	void
	modify_rnode_tag (ListTagDefT::iterator tagdef, ListRNodeT::iterator rnode, StringT value)
	{
		//	First find the tagdef's rnode set
		auto rnode_set = get_rnode_set_for_tagdef(tagdef);

		//	Now find the {rnode, tagval} pair which matches the rnode
		auto rn_tv_pair = std::find_if(rnode_set->begin(), rnode_set->end(), pair_first_equal_to(rnode));

		//	Now update the value at the tagvalue position
		rn_tv_pair->second->value = value;

		//	Or by the rnode's tagdef set
		//auto tagdef_set = get_tagdef_set_for_rnode(rnode);
	}

	//template <typename Predicate>


	


*/



};



