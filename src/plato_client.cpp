// Compile with:	
// g++ -std=c++11 -Wall -I src/ src/plato_client.cpp -o bin/plato_client

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>

#include <vector>
#include <map>
#include <string>
#include <utility>
#include <algorithm>
#include <iterator>

#include <iostream>

#include <cstdlib>	//	for std::exit()


#include <prototype_structs.hpp>

#include <tclap/CmdLine.h>

#include <easylogging++.h>
_INITIALIZE_EASYLOGGINGPP

using namespace boost::interprocess;

/*
	Logging levels:
	Global, Trace, Debug, Fatal, Error, Warning, Info, Verbose, Unknown

 */

struct command_struct {
	std::string description;
	//bool can_have_args;
	//bool needs_args;
	//bool has_multiple_args;
	std::size_t number_of_args;	//zero indicates infinite?

};

void
logging_configuration()
{
	el::Configurations conf("plato_easylogging++.conf");

	el::Loggers::reconfigureLogger("default", conf);
	//el::Loggers::reconfigureAllLoggers(conf);
}

/*
std::map<std::string, command_struct> allowed_commandMap = {
		{"help",		{"provides help", 1}}
		{"create",		{"create an object", 1}}
		{"new", 		{"create an object", 1}}
		{"delete", 		{"delete an object", 1}}
		{"list", 		{"list objects", 1}}
		{"modify", 		{"modify an object", 1}}
		{"merge", 		{"merge an object", 1}}
		{"duplicate",	
		{"add", 
		{"remove" 
		};
*/



std::vector<std::string> allowed_commands = {
		"help", 
		"create",
		"new", 
		"delete", 
		"list", 
		"info",
		"view",
		"modify", 
		"merge", 
		"duplicate", 
		"add", 
		"remove" 
		};

//std::map<std::string, > allowed_cmd_objects = {
std::vector<std::string> allowed_cmd_objects = {
		"tag",
		"tags",
		"file",
		"files",
		"rnode",
		"rnodes",
		"scope",
		"scopes"
		};


/*
		list tags		lists all tags
		list tag		lists 

		list-tags
		create-tag [3]
		delete-tag [1]
		view-tag   [1]
		modify-tag [1] [3]

		list-files
		create-file	
		delete-file
		view-file
		modify-file

		

 */

void
parse_cmd_line_arguments (int argc, char* argv[])
{
	if (argc > 2)
	{
		std::string cmdString (argv[1]);
		std::string objString (argv[2]);

		if ((cmdString == "list"_s) && (objString == "tags"))
		{

		}

	}
}


int main (int argc, char* argv[])
{
	
	logging_configuration();
	
//  try{
	/*
	TCLAP::CmdLine cmd("Plato userspace utility", ' ', "0.01");

	TCLAP::ValuesConstraint<std::string> cmdAllowedVals (allowed_commands);
	TCLAP::ValueArg<std::string> commandArg ("c", "command", "Tell the client what to do.", true, "help", &cmdAllowedVals, cmd);
	//TCLAP::UnlabeledValueArg<std::string> commandArg ("command", "Tell the client what to do.", true, "help", &cmdAllowedVals, cmd);


	TCLAP::ValuesConstraint<std::string> cmdAllowedObjs (allowed_cmd_objects);


	TCLAP::ValueArg<std::string> objectArg ("t", "object", "tells the client what to operate on", true, "", &cmdAllowedObjs, cmd);
	//TCLAP::UnlabeledValueArg<std::string> objectArg ("what", "tells the client what to operate on", true, "", &cmdAllowedObjs, cmd);


//	TCLAP::UnlabeledMultiArg<std::string> otherArgs ("other arguments", "like the names of the files or tags, etc.", false, "filenames, tag names, etc.", cmd);

	cmd.setExceptionHandling(false);

	cmd.parse(argc, argv);

	auto cmdArg = commandArg.getValue();
	auto objArg = objectArg.getValue();
	//auto objArg = otherArgs.getValue().front();
//	auto xtraArgs = otherArgs.getValue();

	LOG(INFO) << "Started " << argv[0] << ". Command arg was \"" << cmdArg << "\" and object arg was \"" << objArg << "\".";
	*/

	LOG(INFO) << "Started " << argv[0] << ".";

	std::string cmdArg = "help";
	std::string objArg = "";

	std::vector<std::string> xtraArgs;


	if (argc > 1) {
		cmdArg = std::string(argv[1]);
		if (argc > 2) {
			objArg = std::string(argv[2]);
		}
		if (argc > 3) {
			for (int i = 3; i < argc; ++i)
				xtraArgs.push_back(argv[i]);
		}
	}


	if (cmdArg == "help"_s)
	{
		//	Help doesn't do anything really, we'll work on it later.
	}
	else
	{
		std::shared_ptr<managed_shared_memory> segment;
		std::string segment_name ("PlatoDaemonSharedMemory");

		try {
			segment.reset(new managed_shared_memory(open_only, segment_name.c_str()));
		} catch (const interprocess_exception& e) {
			LOG(FATAL) << "An exception occurred while attempting to connect to the shared memory segment. The exception is: " << e.what();
			std::exit(1);
		} catch (...) {
			LOG(FATAL) << "An unknown/unexpected exception occurred while attempting to connect to the shared memory segment.";
			std::exit(1);
		}

		LOG(INFO) << "Connected to shared memory segment \"" << segment_name << "\".";
	
		PlatoDB pdb (*segment);

		LOG(INFO) << "Created PlatoDB object.";

		//pdb.print_basic_diagnostics (std::cout);

		
		//	Go through the commands


		if (cmdArg == "list"_s)
		{
			if (objArg == "tag"_s || objArg == "tags"_s )
			{
				std::copy(pdb.tags_begin(), pdb.tags_end(), std::ostream_iterator<TagDefT>(std::cout, "\n"));
			} else
			if (objArg == "file"_s || objArg == "files"_s
					|| objArg == "rnode"_s || objArg == "rnodes"_s)
			{
				std::copy(pdb.rnodes_begin(), pdb.rnodes_end(), std::ostream_iterator<RNode>(std::cout, "\n"));
			} else
			if (objArg == "scope"_s || objArg == "scopes"_s )
			{
				std::cout << "Hm... scopes don't appear to be implemented yet. Sorry!" << std::endl;
			}

		} else
		if (cmdArg == "create"_s || cmdArg == "new"_s )
		{
			if (objArg == "tag"_s || objArg == "tags"_s )
			{
				if (xtraArgs.size() >= 3)
				{
					pdb.create_tag(xtraArgs.at(0), xtraArgs.at(1), xtraArgs.at(2));
					LOG(INFO) << "Created new tag.";
				} else
				{
					LOG(WARNING) << "Couldn't create new tag. Not enough arguments.";
				}
			} else
			if (objArg == "file"_s || objArg == "files"_s
					|| objArg == "rnode"_s || objArg == "rnodes"_s)
			{
				pdb.create_rnode();
				LOG(WARNING) << "There isn't a good way to find this file/rnode once it's created!";
			} else
			if (objArg == "scope"_s || objArg == "scopes"_s )
			{
				LOG(WARNING) << "Hm... scopes don't appear to be implemented yet. Sorry!";
			}


		} else
		if (cmdArg == "add"_s)
		{
			if (objArg == "tag"_s || objArg == "tags"_s )
			{
				if (xtraArgs.size() == 2)
				{
					auto td = pdb.get_tag(xtraArgs.at(0));
					//auto rn = pdb.get_rnode(xtraArgs.at(1));
					//pdb.add_tag_to_rnode(td, rn, ""_s);
				} else
				if (xtraArgs.size() == 3)
				{
					auto td = pdb.get_tag(xtraArgs.at(0));
					//auto rn = pdb.get_rnode(xtraArgs.at(1));
					//pdb.add_tag_to_rnode(td, rn, xtraArgs.at(2));
				}
			} else
			if (objArg == "file"_s || objArg == "files"_s
					|| objArg == "rnode"_s || objArg == "rnodes"_s)
			{
				LOG(WARNING) << "Sorry, this syntax just doesn't make any sense!";
			} else
			if (objArg == "scope"_s || objArg == "scopes"_s )
			{
				LOG(WARNING) << "Hm... scopes don't appear to be implemented yet. Sorry!";
			}
		} else
		if (cmdArg == "remove"_s)
		{
			if (objArg == "tag"_s || objArg == "tags"_s )
			{
				if (xtraArgs.size() == 2)
				{
					auto td = pdb.get_tag(xtraArgs.at(0));
					//auto rn = pdb.get_rnode(xtraArgs.at(1));
					//pdb.remove_tag_from_rnode(td, rn);
				} 
			//	if (xtraArgs.size() >= 3)
			//		pdb.create_tag(xtraArgs.at(0), xtraArgs.at(1), xtraArgs.at(2));
			} else
			if (objArg == "file"_s || objArg == "files"_s
					|| objArg == "rnode"_s || objArg == "rnodes"_s)
			{
				LOG(WARNING) << "Sorry, this syntax just doesn't make any sense!";
			} else
			if (objArg == "scope"_s || objArg == "scopes"_s )
			{
				LOG(WARNING) << "Hm... scopes don't appear to be implemented yet. Sorry!";
			}
		}
	}

//  } catch (const TCLAP::ArgException& e)
//  { LOG(FATAL) << "Failed to parse arguments. Error is: " << e.error() << " " << e.argId(); }
	
	return 0;
}

