#pragma once


//#include <atomic>
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



