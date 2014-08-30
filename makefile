

# g++ -std=c++11 -ggdb -Wall -I src/ src/plato_daemon.cpp -o bin/plato_daemon

# g++ -std=c++11 -Wall -I src/ src/plato_client.cpp -o bin/plato_client

# CXX=g++-4.9

source_dir=src/
output_dir=bin/


programs=plato_daemon plato_client

program_sources=$(addprefix $(source_dir),$(addsuffix .cpp,$(programs)))

headers=FileMap.hpp PFile.hpp RNode.hpp TagDef.hpp TagMap.hpp ExampleTagDefs.hpp PlatoDB.hpp
header_sources=$(addprefix $(source_dir),$(headers))

program_binaries=$(programs)
#program_binaries=$(addprefix $(output_dir),$(addsuffix 




MAKEFILE=makefile


std_lib_flags=-std=c++11

c_flags=-Wall -Wextra -ggdb

ld_flags=-Wall -Wextra -ggdb

include_dirs=-I. -I src/

boost_libs=-lboost_program_options -lboost_system -lboost_iostreams -lboost_filesystem -lboost_serialization

libs=$(boost_libs)


################

.PHONY: all
all:	$(program_binaries)


.SECONDEXPANSION:
$(program_binaries):	$(source_dir)$$@.cpp $(header_sources) $(MAKEFILE) 
	$(CXX) $(std_lib_flags) $(c_flags) $(include_dirs) $(libs) $< -o $(output_dir)$@


.PHONY: run
run:	$(program_binaries)
	./$@


.PHONY: clean
clean:
	rm -f $(program_binaries)
