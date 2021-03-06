#pragma once

//////////////////////////////////////////////////////////////////////////////
//					Boost Interprocess Library Headers
//////////////////////////////////////////////////////////////////////////////

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>

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


//////////////////////////////////////////////////////////////////////////////
//					
//////////////////////////////////////////////////////////////////////////////

#include <string>
#include <array>


#include <utilities.hpp>

//using namespace boost::interprocess;

namespace bi = boost::interprocess;



//Typedefs of allocators and containers
typedef bi::managed_mapped_file::segment_manager                       segment_manager_t;
//typedef managed_shared_memory::segment_manager                       segment_manager_t;
typedef bi::allocator<void, segment_manager_t>                           void_allocator;
typedef bi::allocator<char, segment_manager_t>                           char_allocator;
typedef bi::basic_string<char, std::char_traits<char>, char_allocator>   char_string;



struct RNode {
	typedef std::size_t UuidT;

	UuidT uuid;

	RNode(void) = delete;

	RNode(UuidT in_uuid) : uuid(in_uuid) { }

	friend
	std::ostream&
	operator<< (std::ostream& os, const RNode& rn)
	{
		os << "{uuid:" << rn.uuid << "}";
		return os;
	}
};




template <typename StringT>
struct TagDef {
	StringT name;
	StringT type;
	StringT description;

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
	bool
	operator== (const TagDef& lhs, const TagDef& rhs)
	{
		return ( lhs.name == rhs.name );
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

	template <typename StringInputT>
	TagVal(StringInputT in_value, const void_allocator& void_alloc)
		: value(in_value.begin(), in_value.end(), void_alloc)
	{ }

	template <typename StringInputT>
	bool
	compare (StringInputT compare_value)
	{
		if (value.size() != compare_value.size())
			return false;
		
		return std::equal(value.begin(), value.end(), compare_value.begin());
	}

	friend
	std::ostream&
	operator<< (std::ostream& os, const TagVal& tv)
	{
		os << tv.value;
		return os;
	}
};




//	TagDef Array container
typedef TagDef<char_string> TagDefT;

typedef bi::allocator<TagDefT, segment_manager_t> TagDefTAlloc;

typedef bi::list<TagDefT, TagDefTAlloc> ListTagDefT;
	

//	RNode Array container
typedef bi::allocator<RNode, segment_manager_t> RNodeAlloc;

typedef bi::list<RNode, RNodeAlloc> ListRNodeT;
	
	
//	TagVal Array container
typedef TagVal<char_string> TagValT;

typedef bi::allocator<TagValT, segment_manager_t> TagValTAlloc;

typedef bi::list<TagValT, TagValTAlloc> ListTagValT;

	

//	New pair typedefs (use iterators)
typedef std::pair<ListRNodeT::iterator, ListTagValT::iterator> RNodeValPairT;

typedef std::pair<ListTagDefT::iterator, ListTagValT::iterator>TagDefValPairT;


	
typedef bi::allocator<RNodeValPairT, segment_manager_t> RNodeValPairTAlloc;
	
typedef bi::allocator<TagDefValPairT, segment_manager_t> TagDefValPairTAlloc;

typedef bi::list<RNodeValPairT, RNodeValPairTAlloc> ListRNodeValPairT;
	
typedef bi::list<TagDefValPairT, TagDefValPairTAlloc> ListTagDefValPairT;



//	The "list-of-lists" used to store the lists of tags/rnodes associated with the list of rnodes/tags, respectively.

typedef bi::allocator<ListRNodeValPairT, segment_manager_t> ListRNodeValPairTAlloc;

typedef bi::list<ListRNodeValPairT, ListRNodeValPairTAlloc> RNodeValPairTListList;

typedef bi::allocator<ListTagDefValPairT, segment_manager_t> ListTagDefValPairTAlloc;

typedef bi::list<ListTagDefValPairT, ListTagDefValPairTAlloc> TagDefValPairTListList;



std::ostream&
operator<< (std::ostream& os, const RNodeValPairT& rn_tv_pair)
{
	os << "{rnode uuid:" << *rn_tv_pair.first
		<< ",value:\"" << *rn_tv_pair.second
		<< "\"}";

	return os;
}


std::ostream&
operator<< (std::ostream& os, const TagDefValPairT& td_tv_pair)
{
	os << "{tagdef:" << *td_tv_pair.first
	<< ",value:\"" << *td_tv_pair.second
	<< "\"}";

	return os;
}


//template <typename T>
class PlatoDB {
public:

	void_allocator			void_alloc;
	
	ListTagDefT*			allTagDefs;

	ListRNodeT*				allRNodes;

	ListTagValT*			allTagVals;

	TagDefValPairTListList*	td_tv_pair_sets_for_each_rn;

	RNodeValPairTListList*	rn_tv_pair_sets_for_each_td;
	
	std::size_t* last_uuid;

public:

	PlatoDB () = delete;

	//typedef bi::managed_shared_memory ManagedSharedMemoryT;
	typedef bi::managed_mapped_file ManagedSharedMemoryT;

	PlatoDB (ManagedSharedMemoryT& segment)
		: void_alloc(segment.get_segment_manager())
	{
		allTagDefs = segment.find_or_construct<ListTagDefT>("TagDefArray")(void_alloc);
		allRNodes = segment.find_or_construct<ListRNodeT>("RNodeArray")(void_alloc);
		allTagVals = segment.find_or_construct<ListTagValT>("TagValArray")(void_alloc);
		td_tv_pair_sets_for_each_rn = segment.find_or_construct<TagDefValPairTListList>("RNodeValPairArray")(void_alloc);
		rn_tv_pair_sets_for_each_td = segment.find_or_construct<RNodeValPairTListList>("TagDefValPairArray")(void_alloc);

		last_uuid = segment.find_or_construct<std::size_t>("LastUuid")();
	}

	
	PlatoDB (ManagedSharedMemoryT& segment, const void_allocator& input_alloc)
		: void_alloc(input_alloc)
	{
		allTagDefs = segment.find_or_construct<ListTagDefT>("TagDefArray")(void_alloc);
		allRNodes = segment.find_or_construct<ListRNodeT>("RNodeArray")(void_alloc);
		allTagVals = segment.find_or_construct<ListTagValT>("TagValArray")(void_alloc);
		td_tv_pair_sets_for_each_rn = segment.find_or_construct<TagDefValPairTListList>("RNodeValPairArray")(void_alloc);
		rn_tv_pair_sets_for_each_td = segment.find_or_construct<RNodeValPairTListList>("TagDefValPairArray")(void_alloc);

		last_uuid = segment.find_or_construct<std::size_t>("LastUuid")();
	}


	~PlatoDB ()
	{

	}


	void
	print_basic_diagnostics (std::ostream& os)
	{
		os << "! Basic Diagnostic Information About PlatoDB Object:" << std::endl;
		os << "!\tSize of allTagDefs:\t" << allTagDefs->size() << std::endl;
		os << "!\tSize of allRNodes:\t" << allRNodes->size() << std::endl;
		os << "!\tSize of allTagVals:\t" << allTagVals->size() << std::endl;
		os << "!\tSize of td_tv_pair_sets_for_each_rn:\t" << td_tv_pair_sets_for_each_rn->size() << std::endl;
		os << "!\tSize of rn_tv_pair_sets_for_each_td:\t" << rn_tv_pair_sets_for_each_td->size() << std::endl;
		os << std::endl;
		return;
	}
	
	///////////////////////////////////////////////////////////////////////////
	//					Iterator Helpers
	///////////////////////////////////////////////////////////////////////////

	//	Given an iterator to an element in a list of tagdefs, return the appropriate set of {rnode,tagval} pairs
	RNodeValPairTListList::iterator
	get_rnode_set_for_tagdef (ListTagDefT::iterator tagdef)
	{
		return get_equivalent_iterator(allTagDefs->begin(), tagdef, rn_tv_pair_sets_for_each_td->begin());
	}

	//	Given an iterator to an element in a list of rnodes, return the appropriate set of {tagdef,tagval} pairs
	TagDefValPairTListList::iterator
	get_tagdef_set_for_rnode (ListRNodeT::iterator rnode)
	{
		return get_equivalent_iterator(allRNodes->begin(), rnode,
					td_tv_pair_sets_for_each_rn->begin());
	}

	//	Inverse function of get_rnode_set_for_tagdef
	ListTagDefT::iterator
	get_tagdef_for_rnode_set (TagDefValPairTListList::iterator rnode_set)
	{
		return get_equivalent_iterator(td_tv_pair_sets_for_each_rn->begin(), rnode_set
						, allTagDefs->begin());
	}

	//	Inverse function of get_tagdef_set_for_rnode
	ListRNodeT::iterator
	get_rnode_for_tagdef_set (RNodeValPairTListList::iterator tagdef_set)
	{
		return get_equivalent_iterator(rn_tv_pair_sets_for_each_td->begin(), tagdef_set, allRNodes->begin());
	}


/*	
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

*/
  public:
	

	template <typename StringInputT>
	ListTagDefT::iterator
	//create_tag (char_string name, char_string type, char_string description)
	create_tag (StringInputT name, StringInputT type, StringInputT description)
	{
		//	Check first to make sure that the tag doesn't exist
		auto search_for_tag = get_tag(name);

		if (search_for_tag != allTagDefs->end())
		{
			return search_for_tag;
			
		}

		allTagDefs->emplace_front(name, type, description, void_alloc);
		rn_tv_pair_sets_for_each_td->push_front(ListRNodeValPairT(void_alloc));
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
			});
		return td;
	}


	ListTagDefT::iterator
	tags_begin(void) const
	{
		return allTagDefs->begin();
	}
	
	ListTagDefT::iterator
	tags_end(void)
	{
		return allTagDefs->end();
	}

	
	void
	delete_tag (ListTagDefT::iterator tagDef)
	{
		//	First, check that tag still exists
		if (iter_is_in_range(allTagDefs->begin(), allTagDefs->end(), tagDef))
		{

		//	Need to find all tag instances which will be removed
		auto taggedFiles = get_rnode_set_for_tagdef (tagDef);
		

		//	Iterate through the references to tagval instances and erase
		//	them from allTagVals, then go through the tag sets within for
		//	each rnode and erase those elements
		for (auto rnodeTagValPair : *taggedFiles)
		{
			auto val_instance = rnodeTagValPair.second;
			
			allTagVals->erase(val_instance);

			auto tagdef_set = get_tagdef_set_for_rnode (rnodeTagValPair.first);

			//	Remove the element which has the same tagdef as what we're trying to remove
			//	This could be done more efficiently without list::remove_if,
			//	but we can change that later.
			tagdef_set->remove_if ( pair_first_equal_to<TagDefValPairT>(tagDef) );
		}


		//	Erase the set of {rnode, tagval} pairs for the tagdef we're deleting
		rn_tv_pair_sets_for_each_td->erase( taggedFiles );
		
		//	Finally, erase the tagdef
		allTagDefs->erase(tagDef);
		
		}
		else
		{
			std::cout << "Tag couldn't be deleted, it didn't exist!" << std::endl;
		}
	}

	/*

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
		++(*last_uuid);
		allRNodes->emplace_front(*last_uuid);

		td_tv_pair_sets_for_each_rn->push_front(ListTagDefValPairT(void_alloc));
		return allRNodes->begin();
	}

	std::size_t
	count_rnodes (void) const
	{
		return allRNodes->size();
	}

	ListRNodeT::iterator
	get_rnode (RNode::UuidT uuid)
	{
		auto rn = std::find_if (allRNodes->begin(), allRNodes->end(),
			[&](RNode x){
				return (x.uuid == uuid);
			}
			);
		return rn;
	}

	ListRNodeT::iterator
	rnodes_begin (void)
	{
		return allRNodes->begin();
	}

	ListRNodeT::iterator
	rnodes_end (void)
	{
		return allRNodes->end();
	}



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
	
			auto rnode_set = get_rnode_set_for_tagdef (td_pair.first);

			rnode_set->remove_if(pair_first_equal_to<RNodeValPairT>(rnode));
		}

		//	Erase the set of {tagdef, tagval} pairs for the rnode we're deleting
		td_tv_pair_sets_for_each_rn->erase( td_pair_set );

		//	Finally, erase the rnode
		allRNodes->erase(rnode);
	}


	template <typename StringInputT>
	ListTagValT::iterator
	add_tag_to_rnode (ListTagDefT::iterator tagdef, ListRNodeT::iterator rnode, StringInputT value)
	{
		allTagVals->emplace_front(value, void_alloc);
		
		auto tagval = allTagVals->begin();

		auto rnode_set = get_rnode_set_for_tagdef (tagdef);

		auto tagdef_set = get_tagdef_set_for_rnode (rnode);

		rnode_set->emplace_front(std::make_pair(rnode, tagval));

		tagdef_set->emplace_front(std::make_pair(tagdef, tagval));

		return tagval;
	}


	ListTagDefValPairT::iterator
	rnode_tag_set_begin(ListRNodeT::iterator rn)
	{
		return get_tagdef_set_for_rnode(rn)->begin();
	}

	ListTagDefValPairT::iterator
	rnode_tag_set_end(ListRNodeT::iterator rn)
	{
		return get_tagdef_set_for_rnode(rn)->end();
	}



	ListRNodeValPairT::iterator
	tagdef_rnode_set_begin(ListTagDefT::iterator td)
	{
		return get_rnode_set_for_tagdef(td)->begin();
	}

	ListRNodeValPairT::iterator
	tagdef_rnode_set_end(ListTagDefT::iterator td)
	{
		return get_rnode_set_for_tagdef(td)->end();
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
										, pair_first_equal_to<TagDefValPairT>(tagdef)
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
										, pair_first_equal_to<RNodeValPairT>(rnode)
										);

		//	Now remove the {rnode, tagval} pair
		rn_tv_pair_set->erase(rn_tv_pair);

		//	Or we could use remove_if:
		//rn_tv_pair_set->remove_if(pair_first_equal_to(rnode));
	}

	template <typename StringInputT>
	bool
	//ListRNodeT::iterator
	find_rnode_tag_with (ListTagDefT::iterator tagdef, StringInputT value)
	{
		auto rnode_set = get_rnode_set_for_tagdef(tagdef);

		auto found_iter = std::find_if(rnode_set->begin(), rnode_set->end(), 
		
			[&](RNodeValPairT x) {
				return (x.second->compare(value));
			}			
			);

		//return found_iter;

		return (found_iter != rnode_set->end());
	}

	template <typename StringInputT>
	
	ListRNodeT::iterator
	get_rnode_tag_with (ListTagDefT::iterator tagdef, StringInputT value)
	{
		auto rnode_set = get_rnode_set_for_tagdef(tagdef);

		auto found_iter = std::find_if(rnode_set->begin(), rnode_set->end(), 
		
			[&](RNodeValPairT x) {
				return (x.second->compare(value));
			}			
			);

		return (found_iter->first);

		//return (found_iter != rnode_set->end());
	}


	template <typename StringInputT>
	void
	modify_rnode_tag (ListTagDefT::iterator tagdef, ListRNodeT::iterator rnode, StringInputT value)
	{
		//	First find the tagdef's rnode set
		auto rnode_set = get_rnode_set_for_tagdef(tagdef);

		//	Now find the {rnode, tagval} pair which matches the rnode
		auto rn_tv_pair = std::find_if(rnode_set->begin(), rnode_set->end()
				, pair_first_equal_to<RNodeValPairT>(rnode));

		//	Now update the value at the tagvalue position
		char_string new_value (value.begin(), value.end(), void_alloc);
		rn_tv_pair->second->value = new_value;

		//	Or by the rnode's tagdef set
		//auto tagdef_set = get_tagdef_set_for_rnode(rnode);
	}

};
