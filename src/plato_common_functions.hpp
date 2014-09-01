#pragma once

#include <string>

#include <prototype_structs.hpp>


/*
		Plato Common Tag Manipulation Functions

	We need to be able to do things with our tags and rnodes, and we need to
	be able to do so safely. Here are the basic functions we need:


		Tag Operations

	create_tag (StringT name, StringT type, StringT description)


	delete_tag (TagDefT* tagDef)


	delete_tag (StringT tagName)
	delete_tag (TagDefT* tagName)

	
	list_tags (TagDefContainerT& tags)
	list_tags (StringT tagName)


	modify_tag (TagDefT* tagDef, StringT name, StringT type, StringT description)

	

	
	duplicate_tag (StringT tagName)
	duplicate_tag (TagDefT* tagDef)

	merge_tags (TagDefT* tagDefToMerge, TagDefT* tagDefToKeepName)


		
		RNode Operations

	//	Might want to think about what create_rnode should be passed
	//	Just passing a container of tagdefs doesn't necessarily make sense
	create_rnode (UuidT uuid, TagDefContainerT& tagsForNewRNode)

	delete_rnode (RNode* rnode)


	list_rnodes (RNodeContainerT& rnodes)



		Combined Operations

	add_tag_to_rnode (RNode* rnode, TagDefT* tagDef, StringT tagVal)


	remove_tag_from_rnode (RNode* rnode, TagDefT* tagDef)


	list_rnode_tags (RNode* rnode)
	list_rnode_tags_and_vals (RNode* rnode)


	modify_rnode_tag (RNode* rnode, TagDefT* tagDef, StringT tagVal)

	

	list_tag_rnodes (TagDefT* tagDef)

	list_tag_rnodes_and_vals (TagDefT* tagDef)


	Depending on how the database is implemented, these should either return the pointer
	to the tag/rnode, or add/remove the tag/rnode to the appropriate container of those objects.

	
	

		Scope Operations

	create_scope (...)

	delete_scope (ScopeT* scope)

	list_rnodes_scope (ScopeT* scope)
	
	
	add_rnode_to_scope (ScopeT* scope, RNode* rnode)

	remove_rnode_from_scope (ScopeT* scope, RNode* rnode)


	scope_intersection (ScopeT* x, ScopeT* y)

	scope_union (ScopeT* x, ScopeT* y)

	scope_difference (ScopeT* x, ScopeT* y)

	scope_symmetric_difference (ScopeT* x, ScopeT* y)


 */



/*
		Needed usage example:

	Suppose we have the following data structures:

	ListTagDefT*		tagArray

	ListRNodeT*			rnodeArray

	ListTagValT*		tagValArray

	RNodeValPairTSet*	rnodeValPairSet

	TagDefValPairTSet*	tagDefValPairSet

	ListRNodeValPairTSet*	rnodeValPairSetArray

	ListTagDefValPairTSet*	tagDefValPairSetArray


	They're going to start off empty, for this example.

	We want to add a TagDef. That tagdef is going to be the tag "filename".


	create_tag (tagArray, "filename", "string", "the name of the file") { tagArray.emplace_back(name, type, description); }



	We also want to create a "filesystem" tag:

	TagDefT* fs_tag = create_tag (tagArray, "filesystem", "string", "the filesystem of the actual file")

	TagDefT* path_tag = create_tag (tagArray, "path", "string", "the full path of the file on the filesystem")

	TagDefT* path_tag = name_tag (tagArray, "path", "string", "the name of the file")


	TagDefT* size_tag = create_tag (tagArray, "size", "int", "the size of the file in bytes")


	Now we want to add a file to the system. Suppose that file is located at "~/testfile.txt"

	RNode* new_rnode = create_rnode (rnodeArray);

	We the rnode is going to represent a file on our (ext4, etc.) filesystem

	path p ("~/testfile.txt");

	
	add_tag_to_rnode (new_rnode, fs_tag, "/")

	add_tag_to_rnode (new_rnode, path_tag, p.string());

	add_tag_to_rnode (new_rnode, name_tag, filename(p).string());

	add_tag_to_rnode (new_rnode, size_tag, file_size(p));

	


 */

