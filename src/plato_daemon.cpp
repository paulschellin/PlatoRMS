

//////////////////////////////////////////////////////////////////////////////
//					Boost Interprocess Library Headers
//////////////////////////////////////////////////////////////////////////////

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/file_mapping.hpp>

//////////////////////////////////////////////////////////////////////////////
//					Boost Logging Library Headers
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
//					Boost Program Option Library Headers
//////////////////////////////////////////////////////////////////////////////

#include <boost/version.hpp>			//	For BOOST_LIB_VERSION
#include <boost/program_options.hpp>	//	Run-time options library (NOT header-only!)

namespace po = boost::program_options;


//////////////////////////////////////////////////////////////////////////////
//					Standard Library Headers
//////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <string>
#include <utility>
#include <cstdlib>	//	for std::exit()
#include <exception>

//////////////////////////////////////////////////////////////////////////////
//					-----
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


int rc_handler (std::ostream& os, po::variables_map& vm, int ac, char* av[])
{
	try {
		po::options_description genOpts;
		genOpts.add_options()
			("help,h", po::value<std::string>()->implicit_value("help"), "")
			//("command", po::value<std::string>(), "")
			("start", "Start the Plato daemon. Will fail if already running.")
			("stop", "Stop the Plato daemon. Will fail if not previously running.")
			("restart", "Stop the Plato daemon, then start it up again. Will fail if not previously running.")
			("status", "View information about the Plato daemon, such as if it is running, how much of its reserved memory it is using, and other things.")
			//("verbose,v", "")
			("version,V", "Print the version information.")
			("input-file,i", "Specify the input file for serialization.")
			("output-file,o", "Specify the output file for serialization.")
			("log-file,l", "Specify the log file.")
			("config-file", "Specify the runtime configuration file.")
		;

		po::options_description configOpts("Options to be specified in a configuration file");

		configOpts.add_options()
			("shared-memory-name", po::value<std::string>()->default_value("PlatoDaemonSharedMemory"), "")
			("tag-map-name", po::value<std::string>()->default_value("PlatoTagMap"), "")
			("file-map-name", po::value<std::string>()->default_value("PlatoFileMap"), "")
			("start-up-buffer", po::value<float>()->default_value(1.0), "Pick the amount of headroom you want to have at startup.")
		;

		po::options_description cmdlineOpts;

		cmdlineOpts.add(genOpts).add(configOpts);

		po::options_description configFileOpts;
		//configFileOpts.add(configOpts);
		
		po::options_description visibleOpts("Supported command-line options");
		visibleOpts.add(genOpts);

		//	Positional options
		//po::positional_options_description posOpts;
		//posOpts.add("command", 1);

		po::store(po::command_line_parser(ac, av).options(cmdlineOpts).run(), vm);

		po::notify(vm);

		if (vm.count("help")) {
			if (!vm["help"].as<std::string>().compare("help")) {
			os << visibleOpts << std::endl;
			os << "For additional options regarding config-files, run with \"--help config-files\"" << std::endl;
			} else if ( ! vm["help"].as<std::string>().compare("config-files")) {
				os << configOpts << std::endl;
			}
			exit(0);
		}

		if (vm.count("version")) {
			os << "plato_daemon"
				<< std::endl << "The Plato Semantic Resource Management System Daemon" << std::endl;
			os << "Version "
			#ifdef PLATO_DAEMON_VERSION_ID
				<< PLATO_DAEMON_VERSION_ID;
			#else
				<< "<unspecified> (set preprocessor macro \"PLATO_DAEMON_VERSION_ID\" to specify version string)";
			#endif
			os << std::endl
				<< "Compiled on " << __DATE__ << " at " << __TIME__ << std::endl
				<< "Compiler version: " << __VERSION__ << std::endl
				<< "Boost library version: " << BOOST_LIB_VERSION << std::endl
				<< "Git commit: " 
				#ifdef PLATO_DAEMON_GIT_COMMIT
					<< PLATO_DAEMON_GIT_COMMIT 
				#else
					<< "<unspecified> (set preprocessor macro \"PLATO_DAEMON_GIT_COMMIT\" to specify version string)"
				#endif
				<< std::endl;
			exit(0);
		}

		std::ifstream ifs (vm["config-file"].as<std::string>().c_str());

		po::store(po::parse_config_file(ifs, configFileOpts), vm);

	} catch (std::exception& e) {std::cerr << "error: " << e.what() << std::endl; std::exit(1);
	} catch (...) {std::cerr << "Exception of unknown type occurred." << std::endl; std::exit(1);}
	
	return 0;
}

/*
init_logging()
{
	bl::add_console_log()->set_filter();

	bl::add_file_log
	(
		bl::keywords::file_name = "",
		bl::keywords::format = "[%TimeStamp%]: %Message%"
	);

	bl::core::get()->set_filter( bl::trivial::severity >= bl::trivial::info );
};
*/


int command_handler (std::ostream& os, po::variables_map& vm)
{
	//auto cmdStr = vm["command"].as<std::string>();

	if (vm.count("start")) {


	} else
	if (vm.count("stop")) {
		shared_memory_object::remove(vm["shared-memory-name"].as<std::string>().c_str());
		std::exit(0);
	} else
	if (vm.count("restart")) {
		shared_memory_object::remove(vm["shared-memory-name"].as<std::string>().c_str());
	} else
	if (vm.count("status")) {

		std::exit(0);
	}

	return 0;
}


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


int main (int argc, char* argv[])
{
	//	Take care of the runtime configuration settings.
	po::variables_map vm;
	rc_handler(std::cout, vm, argc, argv);


	/*
					How do we want to serialize and deserialize?

			Should we read in the file first, then move it into shared memory?
				That would probably take a lot of time and waste a lot of memory.
			Should we read each object in element-by-element directly into the shared memory?
				If we really are not able to change the memory dynamically, then we probably want to know how big all of the objects will be.


	 */
	
	
	command_handler(std::cout, vm);


	unsigned shm_region_size_bytes = 1048576; // estimate_space_requirements();
	const std::string shm_region_name (vm["shared-memory-name"].as<std::string>().c_str()); //("PlatoDaemonSharedMemory");


   //Remove shared memory on construction and destruction
   /*
   struct shm_remove
   {
      shm_remove() { shared_memory_object::remove("MySharedMemory"); }
      ~shm_remove(){ shared_memory_object::remove("MySharedMemory"); }
   } remover;
	*/

	//shared_memory_object::remove(shm_region_name.c_str());

   //Create shared memory
   managed_shared_memory segment(create_only,shm_region_name.c_str(), shm_region_size_bytes);// shm_region_size);

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
