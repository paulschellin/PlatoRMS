

//////////////////////////////////////////////////////////////////////////////
					Boost Interprocess Library Headers
//////////////////////////////////////////////////////////////////////////////

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>


//////////////////////////////////////////////////////////////////////////////
					Boost Logging Library Headers
//////////////////////////////////////////////////////////////////////////////

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

namespace bl = boost::log;


//////////////////////////////////////////////////////////////////////////////
					Boost Program Option Library Headers
//////////////////////////////////////////////////////////////////////////////

#include <boost/program_options.hpp>	//	Run-time options library (NOT header-only!)

namespace po = boost::program_options;


//////////////////////////////////////////////////////////////////////////////
					Standard Library Headers
//////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <string>
#include <utility>
#include <cstdlib>	//	for std::exit()


//////////////////////////////////////////////////////////////////////////////
					-----
//////////////////////////////////////////////////////////////////////////////



using namespace boost::interprocess;

//Typedefs of allocators and containers
typedef managed_shared_memory::segment_manager                       segment_manager_t;
typedef allocator<void, segment_manager_t>                           void_allocator;
typedef allocator<int, segment_manager_t>                            int_allocator;
typedef vector<int, int_allocator>                                   int_vector;
typedef allocator<int_vector, segment_manager_t>                     int_vector_allocator;
typedef vector<int_vector, int_vector_allocator>                     int_vector_vector;
typedef allocator<char, segment_manager_t>                           char_allocator;
typedef basic_string<char, std::char_traits<char>, char_allocator>   char_string;


//Definition of the map holding a string as key and char_string as mapped type
typedef std::pair<const char_string, char_string>                      map_value_type;
typedef std::pair<char_string, char_string>                            movable_to_map_value_type;
typedef allocator<map_value_type, segment_manager_t>                    map_value_type_allocator;
typedef map< char_string, char_string
           , std::less<char_string>, map_value_type_allocator>          shared_map_type;


enum SubCommand {
	cmdHelp, cmdStart, cmdStop, cmdRestart, cmdStatus
};

SubCommand
arg_handler (int argc, char* argv[])
{
	std::string cmdStr = "help";
	if (argc > 1) {
		cmdStr = argv[1];
	}
	
	if (cmdStr.compare("start") == 0)	return cmdStart;
	if (cmdStr.compare("restart") == 0)	return cmdRestart;
	if (cmdStr.compare("stop") == 0)	return cmdStop;
	if (cmdStr.compare("status") == 0)	return cmdStatus;

	std::cout << "Usage: " << argv[0] 
		<< " [ \"start\" | \"stop\" | \"restart\" | \"status\" | \"help\" ]" << std::endl;
	std::exit(1);

	return cmdHelp;
}

int main (int argc, char* argv[])
{

	SubCommand subcmd = arg_handler (argc, argv);	

	std::vector< std::pair< const std::string, const std::string > > spv = {
		{"FileMap", "hpp"}
		, {"FileMap", "hpp"}
		, {"ExampleTagDefs", "hpp"}
		, {"PFile", "hpp"}
		, {"PlatoDB", "hpp"}
		, {"PlatoDB_RuntimeConfig", "hpp"}
		, {"RNode", "hpp"}
		, {"SimpleDB", "hpp"}
		, {"TagDef", "hpp"}
		, {"TagMap", "hpp"}
		, {"old_RNode", "hpp"}
		, {"dynamic_type_system_test", "cpp"}
		, {"exterminator_ref", "cpp"}
		, {"first_attempt", "cpp"}
		, {"perf_db_test", "cpp"}
		, {"plato_client", "cpp"}
		, {"plato_daemon", "cpp"}
		};


	const unsigned shm_region_size = 1048576;
	const std::string shm_region_name ("PlatoDaemonSharedMemory");


   //Remove shared memory on construction and destruction
   /*
   struct shm_remove
   {
      shm_remove() { shared_memory_object::remove("MySharedMemory"); }
      ~shm_remove(){ shared_memory_object::remove("MySharedMemory"); }
   } remover;
	*/

	shared_memory_object::remove(shm_region_name.c_str());

   //Create shared memory
   managed_shared_memory segment(create_only,shm_region_name.c_str(), shm_region_size);// shm_region_size);

   //An allocator convertible to any allocator<T, segment_manager_t> type
   void_allocator alloc_inst (segment.get_segment_manager());

   //Construct the shared memory map and fill it
   shared_map_type *mymap = segment.construct<shared_map_type>
      //(object name), (first ctor parameter, second ctor parameter)
         ("MyMap")(std::less<char_string>(), alloc_inst);

   for(std::size_t i = 0; i < spv.size(); ++i){
      //Both key(string) and value(complex_data) need an allocator in their constructors
      auto spvL = spv.at(i).first;
      auto spvR = spv.at(i).second;
	  
	  char_string  key_object(spvL.begin(), spvL.end(),alloc_inst);
	  //char_string  key_object(spv.at(i).first.begin(), spv.at(i).first.end(),alloc_inst);
      //complex_data mapped_object(i, "default_name", alloc_inst);
      char_string mapped_object(spvR.begin(), spvR.end(),alloc_inst);
	  map_value_type value(key_object, mapped_object);
      //Modify values and insert them in the map

	//	value.first = stringPairVec.at(i).first;
	//	value.second = stringPairVec.at(i).second;
		//mymap[stringPairVec.at(i).first] = stringPairVec.at(i).second;
      mymap->insert(value);
   }
   return 0;
}
