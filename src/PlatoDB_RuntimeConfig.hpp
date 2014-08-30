

#pragma once

#include <boost/program_options.hpp>

#include <iostream>
#include <string>
#include <exception>





int rc_handler (std::ostream& os, po::variables_map& varMap, int ac, char* av[])
{
	namespace po = boost::program_options;
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

