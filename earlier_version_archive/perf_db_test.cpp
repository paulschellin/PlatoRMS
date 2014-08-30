//	g++ -std=c++11 -Wall perf_db_test.cpp -o perf_db_test -lpthread -lboost_program_options -lboost_serialization -lboost_system -lboost_iostreams -lboost_filesystem

#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <iterator>
#include <iomanip>
#include <climits>

#include <string>
#include <sstream>
#include <ctime>
#include <map>
#include <list>
#include <set>
#include <vector>

#include <exception>

#include <random>
#include <assert.h>


#include <boost/filesystem.hpp>
#include <boost/version.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/range.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>

#include <boost/iterator.hpp>
#include <boost/iterator/function_input_iterator.hpp>
//#include <boost/function_input_iterator.hpp>



#include <boost/multi_index_container.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/key_extractors.hpp>
//#include <boost/multi_index/
#include <boost/next_prior.hpp>


#include <boost/program_options.hpp>



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
#include <boost/serialization/assume_abstract.hpp>

/*
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
 */

/*
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/moment.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/max.hpp>
 */


using namespace boost::multi_index;

namespace fs = boost::filesystem;

namespace po = boost::program_options;

/*
namespace bl = boost::log;
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
*/

class TagDef {
  private:
  	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int /*version*/)
	{
		/*
		ar & name;
		ar & type;
		ar & description;
		*/
		ar & BOOST_SERIALIZATION_NVP(name);
		ar & BOOST_SERIALIZATION_NVP(type);
		ar & BOOST_SERIALIZATION_NVP(description);
	}
  public:
	std::string name;
	std::string type;
	std::string description;

	TagDef (void)
		: name()
		, type()
		, description()
	{ }

	TagDef (std::string name_in, std::string type_in = "", std::string desc_in = "")
		: name (name_in)
		, type (type_in)
		, description (desc_in)
	{ }

	~TagDef () {}

	friend std::ostream& operator<< (std::ostream& os, const TagDef& tagdef)
	{
		//os << " { " << tagdef.name << "\t|\t" << tagdef.type << "\t|\t" << tagdef.description << " } ";
		
		os << boost::format("%1% %|20t|%2% %|20t|%3%") % tagdef.name % tagdef.type % tagdef.description;
		return os;
	}

};

static TagDef nullTagDef;


class File {
  private:
  	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int /*version*/)
	{
		ar & BOOST_SERIALIZATION_NVP(name);
		ar & BOOST_SERIALIZATION_NVP(path);
	}
  public:
	//	The File class pretty desperately needs a better unique identifier.

	std::string name;
	std::string path;

	File (void)
		: name()
		, path()
	{ }

	File (std::string name_in, std::string path_in = "")
		: name (name_in)
		, path (path_in)
	{ }


	//File (boost::filesystem::path path_in)
	File (fs::path path_in)
		: name (boost::filesystem::canonical(boost::filesystem::absolute(path_in)).filename().string())
		, path (boost::filesystem::canonical(boost::filesystem::absolute(path_in)).parent_path().string())
	{ }


	~File() {}


	friend std::ostream& operator<< (std::ostream& os, const File& file_in)
	{
		os << boost::format("%1% %|20t| %|20t|%2%") % file_in.name % file_in.path;
		return os;
	}

};

static File nullFile;
//static File nullFile ();


class TagInstance {
  private:
  	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int /*version*/)
	{
		ar & BOOST_SERIALIZATION_NVP(tag);
		ar & BOOST_SERIALIZATION_NVP(file);
		ar & BOOST_SERIALIZATION_NVP(value);
	}
  public:
	TagDef&	tag;
	//const TagDef&	tag;
	//const File&		file;
	File&		file;

	std::string		value;


	//struct tag_name {};
	//struct file_name {};
	struct value {};
	struct tag_name_hash {};
	struct file_name_hash {};
	struct file_path_hash {};


	TagInstance (void)
		: tag(nullTagDef)
		, file(nullFile)
		, value(std::string(""))
	{ }

	//TagInstance (const TagDef& tag_in, const File& file_in, std::string value_in)
	TagInstance (TagDef& tag_in, File& file_in, std::string value_in = "")
		: tag (tag_in)
		, file (file_in)
		, value (value_in)
	{ }

	~TagInstance() {}


	friend std::ostream& operator<< (std::ostream& os, const TagInstance& inst_in)
	{
		//os << " { " << inst_in.tag << "\t|\t" << inst_in.file << "\t|\t" << inst_in.value << " } ";

		os << boost::format("%1% %|40t|%2% %3%") % inst_in.tag % inst_in.file % inst_in.value;
		return os;
	}


};

struct tag_name {
	typedef std::string result_type;

	result_type
	operator() (const TagInstance& inst) const
	{
		return inst.tag.name;
	}
};

struct file_name {
	typedef std::string result_type;

	result_type
	operator() (const TagInstance& inst) const
	{
		return inst.file.name;
	}
};


typedef multi_index_container
			< TagInstance
			, indexed_by
				//< ordered_non_unique< tag_name >
				< ordered_non_unique< tag_name >
				, ordered_non_unique< file_name >
				//, ordered_non_unique< file_name >
				>
			> tag_instance_set;


/*
template <typename MultiIndexContainer>
void multi_index_serialize (const MultiIndexContainer& m)
{
	typedef typename MultiIndexContainer::iterator iterator;
	typedef typename MultiIndexContainer::const_iterator const_iterator;

	std::ostringstream oss;
	{
		boost::archive::xml_oarchive oa(oss);

		oa << m;

		std::vector<const_iterator>	its(m.size());

		const_iterator				it_end=m.end();

		for (const_iterator it = m.begin(); it != it_end; ++it)
		{
			its.push_back(it);
			oa << const_cast<const const_iterator&>(it_end);
		}
		oa << const_cast<const const_iterator&>(it_end);
	}

}
*/


struct generator {
	typedef std::string result_type;
	generator() { srand(time(0)); }
	result_type operator() () const {
		std::stringstream ss;

		ss << rand();
		//rand() >> ss;
		return ss.str();
		//return std::string(reinterpret_cast<char>(rand()));
	}
};


template <typename Iterator, typename RandomGenerator>
Iterator select_randomly (Iterator first, Iterator last, RandomGenerator& g)
{
	std::uniform_int_distribution<> dist (0, std::distance(first, last) - 1);
	std::advance(first, dist(g));
	return first;
}

template <typename Iterator>
Iterator select_randomly (Iterator first, Iterator last)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	return select_randomly(first, last, gen);
}


template <typename Iterator>
struct generate_random_iterator {
	typedef Iterator result_type;
	Iterator first;
	Iterator last;
	std::random_device rd;
	std::mt19937 gen;

	generate_random_iterator(Iterator first_in, Iterator last_in)
		: first (first_in)
		, last (last_in)
		, rd ()
		, gen (rd())
	{
		//srand(time(0));
	}

	result_type operator() () const {
		//Iterator newIt = first;
		//std::advance(newIt, 
		return select_randomly (first, last, gen);
	}
};


/*
template <typename Iterator>
Iterator random_element (Iterator first, Iterator last)
{
	const unsigned long n = std::distance(first, last);
	const unsigned long divisor = (RAND_MAX + 1) / n;

	unsigned long k;

	do {
		k = std::rand() / devisor;
	} while (k >= n);

	return std::advance(first, k);
}
*/

/*
struct make_random_iter {
	


};
*/


//std::ostream& operator<< (std::ostream& os, 


struct recursive_directory_range {
	typedef boost::filesystem::recursive_directory_iterator iterator;

	recursive_directory_range (boost::filesystem::path p)
		: p_ (p)
	{ }

	iterator begin() { return boost::filesystem::recursive_directory_iterator(p_); }

	iterator end() { return boost::filesystem::recursive_directory_iterator(); }

	boost::filesystem::path p_;
};


std::vector< File >
make_fake_file_container (void)
{
	std::vector< File > result {File("test.txt", "/home/paulschellin/"), File("my_src.cpp", "/home/paulschellin/src/"), File("todo.txt", "/home/paulschellin/"), File("my_script.sh", "/home/paulschellin/scripts/"), File(".vimrc", "/etc/")};
	return result;
}


std::vector< File >
make_file_container (fs::path p)
{
	std::vector< File > result;
	for (auto it : recursive_directory_range(p))
	{
		auto it_path = fs::path(it);
		try {
			if (fs::exists(it_path)) {
				if (fs::is_regular_file(it_path)) {
					auto absCanonical = fs::canonical(fs::absolute(it_path));
					result.push_back(File(absCanonical));
				}
			}
		}
		catch (const fs::filesystem_error& ex) {
			std::cout << ex.what() << std::endl;
		}
	}

	return result;
}


int rc_handler (std::ostream& os, po::variables_map& varMap, int ac, char* av[])
{
	using std::string;
	using std::endl;

	try {
		po::options_description generalOpts;

		generalOpts.add_options()
			("help,h", po::value<string>()->implicit_value("help"), "displays this help message (optionally gives further help on requested topic/command)")
			("verbose,v", po::value<int>()->default_value(1)->implicit_value(1), "changes program output verbosity to the desired level")
			("version,V", "displays version and compilation information")
			("input-dir", po::value<std::string>()->default_value(".plato/"), "specify the directory in which to search for input files")
			("input-file,i", po::value<std::string>()->default_value("plato_db.xml"), "specify the input archive for deserialization")
			("output-dir", po::value<std::string>()->default_value(".plato/"), "specify the output directory")
			("output-file,o", po::value<std::string>()->default_value("plato_db.xml"), "specify the output archive for serialization")
			("log-file,l", po::value<std::string>()->default_value("plato_db.log"), "specify the file name used for the program's log")
			("filesystem", po::value<std::string>()->default_value("."), "specify the filesystem to be used with Plato")
			("fake-files", po::value<bool>()->implicit_value(true), "use a fake set of files instead")
			("fake-tags", po::value<bool>()->implicit_value(true), "use a fake set of tags instead")
		;

		po::options_description configOpts("Additional configuration file commands available (located in specified input directory)");

		configOpts.add_options()
			("backup-file", po::value<std::string>(), "don't actually expect this to be used")
		;

		po::options_description cmdlineOpts;

		cmdlineOpts.add(generalOpts).add(configOpts);

		po::options_description visibleOpts("Supported command-line options");

		visibleOpts.add(generalOpts);

		po::positional_options_description posOpts;
		//posOpts.add("", 1);
		//posOpts.add("", 2);
		posOpts.add("filesystem", -1);

		
		po::store(po::command_line_parser(ac, av).options(cmdlineOpts).positional(posOpts).run(), varMap);

		po::notify(varMap);

		
		if (varMap.count("help")) {
			os << visibleOpts << endl
				<< configOpts << endl;
			exit(1);
		}
		
		if (varMap.count("version")) {
			os << __FILE__
				<< ", version "
				<< "<unspecified> (set preprocessor macro \"PLATO_DB_VERSION\" to specify version string)" << std::endl
				<< "Compiled on " << __DATE__ << " at " << __TIME__ << std::endl
				<< "Compiler version: " << __VERSION__ << std::endl
				<< "Boost library version: " << BOOST_LIB_VERSION << std::endl;
			exit(0);
		}
	
	} catch (std::exception& e) {std::cerr << "error: " << e.what() << std::endl; exit(1);
	} catch (...) {std::cerr << "Exception of unknown type occurred." << std::endl; exit(1);}
	return 0;
}


template <typename T>
void
serialize_to_xml (const T& obj, std::string of_name)
{
	fs::path of_path(fs::absolute(fs::path(of_name)));
	std::ofstream ofs (of_path.string());
	assert(ofs.good());
	
	boost::archive::xml_oarchive oa (ofs);
	
	oa << BOOST_SERIALIZATION_NVP(obj);
	
	ofs.close();
}


int
main (int argc, char* argv[])
{
	po::variables_map varMap;
	rc_handler (std::cout, varMap, argc, argv);
	
/*
	//	Initialize Boost Log
	bl::add_console_log()->set_filter();

	bl::add_file_log
	(
		bl::keywords::file_name = "",
		bl::keywords::format = "[%TimeStamp%]: %Message%"
	);
	
	
	bl::core::get()->set_filter
	(
		bl::trivial::severity >= bl::trivial::info
	);
*/

	const int N_INSTANCES = 60;
	
	generator my_gen;

	std::vector< TagDef >	tags {TagDef("file size", "bytes")
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
								, TagDef("document")
								};


	//std::map < std::string, std::function<void(int)> > tagToFsFn;

/*
	std::copy 
			( boost::make_function_input_iterator(my_gen, 8)
			, boost::make_function_input_iterator(my_gen, N_TAGS)
			, std::back_inserter(tags));
*/


/*
	std::copy
			( boost::make_function_input_iterator(my_gen, N_TAGS)
			, boost::make_function_input_iterator(my_gen, N_TAGS + N_FILES)
			, std::back_inserter(files));
*/

	
	std::string pathString = varMap["filesystem"].as<std::string>();

	fs::path p (pathString);
	
	std::vector< File > files (make_file_container(p));

	std::cout << "Tags:" << std::endl;
	boost::copy (tags, std::ostream_iterator<TagDef> (std::cout, "\n"));
	std::cout << std::endl;



	typedef std::vector< TagDef >::iterator tagdef_iterator;

	//generate_random_iterator< tagdef_iterator > tagIterGen (boost::begin(tags), boost::end(tags));
	std::vector< tagdef_iterator > tag_iterators;

/*
	std::copy ( boost::make_function_input_iterator( tagIterGen, 0)
			  , boost::make_function_input_iterator( tagIterGen, N_INSTANCES)
			  , std::back_inserter(tag_iterators));
*/

	for (unsigned i = 0; i < N_INSTANCES; ++i)
	{
		tag_iterators.push_back(select_randomly(boost::begin(tags), boost::end(tags)));
	}



	typedef std::vector< File >::iterator file_iterator;

	std::vector< file_iterator > file_iterators;


	for (unsigned i = 0; i < N_INSTANCES; ++i)
	{
		file_iterators.push_back(select_randomly(boost::begin(files), boost::end(files)));
	}






/*
	generate_random_iterator<file_iterator> fileIterGen (boost::begin(files), boost::end(files));


	std::copy ( boost::make_function_input_iterator( fileIterGen, 0)
			  , boost::make_function_input_iterator( fileIterGen, N_INSTANCES)
			  , std::back_inserter(tag_iterators));
*/

	//boost::copy (tag_iterators, std::ostream_iterator<tagdef_iterator> (std::cout, "\n"));


	/*
	for (auto elem : tag_iterators)
	{
		std::cout << std::distance(boost::begin(tags), elem) << std::endl;
	}
	*/


	std::vector< TagInstance > tagInstances;

	
	for (unsigned i = 0; i < N_INSTANCES; ++i)
	{
		tagInstances.push_back(TagInstance((*tag_iterators[i]), (*file_iterators[i])));
	}


	/*
	std::cout << "Instances:" << std::endl;
	boost::copy (tagInstances, std::ostream_iterator<TagInstance>(std::cout, "\n"));
	std::cout << std::endl;
	*/


	tag_instance_set tagSet;


	for (auto elem : tagInstances)
	{
		tagSet.insert(elem);
	}


	std::cout << "tags.size() --> " << tags.size()  << std::endl;
	std::cout << "files.size() --> " << files.size()  << std::endl;

	std::cout << "tagSet.get<0>().size() --> " << tagSet.get<0>().size()  << std::endl;
	std::cout << "tagSet.get<1>().size() --> " << tagSet.get<1>().size() << std::endl;

	//std::cout << "Number of files: " << tagSet.get<file_name>().size() << std::endl;



	serialize_to_xml (tags, std::string("tags_") + varMap["output-file"].as<std::string>());
	serialize_to_xml (files, std::string("files_") + varMap["output-file"].as<std::string>());
	serialize_to_xml (tagSet, std::string("tagSet_") + varMap["output-file"].as<std::string>());

	/*
	{
		std::string of_name = std::string("tags_") + varMap["output-file"].as<std::string>();

		fs::path of_path(fs::absolute(fs::path(of_name)));
	
		std::ofstream ofs (of_path.string());
	
		std::cout << "Wrote serialized output to " << of_name << std::endl;
	
		assert(ofs.good());
		boost::archive::xml_oarchive oa (ofs);
		oa << BOOST_SERIALIZATION_NVP(tags);
		ofs.close();
	}

	{
		std::string of_name = std::string("files_") + varMap["output-file"].as<std::string>();

		fs::path of_path(fs::absolute(fs::path(of_name)));
	
		std::ofstream ofs (of_path.string());
	
		std::cout << "Wrote serialized output to " << of_name << std::endl;
	
		assert(ofs.good());
		boost::archive::xml_oarchive oa (ofs);
		oa << BOOST_SERIALIZATION_NVP(files);
		ofs.close();
	}
	
	{
		std::string of_name = std::string("tagSet_") + varMap["output-file"].as<std::string>();

		fs::path of_path(fs::absolute(fs::path(of_name)));
	
		std::ofstream ofs (of_path.string());
	
		std::cout << "Wrote serialized output to " << of_name << std::endl;
	
		assert(ofs.good());
		boost::archive::xml_oarchive oa (ofs);
		oa << BOOST_SERIALIZATION_NVP(tagSet);
		ofs.close();
	}
	*/
	return 0;
}
