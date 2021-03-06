

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


	const std::string shm_region_name (vm["shared-memory-name"].as<std::string>().c_str());
	const std::string mmapped_filename(vm["memory-mapped-filename"].as<std::string>().c_str());

	GenericSharedMemory<managed_mapped_file> sh_memory (mmapped_filename);
	
	
	LOG(INFO) << "Created the shared memory segment \"" << shm_region_name << "\".";


	PlatoDB pdb (*sh_memory.segment);

	LOG(INFO) << "Created PlatoDB object.";

	LOG(INFO) << "Started the daemon.";

	LOG(INFO) << "Plato daemon controller finishing. Daemon still running.";
	return 0;
}
