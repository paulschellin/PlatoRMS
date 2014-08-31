#pragma once

//#define RNODE_USE_BOOST_SERIALIZE 1

#include <iostream>
#include <string>
#include <atomic>

#ifdef RNODE_USE_BOOST_SERIALIZE
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


class RNode {

#ifdef RNODE_USE_BOOST_SERIALIZE
  private:
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int //version
	)
	{
		ar & BOOST_SERIALIZATION_NVP(uuid);
	}

#endif


  public:
	
//	typedef std::size_t UuidT;

	std::atomic_size_t reference_count;

//	UuidT uuid;


	RNode () = delete

	RNode (std::size_t in_uuid) : reference_count(1)
				//, uuid(in_uuid)
				{}
/*
	friend
	std::ostream&
	operator<< (std::ostream& os, const RNode& obj)
	{
		//os << "RNode: {UUID=" << obj.uuid << "}";
		return os;
	}

	friend
	bool
	operator< (const RNode& lhs, const RNode& rhs)
	{
		return ( lhs.uuid < rhs.uuid );
	}
*/

};
