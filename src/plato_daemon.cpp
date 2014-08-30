// Compile with:
// g++ -std=c++11 -Wall -I src/ src/plato_daemon.cpp -o bin/plato_daemon

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/string.hpp>

#include <boost/interprocess/allocators/allocator.hpp>

#include <functional>
#include <utility>

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>

#include <memory>

#include <cstdlib>


#include <plato_daemon_common.hpp>



/*
	http://www.boost.org/doc/libs/1_44_0/doc/html/interprocess/allocators_containers.html#interprocess.allocators_containers.containers_explained.containers_of_containers
 */



int main (int argc, char* argv[])
{
	namespace bi = boost::interprocess;

	const unsigned shm_region_size = 1048576;
	const std::string shm_region_name ("PlatoDaemonSharedMemory");

	std::string daemonCmd = "help";

	//std::cout << "argc: " << argc << std::endl;

	if (argc > 1)
	{
		daemonCmd = argv[1];
	}

	/*
		bi::create_only
		bi::open_only
		bi::open_or_create

		bi::read_write
		bi::read_only
	 */

	std::shared_ptr<bi::managed_shared_memory> segment;

	if (daemonCmd.compare("start") == 0)
	{
		segment.reset( new bi::managed_shared_memory (bi::open_or_create, shm_region_name.c_str(), shm_region_size) );
	} else
	if (daemonCmd.compare("stop") == 0)
	{
		bi::shared_memory_object::remove(shm_region_name.c_str());

		std::exit(0);
	} else
	if (daemonCmd.compare("restart") == 0)
	{
		bi::shared_memory_object::remove(shm_region_name.c_str());
		segment.reset( new bi::managed_shared_memory (bi::create_only, shm_region_name.c_str(), shm_region_size) );

	} else
	if (daemonCmd.compare("status") == 0)
	{
		
		std::exit(1);
	} else
	if (daemonCmd.compare("help") == 0)
	{
		std::cout << "Usage: " << argv[0] << " [ \"start\" | \"stop\" | \"restart\" | \"status\" | \"help\" ]" << std::endl;
		std::exit(1);
	}


/*
	//	Remove shared memory on construction and destruction
	struct shm_remove {
		shm_remove() { bi::shared_memory_object::remove("PlatoDaemonSharedMemory"); }
//		~shm_remove() { bi::shared_memory_object::remove("PlatoDaemonSharedMemory"); }
	} remover;
*/
	
	using namespace PlatoDaemon;



	std::vector< std::pair< const std::string, const std::string > > stringPairVec = {
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

/*
	for (auto kvPair : *myComparisonMap)
	{
		std::cout << "{ " << kvPair.first << " : " << kvPair.second << " }" << std::endl;

	}

	
	std::map<std::string, std::size_t>	storageSizes;
	std::vector<std::pair<std::size_t, std::size_t> > elemStorageSizes;
	std::vector<std::pair<std::size_t, std::size_t> > elemCapStorageSizes;

	storageSizes["MyMap"] = sizeof(MyMap);
	storageSizes["ValueType"] = sizeof(ValueType);
	
	for (auto kvPair : *myComparisonMap)
	{
		elemStorageSizes.push_back(std::make_pair(sizeof(kvPair.first), sizeof(kvPair.second)));
		elemCapStorageSizes.push_back(std::make_pair(sizeof(char) * kvPair.first.capacity(), sizeof(char) * kvPair.second.capacity()));
	}


	std::cout << "The type MyMap has an overhead of " << storageSizes["MyMap"] << " bytes." << std::endl;
	std::cout << "The type ValueType has an overhead of " << storageSizes["ValueType"] << " bytes." << std::endl;


	std::cout << "Total: ";

	std::size_t totalMem = storageSizes["MyMap"] + myComparisonMap->size() * storageSizes["ValueType"];

	for (auto kvPair : elemStorageSizes)
		totalMem += kvPair.first + kvPair.second;

	for (auto kvPair : elemCapStorageSizes)
		totalMem += kvPair.first + kvPair.second;

	std::cout << totalMem;

	std::cout << std::endl;
*/


	




	SharedVoidAllocator alloc_void_inst (segment->get_segment_manager());


	MyMap* mymap = segment->find_or_construct<MyMap>("MyMap")	//	Object name
												(std::less<KeyType>()
												, alloc_void_inst);

	
	//for (auto kv : *myComparisonMap)
	for (auto kv : stringPairVec)
	{
	
		SharedStringT	key_object		(kv.first.begin(), kv.first.end(), alloc_void_inst);
		SharedStringT	mapped_object	(kv.second.begin(), kv.second.end(), alloc_void_inst);
		
		//SharedStringT	key_object		(kv.first.c_str(), alloc_void_inst);
		//SharedStringT	mapped_object	(kv.second.c_str(), alloc_void_inst);

		//SharedStringT	key_object		(alloc_void_inst);
		//SharedStringT	mapped_object	(alloc_void_inst);

		//mymap->at(key_object) = mapped_object;
		
		//mymap->insert( std::pair<const SharedStringT, SharedStringT>(key_object, mapped_object) );

		//mymap->emplace(key_object, mapped_object);

		mymap->emplace(SharedStringT (kv.first.begin(), kv.first.end(), alloc_void_inst),
						SharedStringT (kv.second.begin(), kv.second.end(), alloc_void_inst));
		
		//ValueType		kv_object		(key_object, mapped_object);
		//mymap->insert(kv_object);
		

		//kv_object.first = kv.first.c_str();
		//kv_object.second = kv.second.c_str();



	
	}

	return 0;
}
