



//
//
//	Classes
//
//


class TagDef {
/*	tag name
	extra information/decorators
		detailed text descriptions
		icons (for graphical interfaces)
		pronunciations and weight metrics for verbal interfaces
	value type
	value type definition
	constructor -\
				 |- may be part of the value's type definition?
	destructor	-/
	arrayable
	access and privilege information


*/



};



class RNode {
/*
	list of tag instances

*/
	

};


typedef string ValueT;

typedef pair< weak_ptr< TagDef >
			, ValueT
			> TagValuePair;


typedef unordered_map< shared_ptr<RNode>
					 , vector<TagValuePair>
					 > RNodeMap;

typedef unordered_map< shared_ptr<TagDef>
					 , vector<shared_ptr<RNode>>
					 > InvertedIndex;



/*
	There are many relevant things that must be considered before
	deciding upon how the data will be organized.

	We know there will be many RNode objects (possibly as many as tens of millions),
	so preparing for that will be the most important. There will likely be less than
	tens of thousands of TagDef objects, so that structure is less important.

	Because there are so many RNode objects, we can't assume that they will have
	contiguous memory spaces (so no std::array, no std::vector).


	Using std::map or std::unordered_map with the TagValuePair array as the value
	of the {key, value} pair makes it appear as though it is unnecessary to have
	the container of TagValuePair elements be a member of the RNode class. This
	may be indicative of the RNode being mostly a pointed-to object (rather than a
	searched-for object), so it's possible that if the TagValuePair container is
	a member of the RNode class, the RNodeMap could be changed to be something more
	efficient.

	The RNodes could be without a proper container and instead rely on unnamed pointer
	access styles, but because the RNode object will be managed by a shared_ptr there
	may be a cleaner way to do that.

	We can get away with making RNodes pointer-accessed because, unlike TagDefs, RNode
	renaming makes no sense.

	There may be a good reason to use containers still, just to put frequently-accessed
	RNodes in close proximity to one another in memory.


	Would some kind of embedded shared_ptr system be better than the std::shared_ptr?
	My guess is that it wouldn't be worth the headache.

	
	In a sense, there is no benefit for RNode to even be a defined class -- it seems that
	a typedef of the container of TagValuePair objects would be all that is needed. One
	fewer dereference, and it becomes slightly faster and smaller for no effort.



*/


