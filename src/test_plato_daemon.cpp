//	Compile with:
//	g++ -std=c++11 -Wall -Wextra -I src/ src/test_plato_daemon.cpp -o bin/test_plato_daemon

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <cstdlib>	//	for std::exit()
#include <exception>
#include <memory>	//	for shared_ptr()


#include <utilities.hpp>

#include <plato_shared_memory.hpp>
#include <prototype_structs.hpp>
#include <plato_common_functions.hpp>

namespace bi = boost::interprocess;


int main ()
{


	//unsigned shm_region_size_bytes = 1048576; // estimate_space_requirements();
	const std::string mmapped_filename ("PlatoDaemonFile.mmap");
	const std::string segment_name(mmapped_filename);


	GenericSharedMemory<bi::managed_mapped_file> sh_memory (mmapped_filename);


	PlatoDB pdb (*sh_memory.segment);

	auto filename_tag = pdb.create_tag("filename"_s, "string"_s, "the name of the file"_s);

	auto fs_tag = pdb.create_tag("filesystem"_s, "string"_s, "the filesystem of the actual file"_s);

	auto path_tag = pdb.create_tag("path"_s, "string"_s, "the full path of the file"_s);

	auto file_size_tag = pdb.create_tag("size"_s, "int"_s, "the size of the file in bytes"_s);

	auto tag_we_want_to_delete = pdb.create_tag("test of deletion"_s, "string"_s, "this should be deleted by the beginning of the test"_s);

	auto tag_we_want_to_delete_later = pdb.create_tag("test of deletion 2"_s, "string"_s, "this should be deleted by the end of the test"_s);


	std::cout << "Size of tag list: " << pdb.count_tags() << std::endl;

	auto new_file = pdb.create_rnode();

	
	
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

	

	pdb.print_basic_diagnostics(std::cout);

	std::cout << "Print all tag pairs for the file:" << std::endl;
	std::copy(pdb.rnode_tag_set_begin(new_file), pdb.rnode_tag_set_end(new_file)
			, std::ostream_iterator<TagDefValPairT>(std::cout, "\n"));
	


	pdb.delete_tag(tag_we_want_to_delete_later);

	pdb.print_basic_diagnostics(std::cout);

	std::cout << "Print all tag pairs for the file:" << std::endl;
	std::copy(pdb.rnode_tag_set_begin(new_file), pdb.rnode_tag_set_end(new_file)
			, std::ostream_iterator<TagDefValPairT>(std::cout, "\n"));


	pdb.delete_tag (tag_we_want_to_delete);


	//std::cout << "What about that tag we tried to delete? Here:" << std::endl
	//	<< *tag_we_want_to_delete << std::endl;

	std::cout << std::endl;

	pdb.print_basic_diagnostics(std::cout);


	std::cout << std::endl;
	std::cout << std::endl;
	std::copy(pdb.tags_begin(), pdb.tags_end(), std::ostream_iterator<TagDefT>(std::cout, "\n"));
	std::cout << std::endl;

	return 0;
}
