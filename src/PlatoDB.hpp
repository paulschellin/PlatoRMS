


#include <TagDef.hpp>
#include <PFile.hpp>

typedef pair< weak_ptr<PFile>, shared_ptr<ValueT> > FileValPair;

typedef pair< weak_ptr<TagDef>, shared_ptr<ValueT> > TagDefValPair;


typedef shared_ptr<TagDef>	SharedTagDef;
typedef shared_ptr<PFile>	SharedPFile;

struct shared_TagDef_less {
	bool
	operator() (const shared_ptr<TagDef>& x, const shared_ptr<TagDef>& y) const
	{
		return ( (*x) < (*y) );
	}
};

struct shared_PFile_less {
	bool
	operator() (const shared_ptr<PFile>& x, const shared_ptr<PFile>& y) const
	{
		return ( (*x) < (*y) );
	}
};

typedef map < shared_ptr<TagDef>
    		, set<FileValPair>
			, shared_TagDef_less()
			> TagMap;

typedef map < shared_ptr<PFile.
			, set<TagDefValPair>
			, shared_PFile_less()
			> FileMap;



/*
 	The intersection of two sorted sets is provided as a function template
	in the <algorithm> header.

	std::set_intersection


	In bits/stl_algo.h, it's defined like this:

	while (first1 != last1 && first2 != last2){
		if (comp(*first1, *first2))
			++first1;
		else if (comp(*first2, *first1))
			++first2;
		else {
			*result = *first1;
			++first1;
			++first2;
			++result;
		}
	}
	return result;


	The parallel implementation in parallel/algo.h is:
		(effectively the same?)

	Complexity:
	At most 2 * (N_1 + N_2 - 1) comparisons, where N_1 = std::distance(first1, last1)
	and N_2 = std::distance(first2, last2).

	
	The folded intersections of multiple sets becomes more interesting, as there
	are both many algorithms and many implementations which can do so.

	A naive approach may be:

		given N sorted, unique sets
		take intersection between the zeroth and the ith set, store in jth set
		starting with the zeroth and the first set, for every ith:
			take intersection of ith and (i+1)th set




	std::set is most likely an implementation of a binary search tree

	Structure		Time Complexity	(average | worst)						  	Space Complexity
				  Indexing		  Search		  Insertion		  Deletion				(worst)
Basic array		O(1) | O(1)		O(n) | O(n)		--				--				O(n)
Dynamic array	O(1) | O(1)		O(n) | O(n)		O(n) | O(n)		O(n) | O(n)		O(n)
Binary search	O(log(n)) | O(n)	O(log(n)) | O(n)	O(log(n) | O(n) O(log(n)) | O(n)	O(n)
	tree


 */
template <typename SetT>
SetT
n_set_intersection (typename SetT::const_iterator first, typename ContainerT::const_iterator last, typename ContainerT::value_type::iterator out_first, Compare comp)
{
	SetT result (*first);

	for (auto set_it = first; set_it != last; ++set_it) {
		SetT temp (result);
		std::set_intersection(,  );
	}

}


/*
template< class ForwardIt1, class ForwardIt2 >
ForwardIt1
find_in_name_members( ForwardIt1 first, ForwardIt1 last
 */

template <typename T>
struct equals_name_member_object {

	const T to_match_;

	equals_name_member_object (const T& to_match)
		: to_match_ (to_match)
	{ }

	bool
	operator() (const T& to_compare) const
	{
		return ( to_compare.name == to_match_ );
	}

};


class PlatoDB {

  public:
	TagMap tag_map;
	FileMap file_map;

	
	void
	new_tag (TagDef&& td)
	{
		//shared_ptr<TagDef> td_shared (new TagDef(td));
		
		auto td_shared = make_shared(td);
		tag_map.insert(TagMap::value_type (td_shared, set<TagDefValPair>()); 
	}


	void
	add_tag (shared_ptr<TagDef> td, shared_ptr<PFile> f, ValueT v)
	{
		//shared_ptr<ValueT> v_shared (new ValueT(v));

		auto v_shared = make_shared(v);

		tag_map[td].emplace(f, v_shared);

		file_map[f].emplace(td, v_shared);
	}

	
	void
	remove_tag (shared_ptr<TagDef> td, shared_ptr<PFile> f)
	{
		tag_map[td].erase(f);

		file_map[f].erase(td);
	}


	/*
		How do you call delete_tag(...) in a way that doesn't require some
		out-of-scope object still sharing ownership of the TagDef item?


		auto matchToDelete = find_if (tag_map | adapters::map_keys, equals_name_member_object("tag name to match")); 

		delete_tag(weak_ptr<TagDef>(*matchToDelete));

	 */
	void
	delete_tag (weak_ptr<TagDef> weak_td)
	{
		//	Lock the reference to the tagdef until the scope of delete_tag()
		//	is exited. Passing a weak_ptr instead of a shared_ptr means that
		//	the lifetime of the last owning object can just be a temporary
		//	weak pointer, rather than having a shared_ptr in the enclosing
		//	scope.
		auto shared_td = weak_td.lock();

		for (auto& f = tag_map[shared_td])
			tag_map[f].erase(shared_td);
		tag_map.erase(shared_td);



	//	mem_fn(&set::erase)

	}

};
