#pragma once


#include <vector>
#include <string>

#include <TagDef.hpp>

namespace ExampleTags {

	std::vector<TagDef> NormalTags = { TagDef("file size", "bytes")
								 , TagDef("executable", "bool")
									 , TagDef("readable", "bool")
								, TagDef("writable", "bool")
								, TagDef("user")
								, TagDef("atime", "ns")
								, TagDef("ctime", "ns")
								, TagDef("mtime", "ns")
								, TagDef("color")
								, TagDef("media")
								, TagDef("tv")
								, TagDef("music")
								, TagDef("extension", "string")
								, TagDef("picture")
								, TagDef("document")};

}
