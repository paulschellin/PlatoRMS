#pragma once


#include <map>
#include <set>

#include <TagDef.hpp>
#include <RNode.hpp>



typedef std::map FileMap_MapT;
typedef std::set FileMap_TagSetT;

typedef std::shared_ptr shared_ptrT



struct shared_RNode_less_than {
	bool
	operator() (const shared_ptrT<RNode>& x, const shared_ptrT<RNode>& y) const
	{
		return ( (*x) < (*y) );
	}
};


typedef TagMap_MapT < shared_ptrT<RNode>
					, FileMap_TagSetT< >
					, shared_RNode_less_than()
					> FileMap;



