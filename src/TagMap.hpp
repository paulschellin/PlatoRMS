#pragma once


#include <map>
#include <set>

#include <TagDef.hpp>
#include <RNode.hpp>



typedef std::map TagMap_MapT;
typedef std::set TagMap_FileSetT;

typedef std::shared_ptr shared_ptrT



struct shared_TagDef_less_than {
	bool
	operator() (const shared_ptrT<TagDef>& x, const shared_ptrT<TagDef>& y) const
	{
		return ( (*x) < (*y) );
	}
};


typedef TagMap_MapT < shared_ptrT<TagDef>
					, TagMap_FileSetT< >
					, shared_TagDef_less_than()
					> TagMap;


