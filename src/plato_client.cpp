// Compile with:	
// g++ -std=c++11 -Wall -I src/ src/plato_client.cpp -o bin/plato_client

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>

#include <vector>
#include <string>
#include <utility>

#include <iostream>

#include <cstdlib>	//	for std::exit()


#include <prototype_structs.hpp>

//#include <tclap/CmdLine.h>

using namespace boost::interprocess;

/*
//Typedefs of allocators and containers
typedef managed_shared_memory::segment_manager                       segment_manager_t;
typedef allocator<void, segment_manager_t>                           void_allocator;
typedef allocator<int, segment_manager_t>                            int_allocator;
typedef vector<int, int_allocator>                                   int_vector;
typedef allocator<int_vector, segment_manager_t>                     int_vector_allocator;
typedef vector<int_vector, int_vector_allocator>                     int_vector_vector;
typedef allocator<char, segment_manager_t>                           char_allocator;
typedef basic_string<char, std::char_traits<char>, char_allocator>   char_string;
*/

/*
template <typename T>
std::ostream&
operator<<(std::ostream& os, const std::pair<T, T>& inPair)
{

	os << "{ " << inPair.first << " : " << inPair.second << " }";

	return os;
}
*/
/*
//Definition of the map holding a string as key and complex_data as mapped type
typedef std::pair<const char_string, char_string>                      map_value_type;
typedef std::pair<char_string, char_string>                            movable_to_map_value_type;
typedef allocator<map_value_type, segment_manager_t>                    map_value_type_allocator;
typedef map< char_string, char_string
           , std::less<char_string>, map_value_type_allocator>          shared_map_type;
*/




int main (int argc, char* argv[])
{

/*
	try {

	TCLAP::CmdLine cmd("Plato userspace utility", ' ', "0.01");

	std::vector<std::string> cmdAllowed = {"list", "add", "remove", "create", "new", "delete", "duplicate", "merge", "help"};
	TCLAP::ValuesConstraint<std::string> cmdAllowedVals (cmdAllowed);

	TCLAP::UnlabeledValueArg<std::string> commandArg ("command", "Pass All the commands!", true, "help", &cmdAllowedVals, cmd );
	

	std::vector<std::string> thingsAllowed = {"tag", "tags", "file", "files", "rnode", "rnodes"};
	TCLAP::ValuesConstraint<std::string> thingsAllowedVals (thingsAllowed);

	//TCLAP::UnlabeledValueArg<std::string> typeOfThingArg ("type of thing", "Pass All the things!", false, "help", &thingsAllowedVals, cmd );


	cmd.parse(argc, argv);

	std::string cmdArg = commandArg.getValue();
//	std::string typeArg = typeOfThingArg.getValue();


	std::cout << cmdArg << std::endl;
	//std::cout << typeArg << std::endl;

	try {
		managed_shared_memory segment(open_only, "PlatoDaemonSharedMemory");
	} catch (const interprocess_exception& e) {
		std::cerr << "An exception occurred while attempting to connect to the shared memory segment. The exception is: " << e.what() << std::endl;
	} catch (...) {
		std::cerr << "An unknown/unexpected exception occurred while attempting to connect to the shared memory segment." << std::endl;
	}

	
	ListTagDefT* tagArray = segment.find<ListTagDefT>("TagDefArray").first;

	ListRNodeT* rnodeArray = segment.find<ListRNodeT>("RNodeArray").first;

	ListTagValT* tagValArray = segment.find<ListTagValT>("TagValArray").first;

	ListRNodeValPairT* rnodeValPairArray = segment.find<ListRNodeValPairT>("RNodeValPairArray").first;
	
	ListTagDefValPairT* tagDefValPairArray= segment.find<ListTagDefValPairT>("TagDefValPairArray").first;




	//std::cout << "Shm region size is: " << segment.get_size() << std::endl;

	
	 	.find
		.find_no_lock
	 

	//shared_map_type* mymap = segment.find<shared_map_type>("MyMap").first;


	
	//std::copy( mymap->begin(), mymap->end(), std::ostream_iterator<ValueType>(std::cout, "\n"));

	//for (auto kvPair : *mymap)
	
	for (auto iter = mymap->begin(); iter != mymap->end(); ++iter)
	{
		std::cout << "{ " << std::string(iter->first.begin(), iter->first.end())
				<< " : " << std::string(iter->second.begin(), iter->second.end())
				<< " }" << std::endl;

	}

	std::cout.flush();
	

	} catch (TCLAP::ArgException &e)	// catch all the exceptions!
	{
		std::cout << "error: " << e.error() << " for arg " << e.argId() << std::endl;
	}
	*/

	std::shared_ptr<managed_shared_memory> segment;

	try {
		segment.reset(new managed_shared_memory(open_only, "PlatoDaemonSharedMemory"));
	} catch (const interprocess_exception& e) {
		std::cerr << "An exception occurred while attempting to connect to the shared memory segment. The exception is: " << e.what() << std::endl;
	} catch (...) {
		std::cerr << "An unknown/unexpected exception occurred while attempting to connect to the shared memory segment." << std::endl;
	}

	
	PlatoDB pdb (*segment);
	//PlatoDB pdb (*segment, alloc_inst);


	pdb.print_basic_diagnostics (std::cout);

	
	return 0;
}

