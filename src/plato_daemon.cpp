

//////////////////////////////////////////////////////////////////////////////
//					Boost Interprocess Library Headers
//////////////////////////////////////////////////////////////////////////////

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/deque.hpp>
#include <boost/interprocess/containers/list.hpp>
#include <boost/interprocess/containers/slist.hpp>

#include <boost/interprocess/containers/set.hpp>
#include <boost/interprocess/containers/flat_set.hpp>
#include <boost/interprocess/containers/flat_map.hpp>


#include <boost/interprocess/smart_ptr/shared_ptr.hpp>
#include <boost/interprocess/smart_ptr/deleter.hpp>


#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>

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
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <cstdlib>	//	for std::exit()
#include <exception>
#include <memory>	//	for shared_ptr()


//////////////////////////////////////////////////////////////////////////////
//					Try another logging framework
//////////////////////////////////////////////////////////////////////////////

#include <easylogging++.h>

_INITIALIZE_EASYLOGGINGPP

//////////////////////////////////////////////////////////////////////////////
//					-----
//////////////////////////////////////////////////////////////////////////////

#include <plato_shared_memory.hpp>
#include <prototype_structs.hpp>
#include <plato_common_functions.hpp>


using namespace boost::interprocess;



//////////////////////////////////////////////////////////////////////////////
//						Runtime Configuration Handler
//////////////////////////////////////////////////////////////////////////////

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
			("mode,m", po::value<std::string>()->default_value("mmap"), "Specify either 'mmap' (memory mapped file), or 'shm' (shared memory).")
			("input-file,i",	po::value<std::string>()->default_value("plato_daemon_datafile"),	"Specify the input file for serialization.")
			("output-file,o",	po::value<std::string>()->default_value("plato_daemon_datafile"),	"Specify the output file for serialization.")
			("log-file,l", 		po::value<std::string>()->default_value("plato_daemon.log"),	"Specify the log file.")
			("config-file",		po::value<std::string>()->default_value("plato_daemon.conf"),	"Specify the runtime configuration file.")
		;

		po::options_description configOpts("Options to be specified in a configuration file");

		configOpts.add_options()
			("memory-mapped-filename", po::value<std::string>()->default_value("PlatoDaemonFile.mmap"), "")
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



//////////////////////////////////////////////////////////////////////////////
//						Initialize Logging
//////////////////////////////////////////////////////////////////////////////
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

void
logging_configuration()
{
	el::Configurations conf("plato_easylogging++.conf");

	el::Loggers::reconfigureLogger("default", conf);
	//el::Loggers::reconfigureAllLoggers(conf);
}


//////////////////////////////////////////////////////////////////////////////
//						Run the Requested Command
//////////////////////////////////////////////////////////////////////////////

int command_handler (std::ostream& os, po::variables_map& vm)
{
	//auto cmdStr = vm["command"].as<std::string>();

	if (vm.count("start")) {


	} else
	if (vm.count("stop")) {
		shared_memory_object::remove(vm["shared-memory-name"].as<std::string>().c_str());

		os << "Stopped the daemon." << std::endl;
		std::exit(0);
	} else
	if (vm.count("restart")) {
		shared_memory_object::remove(vm["shared-memory-name"].as<std::string>().c_str());
		os << "Temporarily stopped the daemon." << std::endl;
	} else
	if (vm.count("status")) {
		std::exit(0);
	}

	return 0;
}



//////////////////////////////////////////////////////////////////////////////
//					-----
//////////////////////////////////////////////////////////////////////////////

/*
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
*/


//////////////////////////////////////////////////////////////////////////////
//						Main Function
//////////////////////////////////////////////////////////////////////////////

int main (int argc, char* argv[])
{
	//	Take care of the runtime configuration settings.
	po::variables_map vm;
	rc_handler(std::cout, vm, argc, argv);

	//_START_EASYLOGGINGPP(argc, argv);
	// Global Trace Debug Fatal Error Warning Info Verbose Unknown

	/*
					How do we want to serialize and deserialize?

			Should we read in the file first, then move it into shared memory?
				That would probably take a lot of time and waste a lot of memory.
			Should we read each object in element-by-element directly into the shared memory?
				If we really are not able to change the memory dynamically, then we probably want to know how big all of the objects will be.


	 */
	
	logging_configuration();
	
	LOG(INFO) << "Started " << argv[0] << ".";

	command_handler(std::cout, vm);


	unsigned shm_region_size_bytes = 1048576; // estimate_space_requirements();
	const std::string shm_region_name (vm["shared-memory-name"].as<std::string>().c_str());
	const std::string mmapped_filename(vm["memory-mapped-filename"].as<std::string>().c_str());

   //Remove shared memory on construction and destruction
   /*
   struct shm_remove
   {
      shm_remove() { shared_memory_object::remove("MySharedMemory"); }
      ~shm_remove(){ shared_memory_object::remove("MySharedMemory"); }
   } remover;
	*/

	//shared_memory_object::remove(shm_region_name.c_str());

	
	///////
	///////
	///////

	/*

	if (vm["mode"].as<std::string>().compare("mmap") == 0)
	{
		//	Remove the file mapping
		// file_mapping::remove(vm["memory-mapped-filename"].as<std::string>().c_str());

		//	Create a file buffer
		std::filebuf fbuf;

		try {
			fbuf.open(mmapped_filename.c_str()
				, std::ios_base::in | std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
		}
		catch (const interprocess_exception& e)
		{
			std::cout << "An exception occurred while trying to open \""
				<< mmapped_filename << "\" for the filebuf. The exception is: "
				<< e.what() << std::endl;
		}
		catch (...)
		{
			std::cout << "An unknown/unexpected exception occurred while trying to open \""
				<< mmapped_filename << "\" for the filebuf." << std::endl;
		}



		//	Set the filebuf size
		//	seekoff sets a new position for internal position points...
		
		try {
			fbuf.pubseekoff(shm_region_size_bytes - 1, std::ios_base::beg);
		}
		catch (const interprocess_exception& e)
		{
			std::cout << "An exception occurred while trying to pubseekoff the filebuffer for \""
				<< mmapped_filename << "\". The exception is: "
				<< e.what() << std::endl;
		}
		catch (...)
		{
			std::cout << "An unknown/unexpected exception occurred while trying to pubseekoff the filebuffer for \""
				<< mmapped_filename << "\"." << std::endl;
		}

		try {
			fbuf.sputc(0);	//	Store character at current put position and increase put pointer
		}
		catch (const interprocess_exception& e)
		{
			std::cout << "An exception occurred while trying to sputc the filebuffer for \""
				<< mmapped_filename << "\". The exception is: "
				<< e.what() << std::endl;
		}
		catch (...)
		{
			std::cout << "An unknown/unexpected exception occurred while trying to sputc the filebuffer for \"."
				<< mmapped_filename << "\"" << std::endl;
		}




		//	Create the file mapping
		std::shared_ptr<file_mapping> m_file;
		try{
			//file_mapping m_file(mmapped_filename.c_str(), read_write);
			m_file.reset(new file_mapping(mmapped_filename.c_str(), read_write));
		}
		catch (const interprocess_exception& e)
		{
			std::cout << "An exception occurred while trying to create the file mapping for \"" << mmapped_filename << "\". The exception is: "
			<< e.what() << std::endl;
		}
		catch (...)
		{
			std::cout << "An unknown/unexpected exception occurred while trying to create the file mapping for \"" << mmapped_filename << "\"." << std::endl;
		}


		//	Map the whole file with read-write permissions in this process
		std::shared_ptr<mapped_region> region;
		try{
			region.reset(new mapped_region(*m_file, read_write));
			//mapped_region region(*m_file, read_write);
		}
		catch (const interprocess_exception& e)
		{
			std::cout << "An exception occurred while trying to create the mmapped region for \"" << mmapped_filename << "\". The exception is: "
			<< e.what() << std::endl;
		}
		catch (...)
		{
			std::cout << "An unknown/unexpected exception occurred while trying to create the mmapped region for \"" << mmapped_filename << "\"." << std::endl;
		}


		//	Get the address of the mapped region
		void*		addr	= region->get_address();
		std::size_t size	= region->get_size();

		std::memset(addr, 1, size);
	}
	
	*/

	///////
	///////
	///////



	//	Create shared memory region
	//std::shared_ptr<managed_shared_memory> segment;
	
	GenericSharedMemory<managed_mapped_file> sh_memory (mmapped_filename.c_str());
	
	/*
	std::shared_ptr<managed_mapped_file> segment;
	
	try {



   		//managed_shared_memory
		//segment.reset(new managed_shared_memory(create_only,shm_region_name.c_str(), shm_region_size_bytes)
//);
		segment.reset(new managed_mapped_file(open_or_create, mmapped_filename.c_str(), shm_region_size_bytes));
	}
	catch (interprocess_exception& e)
	{
		std::cout << "Did not start the daemon, the exception is: " << e.what() << std::endl; std::exit(1);
	}
	catch (...)
	{
		std::cout << "An unknown/unexpected exception occurred. The daemon did not start up. Probably."
					<< std::endl;
		std::exit(1);
	}
	*/
	LOG(INFO) << "Created the shared memory segment \"" << shm_region_name << "\".";




   //An allocator convertible to any allocator<T, segment_manager_t> type
	//void_allocator alloc_inst (segment->get_segment_manager());


	//void_allocator m_alloc_inst (m_segment->get_segment_manager());

	
	//print_basic_shm_diagnostics(std::cout, *segment);


	//print_basic_shm_diagnostics(std::cout, *m_segment);

	/*
	//	Construct the tag map in shared memory
	shared_map_type* tagMap = segment->construct<shared_map_type>(
											vm["tag-map-name"].as<std::string>().c_str())
												(std::less<char_string>(), alloc_inst);

	//	Construct the file map in shared memory
	shared_map_type* fileMap = segment->construct<shared_map_type>(
											vm["file-map-name"].as<std::string>().c_str())
												(std::less<char_string>(), alloc_inst);
	*/

	//	Alright, let's get all of the data structures squared away in shared-space...

/*	
	//	TagDef Array container
	typedef TagDef<char_string> TagDefT;

	typedef allocator<TagDefT, segment_manager_t> TagDefTAlloc;

	typedef list<TagDefT, TagDefTAlloc> ListTagDefT;

	ListTagDefT* tagArray = segment->construct<ListTagDefT>("TagDefArray")(alloc_inst);

	

	//	RNode Array container
	typedef allocator<RNode, segment_manager_t> RNodeAlloc;

	typedef list<RNode, RNodeAlloc> ListRNodeT;

	ListRNodeT* rnodeArray = segment->construct<ListRNodeT>("RNodeArray")(alloc_inst);

	
	
	//	TagVal Array container
	typedef TagVal<char_string> TagValT;

	typedef allocator<TagValT, segment_manager_t> TagValTAlloc;

	typedef list<TagValT, TagValTAlloc> ListTagValT;

	ListTagValT* tagValArray = segment->construct<ListTagValT>("TagValArray")(alloc_inst);

	

	//	RNode Set container
	typedef std::pair<offset_ptr<RNode>, offset_ptr<TagValT> > RNodeValPairT;

	typedef std::pair<offset_ptr<TagDefT>, offset_ptr<TagValT> > TagDefValPairT;

	
	typedef allocator<RNodeValPairT, segment_manager_t> RNodeValPairTAlloc;
	
	typedef allocator<TagDefValPairT, segment_manager_t> TagDefValPairTAlloc;

	typedef list<RNodeValPairT, RNodeValPairTAlloc> ListRNodeValPairT;
	
	typedef list<TagDefValPairT, TagDefValPairTAlloc> ListTagDefValPairT;
	

	ListRNodeValPairT* rnodeValPairArray = segment->construct<ListRNodeValPairT>("RNodeValPairArray")(alloc_inst);
	ListTagDefValPairT* tagDefValPairArray= segment->construct<ListTagDefValPairT>("TagDefValPairArray")(alloc_inst);
*/

	//PlatoDB pdb (*segment, alloc_inst);

	PlatoDB pdb (*sh_memory.segment);

	LOG(INFO) << "Created PlatoDB object.";

	LOG(INFO) << "Started the daemon.";

//	print_basic_shm_diagnostics(std::cout, *segment);
	
	//using namespace std::string_literals;
	//using namespace std::literals::string_literals;
	auto filename_tag = pdb.create_tag("filename"_s, "string"_s, "the name of the file"_s);

	auto fs_tag = pdb.create_tag("filesystem"_s, "string"_s, "the filesystem of the actual file"_s);

	auto path_tag = pdb.create_tag("path"_s, "string"_s, "the full path of the file"_s);

	auto file_size_tag = pdb.create_tag("size"_s, "int"_s, "the size of the file in bytes"_s);

	auto tag_we_want_to_delete = pdb.create_tag("test of deletion"_s, "string"_s, "this should be deleted by the beginning of the test"_s);

	auto tag_we_want_to_delete_later = pdb.create_tag("test of deletion 2"_s, "string"_s, "this should be deleted by the end of the test"_s);

	pdb.print_basic_diagnostics(std::cout);

//	pdb.delete_tag(tag_we_want_to_delete);

	pdb.print_basic_diagnostics(std::cout);

//	print_basic_shm_diagnostics(std::cout, *segment);

	std::cout << "Size of tag list: " << pdb.count_tags() << std::endl;

	auto new_file = pdb.create_rnode();

	
//	print_basic_shm_diagnostics(std::cout, *segment);
	
	std::cout << "Size of rnode list: " << pdb.count_rnodes() << std::endl;


	auto found_tag = pdb.get_tag("size"_s);

	std::cout << "Tried search on tag, was it correct? " << (file_size_tag == found_tag) << std::endl;


	std::cout << "Found tag: (next line)" << std::endl << *found_tag << std::endl;


	std::cout << "Print all tags:" << std::endl;

	std::copy(pdb.tags_begin(), pdb.tags_end(), std::ostream_iterator<TagDefT>(std::cout, "\n"));

	pdb.print_basic_diagnostics(std::cout);

	pdb.add_tag_to_rnode (filename_tag, new_file, "test.txt"_s);
	pdb.add_tag_to_rnode (fs_tag, new_file, "ext4 \"/\""_s);
	pdb.add_tag_to_rnode (path_tag, new_file, "/home/paulschellin/"_s);
	pdb.add_tag_to_rnode (file_size_tag, new_file, "4096"_s);
	pdb.add_tag_to_rnode (tag_we_want_to_delete_later, new_file, "this should be deleted by the end!"_s);

	
	//print_basic_shm_diagnostics(std::cout, *segment);
	//auto ts_begin = pdb.rnode_tag_set_begin(new_file);
	//auto ts_end = pdb.rnode_tag_set_end(new_file);

	pdb.print_basic_diagnostics(std::cout);

	std::cout << "Print all tag pairs for the file:" << std::endl;
	std::copy(pdb.rnode_tag_set_begin(new_file), pdb.rnode_tag_set_end(new_file)
			, std::ostream_iterator<TagDefValPairT>(std::cout, "\n"));
	


	pdb.delete_tag(tag_we_want_to_delete_later);

	pdb.print_basic_diagnostics(std::cout);

	std::cout << "Print all tag pairs for the file:" << std::endl;
	std::copy(pdb.rnode_tag_set_begin(new_file), pdb.rnode_tag_set_end(new_file)
			, std::ostream_iterator<TagDefValPairT>(std::cout, "\n"));


	//std::cout.flush();
/*
	for (auto it = ts_begin; it != ts_end; ++it)
	{
		std::cout << *it << std::endl;
		
		std::cout.flush();
	}
*/
	pdb.delete_tag (tag_we_want_to_delete);

	
	pdb.print_basic_diagnostics(std::cout);
	


	/*
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
   */

	//delete segment;

	LOG(INFO) << "Plato daemon controller finishing. Daemon still running.";
	return 0;
}
