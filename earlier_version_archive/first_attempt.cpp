//	g++ -std=c++11 -lboost_serialization -lboost_system -lboost_iostreams -lboost_filesystem first_attempt.cpp -o first_attempt

#include <iostream>
#include <fstream>
#include <string>

#include <vector>

#include <utility>

#include <set>	// collection of unique keys, sorted by keys
#include <map>	// collection of key-value pairs, sorted by keys (keys are unique)

//#include <multiset>	// collection of keys, sorted by keys
//#include <multimap>	// collection of key-value pairs, sorted by keys


//#include <unordered_set>	// collection of unique keys, hashed by keys
//#include <unordered_map>	// collection of key-value pairs, hashed by keys (keys are unique)

//#include <unordered_multiset>	// collection of keys, hashed by keys
//#include <unordered_multimap>	// collection of key-value pairs, hashed by keys

//#include <boost/container/flat_set.hpp>
//#include <boost/container/flat_map.hpp>

#include <algorithm>
#include <functional>
#include <utility>
#include <iterator>

#include <bitset>


#include <boost/tuple/tuple.hpp>

#include <boost/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>
#include <boost/bimap/list_of.hpp>
#include <boost/bimap/unordered_set_of.hpp>


#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <boost/serialization/version.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/utility.hpp>



//	Tag Definitions
class TagDef {
  private:
  	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned int /*version*/)
	{
		ar & BOOST_SERIALIZATION_NVP(tagName)
			& BOOST_SERIALIZATION_NVP(tagDescription)
			& BOOST_SERIALIZATION_NVP(tagType);
	}

  public:
	std::string tagName;

	std::string tagDescription;

	std::string tagType;

	TagDef (void)
		: tagName ("")
		, tagDescription("")
		, tagType ("")
	{ }



	TagDef (std::string name_in, std::string desc_in, std::string type_in)
		: tagName (name_in)
		, tagDescription(desc_in)
		, tagType (type_in)
	{ }
	
	//std::string tagValue;

	//function generating
	
	friend std::ostream& operator<< (std::ostream& os, const TagDef& val);
};

std::ostream&
operator<< (std::ostream& os, const TagDef& val)
{
	os << "[ Tag Definition ]" << std::endl
		<< val.tagName << " :: " << val.tagType << std::endl
		<< "-- " << val.tagDescription << std::endl;
	return os;
}

/*
struct TagInstance {
	TagDef& tagDef;
	//const TagDef& tagDef;

	std::string tagValue;
};
*/




struct File {
	//	A file should have a collection of unique tag instances

	//	This could be represented by a std::set, if we don't care about values

	//	Similarly by an

	
	//	In order to use the set_intersection, _union, _difference, and _symmetric_difference functions though, the ranges must be sorted. So the unordered sets are out of the question.

//	std::set<const TagDef&>	tagsNoValues;

	typedef std::string tag_value_type;

	std::map<TagDef*, std::string>	tags;	
};




/*
//	Tag Lists (Tag -> Files)
class TagsToFiles {
  public:	
	std::map< TagDef&
			, std::map<File&, typename File::tag_value_type>
			>	tagsToFiles;
};



//	File Lists (File -> Tags)
class FilesToTags {
  public:
	std::map< File&
			, std::map<TagDef&, typename File::tag_value_type>
			>	filesToTags;
};
*/

/*
int parse_options (std::ostream& os, po::variables_map& varMap, int argc, char* argv[])
{
	try {
		po::options_description generalOpts;
		generalOpts.add_options()
			("help,h","")
			("add","")
			("rm","")
			("new","")
			("delete","");
	
	
	} catch (std::exception& e) {std::cerr << "error: " << e.what() << std::endl;
	} catch (...) {std::cerr << "An exception of unknown type occurred." << std::endl;}

	return 0;
}
*/




/*
	If a boost::bimap is to be used, then there are a few differences which
	must be taken into consideration:

	 - Because boost::bimaps have "with_info" tags, the value of tag instances
	   can be addressed in that way. This means that no actual "TagInstance"
	   class is necessary for the mapping, though it might be useful elsewhere.

	 - The set types the bimap is tagged with can be "set_of", "multiset_of",
	   "unordered_set_of", and "unordered_multiset_of". In any case that I can
	   think of, we do not want the "multiset" variant, because the multiset
	   allows duplication which makes little sense in Plato. In some cases
	   it might make sense to use the unordered variants, since it uses
	   hashing. That would make fuzzy / regex lookups slower at the expense of
	   an increase in exact-match lookups:

		name				features			map view type
	set_of					ordered, unique		map
	multiset_of				ordered				multimap
	unordered_set_of		hashed, unique		unordered_map
	unordered_multiset_of	hashed				unordered_multimap
	list_of					sequenced			list_map
	vector_of				random access		vector_map
	unconstrained_set_of	unconstrained		<cannot be viewed>


	View the following page for lots of info:

	http://www.boost.org/doc/libs/1_55_0/libs/bimap/doc/html/boost_bimap/the_tutorial/additional_information.html

 */


/*
	For this version, we use a bimap of two "set_of" collection types, both
	containing strings, one for the tag name, the other for the file name.

	At the moment the "with_info" is also a string, containing the attribute
	values of tags in Plato.

	
	Another way to design this is to have two "set_of" collection types which
	contain reference-counted smart pointers to both tag definition instances
	and file object instances. The "with_info" would be either tag value
	instances, or just a simple value instead.

	If the link is to the tag definition object instance, then the lookup
	could not be hashed and the comparison operators would need to be written
	for the definition classes. The other side of that is that the definitions
	can contain the information such as Plato's simplified type-system info,
	which could be used with boost::any in order to get each value in the
	proper type.

 */


struct tag_name {};
struct file_name {};
struct instance_value {};

typedef boost::bimap
			< boost::bimaps::set_of< 	boost::bimaps::tagged< std::string, tag_name > >
			, boost::bimaps::multiset_of< 	boost::bimaps::tagged< std::string, file_name > >
			, boost::bimaps::with_info< boost::bimaps::tagged< std::string, instance_value > >
			> my_bimap_type;

typedef my_bimap_type::value_type tag_instance_type;


/*
	There's a "collection of relations" view of boost::bimap. Lots of
	information here:

	http://www.boost.org/doc/libs/1_55_0/libs/bimap/doc/html/boost_bimap/the_tutorial/the_collection_of_relations_type.html


		Name											Additional Parameters
	left_based										Not a template.
	right_based										Not a template.
	
	set_of_relation<KeyComp>						KeyComp is a Functor that compares two types using less-than. By default, the less-than operator is std::less<_relation>.
	multiset_of_relation<KeyComp>

	unordered_set_of_relation<HashFunctor,EqualKey>			HashFunctor converts the relation into an std::size_t value. By default it is boost::hash<_relation>.
	unordered_multiset_of_relation<HashFunctor,EqualKey>	EqualKey is a Functor that tests two relations for equality. By default, the equality operator is std::equal_to<_relation>.

	list_of_relation								Not a template.
	vector_of_relation								Not a template.
	unconstrained_set_of_relation					Not a template.

 */


typedef boost::bimap
			< boost::bimaps::multiset_of< 	boost::bimaps::tagged< std::string, tag_name > >
			, boost::bimaps::multiset_of< 	boost::bimaps::tagged< std::string, file_name > >
			, boost::bimaps::multiset_of_relation<>
			, boost::bimaps::with_info< boost::bimaps::tagged< std::string, instance_value > >
			> my_relation_type;



/*
	For struct tags Left and Right:


			Untagged version			Tagged version
		bm.left						bm.by<Left>()
		bm.right					bm.by<Right>()
		bm_type::left_map			bm::map_by<Left>::type
		bm_type::right_value_type	bm::map_by<Right>::value_type
		bm_type::left_iterator		bm::map_by<Left>::iterator
		bm_type::right_const_iterator	bm::map_by<Right>::const_iterator
		iter->left					iter->get<Left>()
		iter->right					iter->get<Right>()
		left_iter->first			left_iter->get<Left>()
		left_iter->second			left_iter->get<Right>()
		right_iter->first			right_iter->get<Left>()
		right_iter->second			right_iter->get<Right>()
		bm.project_left(iter)		bm.project<Left>(iter)
		bm.project_right(iter)		bm.project<Right>(iter)


	All from:
		http://www.boost.org/doc/libs/1_55_0/libs/bimap/doc/html/boost_bimap/the_tutorial/bimaps_with_user_defined_names.html
 */


/*
template <typename T>
void
bm_printer (const T& bmap)
{
	for (auto it = bmap.by<tag_name>().find("size"); it != my_bimap.by<tag_name>().end(); ++it)
	{
		std::cout << "Tag Name:\t" << it->get<tag_name>() << std::endl
				  << "File Names:\t" << it->get<file_name>() << std::endl
				  << "Tag Value:\t" << it->get<instance_value>() << std::endl;
	}
}
*/



int
main (int argc, char* argv[])
{
	/*
	std::map <std::string, TagDef*>		allTags;
	TagDef myLocalTag = {"test tag0", "This is a test tag.", "None"};
	allTags.emplace(myLocalTag.tagName, &myLocalTag);

	std::cout << "Actual tag:" << std::endl
			<< myLocalTag << std::endl;
	*/

	//using namespace std::string_literals;

	std::map <std::string, TagDef>	allTags;

	std::ifstream ifs("allTags.xml");
	boost::archive::xml_iarchive ia(ifs);
	ia >> BOOST_SERIALIZATION_NVP(allTags);
	ifs.close();



	//TagDef myLocalTag ("test tag0", "This is a test tag.", "None");

	//allTags.emplace(myLocalTag.tagName, myLocalTag);
	//allTags.emplace(std::make_pair("test tag0", "test tag0", "This is a test tag.", "None"));
	//allTags.emplace(std::make_pair("test tag0", "test tag0", "This is a test tag.", "None"));
	//allTags.emplace(std::make_pair("test tag0", new TagDef("test tag0", "This is a test tag.", "None")));
	
/*	allTags.emplace ( std::piecewise_construct
					, std::make_tuple("test tag1")
					, std::make_tuple("test tag1", "This is another test tag.", "None"));
*/
	//allTags.emplace ("woo", TagDef{"woo", "Haha", "None"});
	
	/*
	allTags.emplace ("media", TagDef{"media", "File contains some sort of media data.", "None"});
	
	allTags.emplace ("music", TagDef{"music", "File contains some sort of music data.", "None"});

	allTags.emplace ("video", TagDef{"video", "File contains some sort of video data.", "None"});
	
	allTags.emplace ("ebook", TagDef{"ebook", "File contains some sort of ebook data.", "None"});
	
	
	allTags.emplace ("tv", TagDef{"tv", "File contains some sort of tv data.", "None"});
	*/


	/*
	std::cout << "Tag from map:" << std::endl
			<< allTags.at("test tag0") << std::endl;

	std::cout << "Tag from map:" << std::endl
			<< allTags.at("woo") << std::endl;
	*/
	
	//std::cout << myLocalTag << std::endl;

//	std::cout << allTags << std::endl;

//	std::copy(allTags.begin(), allTags.end(), std::ostream_iterator(std::cout, "\n"));

	//std::cout << argc << std::endl;

	if (argc == 2) {
		if (!std::string("list").compare(argv[1])/* == "list"*/) {
			for ( auto elem : allTags ) {
				std::cout << elem.second << std::endl;
			}
		}
	} else if (argc > 2) {
		if (!std::string("new").compare(argv[1])/*argv[1] == "new"*/) {
			allTags.emplace (argv[2], TagDef{std::string(argv[2]), std::string(argv[3]), std::string(argv[4])});
		}

		if (!std::string("delete").compare(argv[1])){
			//auto it = allTags.find(std::string(argv[2]));
			//allTags.erase(it);
			allTags.erase(std::string(argv[2]));
		}

		if (!std::string("add").compare(argv[1])){

		}

		if (!std::string("rm").compare(argv[1])){

		}

		//if (!std::string("delete").compare(argv[1])){

		//}


	}


	
	
	std::ofstream ofs("allTags.xml");
	boost::archive::xml_oarchive oa(ofs);
	oa & BOOST_SERIALIZATION_NVP(allTags);
	ofs.close();
	

	std::vector<std::string> alltags {"myTagName000", "myTagName001", "size", "color", "user"};
	
	std::vector<std::string> allfiles {"myFileName000", "myFileName001", "test.txt", "todo.txt", "my_src_file.cpp"};


	typedef boost::tuple < std::vector<std::string>::iterator, std::vector<std::string>::iterator, std::string > relational_tuple_type;


	std::vector<relational_tuple_type> relational_tuples
		{ {alltags.begin(), allfiles.begin(), "Woo"}
		, {alltags.begin()+1, allfiles.begin(), "Hello!"}
		, {alltags.begin()+2, allfiles.begin(),		"4 k"}
		, {alltags.begin()+2, allfiles.begin()+1,	"8 k"}
		, {alltags.begin()+2, allfiles.begin()+2,	"12 k"}
		, {alltags.begin()+2, allfiles.begin()+3,	"16 k"}
		, {alltags.begin()+2, allfiles.begin()+4,	"15 M"}

		, {alltags.begin()+3, allfiles.begin(),		"red"}
		, {alltags.begin()+3, allfiles.begin()+1,	"green"}
		, {alltags.begin()+3, allfiles.begin()+2,	"blue"}
		, {alltags.begin()+3, allfiles.begin()+3,	"yellow"}
		, {alltags.begin()+3, allfiles.begin()+4,	"purple"}
		};



	my_bimap_type my_bimap;


	for (auto elem : relational_tuples)
	{
		my_bimap.insert (
			tag_instance_type( *elem.get<0>(), *elem.get<1>(), elem.get<2>())
		);
	}



	my_relation_type my_relations;

	for (auto elem : relational_tuples)
	{
		my_relations.insert (
			tag_instance_type( *elem.get<0>(), *elem.get<1>(), elem.get<2>())
		);

	}

/*
	my_bimap.insert (
		tag_instance_type( "myTagName000"
						 , "myFileName000"
						 , "Woo"
						 )
	);


	my_bimap.insert (
		tag_instance_type( "myTagName000"
						 , "myFileName001"
						 , "Woo2"
						 )
	);
*/

	std::cout << my_bimap.by<tag_name>().at("myTagName000") << std::endl;

	std::cout << my_bimap.by<tag_name>().count("myTagName000") << std::endl;
	//std::cout << my_bimap.by<tag_name>() << std::endl;

	std::cout << std::endl;

	/*
	std::copy( my_bimap.by<tag_name>().begin()
			 , my_bimap.by<tag_name>().end()
			 , std::ostream_iterator<my_bimap_type::left_value_type>(std::cout, "\n")
			 //, std::ostream_iterator<my_bimap.by<tag_name>()>(std::cout, "\n")
			 );
	*/


	//std::pair<auto first, auto second> tagNameIter = my_bimap.by<tag_name>().equal_range("myTagName000");


	std::cout << "Using my_bimap for find:" << std::endl;

	//const auto tagNameIter = my_bimap.by<tag_name>().find("myTagName000");
	
	for (auto it = my_bimap.by<tag_name>().find("size"); it != my_bimap.by<tag_name>().end(); ++it)
	{
		std::cout << "Tag Name:\t" << it->get<tag_name>() << std::endl
				  << "File Names:\t" << it->get<file_name>() << std::endl
				  << "Tag Value:\t" << it->get<instance_value>() << std::endl;
	}
	
	std::cout << std::endl;


	
	std::cout << "Using my_bimap for equal_range:" << std::endl;

	const auto tagNameIterRange = my_bimap.by<tag_name>().equal_range("myTagName000");
	
	for (auto it = tagNameIterRange.first; it != tagNameIterRange.second; ++it)
	{
		std::cout << "Tag Name:\t" << it->get<tag_name>() << std::endl
				  << "File Names:\t" << it->get<file_name>() << std::endl
				  << "Tag Value:\t" << it->get<instance_value>() << std::endl;
	}
	
	std::cout << std::endl;



	std::cout << "Using my_relations for find:" << std::endl;

	//const auto tagNameIter = my_bimap.by<tag_name>().find("myTagName000");
	
	for (auto it = my_relations.by<tag_name>().find("size"); it != my_relations.by<tag_name>().end(); ++it)
	{
		std::cout << "Tag Name:\t" << it->get<tag_name>() << std::endl
				  << "File Names:\t" << it->get<file_name>() << std::endl
				  << "Tag Value:\t" << it->get<instance_value>() << std::endl;
	}
	
	std::cout << std::endl;




	std::cout << "Using my_relations for equal_range:" << std::endl;

	const auto tagRelationIterRange = my_relations.by<tag_name>().equal_range("myTagName000");
	
	for (auto it = tagRelationIterRange.first; it != tagRelationIterRange.second; ++it)
	{
		std::cout << "Tag Name:\t" << it->get<tag_name>() << std::endl
				  << "File Names:\t" << it->get<file_name>() << std::endl
				  << "Tag Value:\t" << it->get<instance_value>() << std::endl;
	}

	std::cout << std::endl;




	std::cout << "Using my_relations for find:" << std::endl;

	//const auto tagNameIter = my_bimap.by<tag_name>().find("myTagName000");
	
	auto& relations_by_file = my_relations.by<file_name>();

	for (auto it = relations_by_file.find("test.txt"); it != relations_by_file.end(); ++it)
	{
		std::cout << "Tag Name:\t" << it->get<tag_name>() << std::endl
				  << "File Names:\t" << it->get<file_name>() << std::endl
				  << "Tag Value:\t" << it->get<instance_value>() << std::endl;
	}
	
	std::cout << std::endl;


	std::cout << "Using my_relations for find, by file (test.txt):" << std::endl;

	const auto wazzat = my_relations.by<file_name>().equal_range("test.txt");
	for (auto it = wazzat.first; it != wazzat.second; ++it)
	{
		std::cout << "Tag Name:\t" << it->get<tag_name>() << std::endl
				  << "File Names:\t" << it->get<file_name>() << std::endl
				  << "Tag Value:\t" << it->get<instance_value>() << std::endl;

	}
	std::cout << std::endl;


	/*
	my_bimap_type::map_by<tag_name>::const_iterator tagNameIter = my_bimap.by<tag_name>().find("myTagName000");

	if (tagNameIter != my_bimap.by<tag_name>().end())
	{
		std::cout << "Tag Name:\t" << tagNameIter->get<tag_name>() << std::endl
				  << "File Names:\t" << tagNameIter->get<file_name>() << std::endl
				  << "Tag Value:\t" << tagNameIter->get<instance_value>() << std::endl;
	}
	*/


	/*
	for (auto elem : my_bimap)
	{
		std::cout << "Tag Name:\t" << elem.get<tag_name>() << std::endl
				  << "File Names:\t" << elem.get<file_name>() << std::endl
				  << "Tag Value:\t" << elem.get<instance_value>() << std::endl;

	}
	*/
	return 0;
}
