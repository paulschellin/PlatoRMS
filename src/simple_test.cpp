//	g++ -std=c++11 -Wall -I. simple_test.cpp -o simple_test -lpthread -lboost_system -lboost_filesystem

//
//	Standard library #includes<>s
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <algorithm>
#include <functional>
#include <iterator>

#include <vector>
#include <set>
#include <map>

#include <utility>
#include <memory>
#include <exception>



//
//	Boost #include<>s
//

#include <boost/filesystem.hpp>
#include <boost/range.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/weak_ptr.hpp>

#include <boost/container/set.hpp>
#include <boost/container/map.hpp>

//#include <boost/algorithm.hpp>


//
//	Special #include<>s
//


#include <infix_iterator.h>
#include <TagDef.hpp>
#include <PFile.hpp>



using std::cout;
using std::cin;
using std::endl;


using std::string;
using std::ostream;

using std::vector;
//using std::set;
using boost::container::set;
//using std::map;
using boost::container::map;
using std::pair;
using std::make_pair;

//using std::shared_ptr;
//using std::weak_ptr;


using boost::shared_ptr;
using boost::weak_ptr;


namespace fs = boost::filesystem;


typedef string ValueT;

typedef pair< PFile, ValueT >	FileValPairT;
typedef pair< TagDef, ValueT >	TagDefValPairT;


typedef map < TagDef
			, set< FileValPairT >
			> TagMapT;

typedef map < PFile
			, set < TagDefValPairT >
			> PFileMapT;


typedef pair< weak_ptr<PFile>, shared_ptr<ValueT> >	FileValPairSmartT;
typedef pair< weak_ptr<TagDef>, shared_ptr<ValueT> >	TagDefValPairSmartT;



template <typename T>
struct shared_ptr_compare_less {
	typedef bool result_type;
	typedef shared_ptr<T> first_argument_type;
	typedef shared_ptr<T> second_argument_type;


	//shared_ptr_compare_less(void){}
	bool
	operator() (const shared_ptr<T>& lhs, const shared_ptr<T>& rhs)
	{
		return (*lhs < *rhs);
	}
};

template <typename T>
struct weak_ptr_compare_less {
	typedef bool result_type;
	typedef weak_ptr<T> first_argument_type;
	typedef weak_ptr<T> second_argument_type;

	//	Make sure before using the comparison that you've handled
	//	the expired weak_ptrs as you want to, otherwise it'll just
	//	default construct T objects. Nobody wants that.
	bool
	operator() (const weak_ptr<T>& lhs, const weak_ptr<T>& rhs)
	{
		return ( *(lhs.lock()) < *(rhs.lock()) );
	}
};


struct shared_TagDef_by_member_name_less {
	typedef bool result_type;
	typedef shared_ptr<TagDef>  first_argument_type;
	typedef shared_ptr<TagDef>  second_argument_type;
	
	bool
	operator() (const shared_ptr<TagDef>& lhs, const shared_ptr<TagDef>& rhs)
	{
		return (*lhs < *rhs);
	}
};

struct shared_PFile_by_member_name_less {
	typedef bool result_type;
	typedef shared_ptr<PFile>  first_argument_type;
	typedef shared_ptr<PFile>  second_argument_type;

	bool
	operator() (const shared_ptr<PFile>& lhs, const shared_ptr<PFile>& rhs)
	{
		return (*lhs < *rhs);
		//return (lhs->name < rhs->name);
	}
};

template <typename T1, typename T2>
struct pair_first_compare_less {
	typedef bool result_type;
	typedef pair <T1, T2> first_argument_type;
	typedef pair <T1, T2> second_argument_type;

	bool
	operator() (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return (lhs.first < rhs.first);
	}
};

template <typename T1, typename T2>
struct pair_second_compare_less {
	typedef bool result_type;
	typedef pair <T1, T2> first_argument_type;
	typedef pair <T1, T2> second_argument_type;

	bool
	operator() (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return (lhs.second < rhs.second);
	}
};






typedef map < shared_ptr<TagDef>
			, set< FileValPairSmartT >
			//, shared_TagDef_by_member_name_less
			//, pair_first_compare_less< sha
			> TagMapSmartT;

typedef map < shared_ptr<PFile>
			, set< TagDefValPairSmartT >
			//, shared_PFile_by_member_name_less
			> PFileMapSmartT;



//
//	Extraction operator helpers
//

template <typename T>
std::ostream&
operator<< (std::ostream& os, const shared_ptr<T>& obj)
{
	os << *obj;
	return os;
}


template <typename T>
std::ostream&
operator<< (std::ostream& os, const weak_ptr<T>& obj)
{
	os << *(obj.lock());
	return os;
}


template <typename T, typename C = std::less<T>, typename A = std::allocator<T> >
std::ostream&
operator<< (std::ostream& os, const set<T, C, A>& obj)
{
	os << "set: {";
	//boost::copy(obj, std::ostream_iterator<T>(os, ", "));
	boost::copy(obj, infix_ostream_iterator<T>(os, ", "));
	os << "}";
	return os;
}

template < typename Container
		 , typename charT = char
		 //, typename traits = std::char_traits<charT>
		 >
struct printer {
	typename Container::const_iterator first;
	typename Container::const_iterator last;
	charT const* delimiter;

	printer (const Container& con, charT const *sep = ", ")
		: first(con.cbegin())
		, last(con.cend())
		, delimiter(sep)
	{
		
	}

	friend
	std::ostream&
	operator<< (std::ostream& os, const printer& obj)
	{
		//std::copy(obj.first, obj.last, std::ostream_iterator<typename Container::value_type>(os, separator_str)); 
		std::copy(obj.first, obj.last, infix_ostream_iterator<typename Container::value_type>(os, obj.delimiter));
		return os;
	}
};


template <typename T1, typename T2>
std::ostream&
operator<< (std::ostream& os, const std::pair<T1, T2>& obj)
{
	os << "pair: {" << std::get<0>(obj) << ", " << std::get<1>(obj) << "}";
	//os << "pair< " << obj.first << ", " << obj.second << " >";
	return os;
}


std::vector< PFile >
make_recursive_file_container (fs::path p)
{
	std::vector< PFile > result;
	for (auto it : recursive_directory_range(p))
	{
		auto it_path = fs::path(it);
		try {
			if (fs::exists(it_path)) {
				if (fs::is_regular_file(it_path)) {
					auto absCanonical = fs::canonical(fs::absolute(it_path));
					result.push_back(PFile(absCanonical.filename().string(), absCanonical.string(), absCanonical));
				}
			}
		}
		catch (const fs::filesystem_error& ex) {
			std::cout << ex.what() << std::endl;
		}
	}

	return result;
}



void
tag_with_path_elements (




void test_map_filling(void)
{
	TagMapSmartT myTagMap;
	PFileMapSmartT myFileMap;
	
	for (auto& elem : ExampleTags::NormalTags)
	{
		myTagMap.emplace(boost::make_shared<TagDef>(elem), set< FileValPairSmartT >());
	}
	
	for (auto& elem : ExampleFiles::AssortedFiles)
	{
		myFileMap.emplace(boost::make_shared<PFile>(elem), set< TagDefValPairSmartT >());
	}


	for (auto it = myTagMap.find()

	
	cout << endl << "TagMap:" << endl;
	boost::copy(myTagMap, std::ostream_iterator<decltype(myTagMap)::value_type>(cout, "\n"));

	cout << endl << "FileMap:" << endl;
	boost::copy(myFileMap, std::ostream_iterator<decltype(myFileMap)::value_type>(cout, "\n"));
}



int
main (int argc, char* argv[])
{
	
	test_map_filling();
	
	/*
	auto myTd = TagDef ("size", "Bytes", "Size of the file");

	

	//TagMapT myTM;

	map<TagDef, int> myTM;


	//TagMapSmartT firstInst;


	shared_ptr<TagDef> jimmy (new TagDef(myTd));

	shared_ptr<ValueT> jimmys_val (new ValueT("Some god awful value"));

	TagDefValPairSmartT paired_jimmy = make_pair(weak_ptr<TagDef>(jimmy), jimmys_val);


	map<shared_ptr<TagDef>, int> woo;


	woo.emplace(jimmy, 47);

	boost::copy(woo, std::ostream_iterator<decltype(woo)::value_type>(cout, "\n"));


	weak_ptr<TagDef> weak_jimmy (jimmy);

	cout << jimmy->name << endl;


	cout << weak_jimmy.lock()->name << endl;


	cout << paired_jimmy << endl;

	int c = 0;

	for (auto& elem : ExampleTags::NormalTags)
	{
		myTM.insert(make_pair(elem, c++));
	}

	cout << myTd;
	
	
	for (auto item : myTM)
	{
		cout << item.first << "\t" << item.second << endl;
	}


	shared_ptr<PFile> jimmys_file (new PFile("jimmy's file.txt", "/home/jimmy/jimmy's file.txt"));

	shared_ptr<ValueT> that_file_value(new ValueT("407 GiB"));

	FileValPairSmartT paired_value = make_pair(weak_ptr<PFile>(jimmys_file), that_file_value);



	//myTagMap.emplace(jimmy, paired_value);
	//myTagMap.emplace(jimmy, set <FileValPairSmartT>());
	
	myTagMap[jimmy];

	cout << "Size of myTagMap[jimmy]: " << myTagMap[jimmy].size() << endl;

	myTagMap[jimmy].emplace(paired_value);

	cout << "Size of myTagMap[jimmy]: " << myTagMap[jimmy].size() << endl;
	
	//cout << endl << endl << "The real deal:" << endl;
	//boost::copy(myTagMap, std::ostream_iterator<decltype(myTagMap)::value_type>(cout, "\n"));


	//boost::copy(ExampleFiles::AssortedFiles, std::ostream_iterator<decltype(ExampleFiles::AssortedFiles)::value_type>(cout, "\n"));

	fs::path p ("../");

	vector<PFile> files (make_recursive_file_container(p));

	//boost::copy(files, std::ostream_iterator<PFile>(cout, "\n"));

	cout << printer<vector<PFile>>(ExampleFiles::AssortedFiles, ", ") << endl;


	
	*/
	return 0;
}
