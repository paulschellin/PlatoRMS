#pragma once
/*
	In our file system, we have two general groupings of things:
	
		- tags
		- files/resource nodes (RNodes)
	
	The RNodes are pretty uniform when it comes to their size, but the tags could have
	multiple implementations:

		- the tag is a simple label (so it points back to the TagDef object)
		- the tag represents an attribute and the associated value and is either
			- comparable in size with a pointer, or
			- much larger than a pointer
	
	When we look at how we believe the file system will be accessed, it becomes
	clear that some combination of these representations is desirable.


	It's beneficial to think about what someone might be doing with their
	file system. They might be using ls, and xgetattr, etc. to understand the
	file better, or if they were using Plato, remember that the block
	allocators for data rely on tag values. That's a good example to start with:
	
	In the case of the block allocator, the information can be relatively small --
	it might be an indirect block pointer, or it might represent the entire
	extent. Most important about this type of TagVal is that there is very little
	need to see it while iterating/searching through TagDefs, but of course it
	is critical when you have the RNode address in your hand.

	What about the other way around though?

	Certainly there are values which are more important to see while searching
	through many files associated with specific tags, like representing
	associations -- thats what file systems are designed to do!

	So there is your data, which you care about, but there also the meta data,
	which you also care about. A file doesn't do any good taking up space on
	your hard drive if you have no meaningful way of accessing it!


	Sometimes there is a clear choice as to which is better, and why it's better.
	Even large TagVals have a place in the FileSet (if the tag is designed for,
	say, searching through the content of the file like an inverted index). That
	sort of tag doesn't seem very useful at all if you've already got the file
	open, though.


	So the idea is this:

		[ ]	Some tags will just be labels, because that's all they are.
			[ ] Some of these tags are more important when looking at a TagSet
					than when looking at a FileSet.
		[ ] Some tags have TagVals which are small enough to be desirable to fit
				into the TagSet/FileSet, even at the expense of being able to fit
				more Label-type elements in.
		[ ] Some tags have TagVals which are either too large or not useful enough
				to bother putting them in the TagSet/FileSet, so a pointer suffices
		[ ] Because there are multiple levels of depth in both the tag and file set,
				there is a natural way to implement a type of ordering.
			[ ] The ordering should be such that all elements of a set at a 
					particular level are all of the same type, the entire set can
					be processed in a predictable way.

		[!] When a TagInstance is removed from a TagSet/FileSet, it would leave a
				gap which would stay around until it gets filled -- by a tag of the
				same sort -- which means that you may be leaving spots empty and
				still requiring jumps to the next contiguous region.

	
	Keep in mind that reading is not all that is done with files. Files are
	created (a pretty cheap process), files are deleted (



	Estimated constraints we have to work with:

		Rate of creation:
			TagDefs < RNodes < TagVals (assuming that most tags aren't labels)

		Rate of deletion:
			TagDefs < RNodes < TagVals


		Expected quantity:
			TagDefs < RNodes < TagVals


		Individual size (minimum):
			TagVals < RNodes < TagDefs

		Individual size (maximum):
			RNodes < TagDefs < TagVals

		




 */



#include <TagDef.hpp>
#include <RNode.hpp>

#include <array>
#include <utility>

/*
 	N should be picked such that the entire TagSet node (including the "next" pointer)
	fits inside of a cache line (64 Bytes).
 */
template <std::size_t N>
struct NodeSet {
	
	std::array<TagDef*, N> set;
	TagSet* next = null;

};



/*
	When TagVal is relatively small (not too much larger than std::size_t),
	it might make sense to put the actual values into the pair.
 */


template <typename NodeT, std::size_t N, std::size_t M>
struct NodeValPairSet {
	typedef std::pair<NodeT*, TagVal<M> > TagDefValPair;

	std::array<TagDefValPtrPair, N> set;
	TagSet* next = null;

};




template <typename NodeT, std::size_t N, std::size_t M>
struct NodeValPtrPairSet {
	
	typedef std::pair<NodeT*, TagVal<M>*> TagDefValPtrPair;

	std::array<TagDefValPtrPair, N> set;
	TagSet* next = null;

};

