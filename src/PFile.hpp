

#pragma once

#include <iostream>

#include <string>
#include <vector>

#include <boost/filesystem.hpp>


#define PFILE_USE_BOOST_SERIALIZE 1


#ifdef PFILE_USE_BOOST_SERIALIZE 
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




class PFile {

#ifdef PFILE_USE_BOOST_SERIALIZE

  private:
	friend class boost::serialization::access;

	template <typename Archive>
	void serialize (Archive& ar, const unsigned in //version
	) {
		ar & name;
		ar & fs_path;
		ar & fs_path_boost;
	}

#endif


  public:
	
	//namespace fs = boost::filesystem;
  	
	typedef std::string StringT;

	StringT name;
	StringT fs_path;

	boost::filesystem::path fs_path_boost;

	PFile (void) = delete;

	PFile (const StringT& name_in, const StringT& fs_path_in = StringT(""), const boost::filesystem::path& fs_path_boost_in = boost::filesystem::path())
		: name (name_in)
		, fs_path (fs_path_in)
		, fs_path_boost (fs_path_boost_in)
	{

	}


	friend
	std::ostream&
	operator<< (std::ostream& os, const PFile& obj)
	{
		os << "PFile: {" << obj.name << ", " << obj.fs_path << ", " << obj.fs_path_boost << "}";
		return os;
	}


	friend
	bool
	operator< (const PFile& lhs, const PFile& rhs)
	{
		return ( lhs.fs_path < rhs.fs_path );

	}
};



struct recursive_directory_range {
	typedef boost::filesystem::recursive_directory_iterator iterator;

	recursive_directory_range (boost::filesystem::path p)
		: p_ (p)
	{ }

	iterator begin() { return boost::filesystem::recursive_directory_iterator(p_); }

	iterator end() { return boost::filesystem::recursive_directory_iterator(); }

	boost::filesystem::path p_;
};


namespace ExampleFiles {

	std::vector< PFile > AssortedFiles
								{ PFile("test.txt", "/home/paulschellin/test.txt", boost::filesystem::path("/home/paulschellin/test.txt"))
								, PFile("my_src.cpp", "/home/paulschellin/src/my_src.cpp", boost::filesystem::path("/home/paulschellin/src/my_src.cpp"))
								, PFile("todo.txt", "/home/paulschellin/todo.txt", boost::filesystem::path("/home/paulschellin/todo.txt"))
								, PFile("my_script.sh", "/home/paulschellin/scripts/my_script.sh", boost::filesystem::path("/home/paulschellin/scripts/my_script.sh"))
								, PFile(".vimrc", "/etc/.vimrc", boost::filesystem::path("/etc/.vimrc"))
								};

}
