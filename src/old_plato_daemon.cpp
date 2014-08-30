// Compile with:
// g++ -std=c++11 -Wall -I src/ src/plato_daemon.cpp -o bin/plato_daemon

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/vector.hpp>
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
//	namespace bi = boost::interprocess;

	using namespace boost::interprocess;

	const unsigned shm_region_size = 1048576;
	const std::string shm_region_name ("PlatoDaemonSharedMemory");

/*
	std::string daemonCmd = "help";

	//std::cout << "argc: " << argc << std::endl;

	if (argc > 1)
	{
		daemonCmd = argv[1];
	}
*/
	/*
		bi::create_only
		bi::open_only
		bi::open_or_create

		bi::read_write
		bi::read_only
	 */

/*
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
*/

/*
	//	Remove shared memory on construction and destruction
	struct shm_remove {
		shm_remove() { bi::shared_memory_object::remove("PlatoDaemonSharedMemory"); }
//		~shm_remove() { bi::shared_memory_object::remove("PlatoDaemonSharedMemory"); }
	} remover;
*/
	
	using namespace PlatoDaemon;



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

	bi::managed_shared_memory segment(bi::create_only, shm_region_name.c_str(), shm_region_size);
	

	SharedVoidAllocator alloc_void_inst (segment.get_segment_manager());


	MyMap* mymap = segment.find_or_construct<MyMap>("MyMap")	//	Object name
												(std::less<KeyType>()
												, alloc_void_inst);

	
	//for (auto kv : *myComparisonMap)
	for (int i = 0; i < spv.size(); ++i)
	{
		auto spvL = spv.at(i).first;
      	auto spvR = spv.at(i).second;

		SharedStringT	key_object		(spvL.begin(), spvL.end(), alloc_void_inst);
		SharedStringT	mapped_object	(spvR.begin(), spvR.end(), alloc_void_inst);

/*
		SharedStringT	key_object		(kv.first.begin(), kv.first.end(), alloc_void_inst);
		SharedStringT	mapped_object	(kv.second.begin(), kv.second.end(), alloc_void_inst);
*/		
		//SharedStringT	key_object		(kv.first.c_str(), alloc_void_inst);
		//SharedStringT	mapped_object	(kv.second.c_str(), alloc_void_inst);

		//SharedStringT	key_object		(alloc_void_inst);
		//SharedStringT	mapped_object	(alloc_void_inst);

		//mymap->at(key_object) = mapped_object;
		
		//mymap->insert( std::pair<const SharedStringT, SharedStringT>(key_object, mapped_object) );

		//mymap->emplace(key_object, mapped_object);

		//mymap->emplace(SharedStringT (kv.first.begin(), kv.first.end(), alloc_void_inst),
		//				SharedStringT (kv.second.begin(), kv.second.end(), alloc_void_inst));
		
		ValueType		kv_object		(key_object, mapped_object);
		mymap->insert(kv_object);
		

		//kv_object.first = kv.first.c_str();
		//kv_object.second = kv.second.c_str();



	
	}

	return 0;
}
