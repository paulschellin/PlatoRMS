// Compile with:	
// g++ -std=c++11 -Wall -I src/ src/plato_client.cpp -o bin/plato_client

#include <boost/interprocess/managed_shared_memory.hpp>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>

#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <iterator>

#include <iostream>

#include <cstdlib>	//	for std::exit()

#include <boost/filesystem.hpp>

#include <plato_shared_memory.hpp>
#include <prototype_structs.hpp>

#include <easylogging++.h>
_INITIALIZE_EASYLOGGINGPP

namespace bfs = boost::filesystem;

using namespace boost::interprocess;

/*
	Logging levels:
	Global, Trace, Debug, Fatal, Error, Warning, Info, Verbose, Unknown

 */

void
logging_configuration()
{
	el::Configurations conf("plato_easylogging++.conf");

	el::Loggers::reconfigureLogger("default", conf);
}


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


void list_tag (PlatoDB& pdb, std::vector<std::string>& xtraArgs)
{
	auto td = pdb.get_tag(xtraArgs.at(0));

	std::cout << "Print all file/rnode pairs for the tag:" << std::endl;
	std::copy(pdb.tagdef_rnode_set_begin(td), pdb.tagdef_rnode_set_end(td)
				, std::ostream_iterator<RNodeValPairT>(std::cout, "\n"));
}


void list_tags (PlatoDB& pdb, std::vector<std::string>& xtraArgs)
{
	std::copy(pdb.tags_begin(), pdb.tags_end(), std::ostream_iterator<TagDefT>(std::cout, "\n"));
}

void list_rnode (PlatoDB& pdb, std::vector<std::string>& xtraArgs)
{
				bfs::path p = bfs::canonical(bfs::path(xtraArgs.front()));

				if (bfs::exists(p))
				{
					//auto rn = pdb.get_rnode(std::stoi(xtraArgs.at(0)));
					
					auto path_tag = pdb.get_tag("path"_s);

					auto rn = pdb.get_rnode_tag_with(path_tag, p.relative_path().string());

					std::cout << "Print all tag pairs for the file:" << std::endl;
					std::copy(pdb.rnode_tag_set_begin(rn), pdb.rnode_tag_set_end(rn)
						, std::ostream_iterator<TagDefValPairT>(std::cout, "\n"));

				}
				else
				{
					LOG(WARNING) << "Could not list file/rnode. The file: " << p << " does not exist!";
				}

}

void list_rnodes (PlatoDB& pdb, std::vector<std::string>& xtraArgs)
{
	std::copy(pdb.rnodes_begin(), pdb.rnodes_end(), std::ostream_iterator<RNode>(std::cout, "\n"));
}

void list_scopes (PlatoDB& pdb, std::vector<std::string>& xtraArgs)
{
	std::cout << "Hm... scopes don't appear to be implemented yet. Sorry!" << std::endl;
}

void create_tags (PlatoDB& pdb, std::vector<std::string>& xtraArgs)
{
	if (xtraArgs.size() >= 3)
	{
		auto new_tag = pdb.create_tag(xtraArgs.at(0), xtraArgs.at(1), xtraArgs.at(2));
					
		LOG(INFO) << "Created new tag: " << *new_tag;
	} else
	{
		LOG(WARNING) << "Couldn't create new tag. Not enough arguments.";
	}
}

void create_rnodes (PlatoDB& pdb, std::vector<std::string>& xtraArgs)
{
	if (xtraArgs.size() >= 1)
	{
		bfs::path p = bfs::canonical(bfs::path(xtraArgs.front()));

		if (bfs::exists(p))
		{
			auto fs_root = p.root_path();
			auto fs_path = p.relative_path();

			auto fs_filename = p.filename();


						//pdb.get_tag("
						//pdb.add_tag_to_rnode(pdb.get_tag("filesystem"_s, new_file, fs_root.string());
						//pdb.add_tag_to_rnode(pdb.get_tag("path"_s, new_file, fs_path.string());

			auto pathTag = pdb.get_tag("path"_s);
						//auto pathTagRNodeSetBegin = pdb.tagdef_rnode_set_begin(pathTag);
						//auto pathTagRNodeSetEnd = pdb.tagdef_rnode_set_end(pathTag);

						//auto fsTag = pdb.get_tag("filesystem"_s);
						//auto fsTagRNodeSetBegin = pdb.tagdef_rnode_set_begin(fsTag);
						//auto fsTagRNodeSetEnd = pdb.tagdef_rnode_set_end(fsTag);


						//	check if file already exists
			if (! pdb.find_rnode_tag_with(pathTag, fs_path.string()))
			{

				auto new_file = pdb.create_rnode();
						
				pdb.add_tag_to_rnode(pdb.get_tag("filesystem"_s), new_file, fs_root.string());
				pdb.add_tag_to_rnode(pdb.get_tag("path"_s), new_file, fs_path.string());
				pdb.add_tag_to_rnode(pdb.get_tag("filename"_s), new_file, fs_filename.string());

				LOG(INFO) << "Created new file/rnode: " << *new_file;
			}
			else
			{
				LOG(WARNING) << "Didn't create new rnode. RNode already exists!";
			}
		}
		else
		{
			LOG(WARNING) << "Couldn't create new rnode. File does not exist: " << p.string();
		}
	}
	else
	{
		LOG(WARNING) << "Couldn't create new rnode. Not enough arguments.";
	}

}

void create_scopes (PlatoDB& pdb, std::vector<std::string>& xtraArgs)
{
	LOG(WARNING) << "Hm... scopes don't appear to be implemented yet. Sorry!";
}

void delete_tags (PlatoDB& pdb, std::vector<std::string>& xtraArgs)
{
	if (xtraArgs.size() >= 1)
	{
		auto tag_to_delete = pdb.get_tag(xtraArgs.at(0));
					
		if (tag_to_delete != pdb.tags_end())
		{
			LOG(INFO) << "Deleting tag: " << *tag_to_delete;
			pdb.delete_tag(tag_to_delete);
			LOG(INFO) << "Tag was deleted.";
		}
		else
		{
			LOG(INFO) << "Tag couldn't be found, so it was not deleted.";
		}
		} else
		{
			LOG(WARNING) << "Couldn't delete tag. Not enough arguments.";
		}
}

void delete_rnodes (PlatoDB& pdb, std::vector<std::string>& xtraArgs)
{
	//	nothing to see here... yet.
}

void delete_scopes (PlatoDB& pdb, std::vector<std::string>& xtraArgs)
{
	LOG(WARNING) << "Hm... scopes don't appear to be implemented yet. Sorry!";
}

void add_tags (PlatoDB& pdb, std::vector<std::string>& xtraArgs)
{
	if (xtraArgs.size() >= 2)
				{
					auto td = pdb.get_tag(xtraArgs.at(0));
					
					bfs::path p = bfs::canonical(bfs::path(xtraArgs.at(1)));
					auto path_tag = pdb.get_tag("path"_s);
					
					if (bfs::exists(p)){
						auto rn = pdb.get_rnode_tag_with(path_tag, p.relative_path().string());
						if (rn != pdb.rnodes_end())
						{
							std::string tagval ("");
							
							if (xtraArgs.size() >= 3)
								tagval = std::string(xtraArgs.at(2));

							pdb.add_tag_to_rnode(td, rn, tagval);
							LOG(INFO) << "Added tag: " << *td << " to rnode: " << *rn << " with value: " << tagval << ".";

						}
						else
						{
							LOG(WARNING) << "Couldn't add tag to rnode because the filename specified does not exist in the Plato database.";

						}
					}
					else
					{
						LOG(WARNING) << "Couldn't add tag to rnode because the filename specified does not exist.";
					}

					//pdb.add_tag_to_rnode(td, rn, ""_s);
				} 
}

void add_rnodes (PlatoDB& pdb, std::vector<std::string>& xtraArgs)
{
	LOG(WARNING) << "Sorry, this syntax just doesn't make any sense!";
}

void add_scopes (PlatoDB& pdb, std::vector<std::string>& xtraArgs)
{
	LOG(WARNING) << "Hm... scopes don't appear to be implemented yet. Sorry!";
}

void remove_tags (PlatoDB& pdb, std::vector<std::string>& xtraArgs)
{
	if (xtraArgs.size() == 2)
				{
					auto td = pdb.get_tag(xtraArgs.at(0));
					//auto rn = pdb.get_rnode(xtraArgs.at(1));
					//pdb.remove_tag_from_rnode(td, rn);
				}
}


void remove_rnodes (PlatoDB& pdb, std::vector<std::string>& xtraArgs)
{
	LOG(WARNING) << "Sorry, this syntax just doesn't make any sense!";
}


void remove_scopes (PlatoDB& pdb, std::vector<std::string>& xtraArgs)
{
	LOG(WARNING) << "Hm... scopes don't appear to be implemented yet. Sorry!";
}

void modify_tag (PlatoDB& pdb, std::vector<std::string>& xtraArgs)
{
				if (xtraArgs.size() == 2)
				{
					auto td = pdb.get_tag(xtraArgs.at(0));
					std::size_t rn_uuid = std::stoi(xtraArgs.at(1));
					auto rn = pdb.get_rnode(rn_uuid);
					pdb.modify_rnode_tag(td, rn, ""_s);
				} else
				if (xtraArgs.size() == 3)
				{
					auto td = pdb.get_tag(xtraArgs.at(0));
					std::size_t rn_uuid = std::stoi(xtraArgs.at(1));
					auto rn = pdb.get_rnode(rn_uuid);
					pdb.modify_rnode_tag(td, rn, xtraArgs.at(2));
				}
}

void modify_rnodes (PlatoDB& pdb, std::vector<std::string>& xtraArgs)
{

}

void modify_scopes (PlatoDB& pdb, std::vector<std::string>& xtraArgs)
{
	LOG(WARNING) << "Hm... scopes don't appear to be implemented yet. Sorry!";
}

int main (int argc, char* argv[])
{
	logging_configuration();
	
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
		const std::string mmapped_filename ("PlatoDaemonFile.mmap");
		const std::string segment_name(mmapped_filename);

		GenericSharedMemory<managed_mapped_file> sh_memory (mmapped_filename);


		LOG(INFO) << "Connected to shared memory segment \"" << segment_name << "\".";
	
		PlatoDB pdb (*sh_memory.segment);

		LOG(INFO) << "Created PlatoDB object.";

		
		//	Go through the commands


		if (cmdArg == "list"_s)
		{
		
			if (objArg == "tag"_s)
				list_tag(pdb, xtraArgs);
			else if (objArg == "tags"_s)
				list_tags(pdb, xtraArgs);
			else if (objArg == "file"_s || objArg == "rnode"_s)
				list_rnode(pdb, xtraArgs);
			else if (objArg == "files"_s || objArg == "rnodes"_s)
				list_rnodes(pdb, xtraArgs);
			else if (objArg == "scope"_s || objArg == "scopes"_s )
				list_scopes(pdb, xtraArgs);
			else
				LOG(WARNING) << "Invalid argument for list: " << objArg;

		} else
		if (cmdArg == "create"_s || cmdArg == "new"_s )
		{

			if (objArg == "tag"_s || objArg == "tags"_s )
				create_tags(pdb, xtraArgs);
			else if (objArg == "file"_s || objArg == "files"_s || objArg == "rnode"_s || objArg == "rnodes"_s)
				create_rnodes(pdb, xtraArgs);	
			else if (objArg == "scope"_s || objArg == "scopes"_s )
				create_scopes(pdb, xtraArgs);
			else
				LOG(WARNING) << "Invalid argument for new/create: " << objArg;

		} else
		if (cmdArg == "delete"_s)
		{

			if (objArg == "tag"_s || objArg == "tags"_s )
				delete_tags(pdb, xtraArgs);
			else if (objArg == "file"_s || objArg == "files"_s || objArg == "rnode"_s || objArg == "rnodes"_s)
				delete_rnodes(pdb, xtraArgs);
			else if (objArg == "scope"_s || objArg == "scopes"_s )
				delete_scopes(pdb, xtraArgs);
			else
				LOG(WARNING) << "Invalid argument for delete: " << objArg;

		} else
		if (cmdArg == "add"_s)
		{

			if (objArg == "tag"_s || objArg == "tags"_s )
				add_tags(pdb, xtraArgs);
			else if (objArg == "file"_s || objArg == "files"_s || objArg == "rnode"_s || objArg == "rnodes"_s)
				add_rnodes(pdb, xtraArgs);
			else if (objArg == "scope"_s || objArg == "scopes"_s )
				add_scopes(pdb, xtraArgs);
			else
				LOG(WARNING) << "Invalid argument for add: " << objArg;

		} else
		if (cmdArg == "remove"_s)
		{

			if (objArg == "tag"_s || objArg == "tags"_s )
				remove_tags(pdb, xtraArgs);
			else if (objArg == "file"_s || objArg == "files"_s || objArg == "rnode"_s || objArg == "rnodes"_s)
				remove_rnodes(pdb, xtraArgs);
			else if (objArg == "scope"_s || objArg == "scopes"_s )
				remove_scopes(pdb, xtraArgs);
			else
				LOG(WARNING) << "Invalid argument for remove: " << objArg;

		} else
		if (cmdArg == "modify"_s)
		{

			if (objArg == "tag"_s || objArg == "tags"_s )
				modify_tag(pdb, xtraArgs);
			else if (objArg == "file"_s || objArg == "files"_s || objArg == "rnode"_s || objArg == "rnodes"_s)
				modify_rnodes(pdb, xtraArgs);
			else if (objArg == "scope"_s || objArg == "scopes"_s )
				modify_scopes(pdb, xtraArgs);
			else
				LOG(WARNING) << "Invalid argument for modify: " << objArg;
				
		} else
			LOG(WARNING) << "Invalid command: " << cmdArg;
	}

	return 0;
}

