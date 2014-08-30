

#pragma once

#include <iostream>
#include <string>
#include <vector>


//#define TAGDEF_USE_BOOST_SERIALIZE 1


#ifdef TAGDEF_USE_BOOST_SERIALIZE 
	#include <boost/archive/text_oarchive.hpp>
	#include <boost/archive/text_iarchive.hpp>

	#include <boost/archive/xml_oarchive.hpp>
	#include <boost/archive/xml_iarchive.hpp>

	#include <boost/serialization/version.hpp>
	#include <boost/serialization/vector.hpp>
	#include <boost/serialization/map.hpp>
	#include <boost/serialization/list.hpp>
	#include <boost/serialization/string.hpp>
	#include <boost/serialization/nvp.hpp>
	#include <boost/serialization/utility.hpp>
	#include <boost/serialization/export.hpp>
	#include <boost/serialization/split_member.hpp>
	#include <boost/serialization/assume_abstract.hpp>
#endif

class TagDef {
  
#ifdef TAGDEF_USE_BOOST_SERIALIZE
  private:
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int //version
	) {	
		ar & BOOST_SERIALIZATION_NVP(name);
		ar & BOOST_SERIALIZATION_NVP(type);
		ar & BOOST_SERIALIZATION_NVP(description);
	}


#endif


  public:
	typedef std::string StringT;

	StringT	name;
	StringT type;
	StringT description;


	TagDef (void) = delete;

	TagDef (const StringT& name_in, const StringT& type_in = StringT("None"), const StringT& desc_in = StringT("(none)"))
		: name (name_in)
		, type (type_in)
		, description (desc_in)
	{

	}



	friend
	std::ostream&
	operator<< (std::ostream& os, const TagDef& obj)
	{
		os << "TagDef: {" << obj.name << ", " << obj.type << ", " << obj.description << "}";
		return os;
	}

	
	friend
	bool
	operator< (const TagDef& lhs, const TagDef& rhs)
	{
		return ( lhs.name < rhs.name );
	}
	
};

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
