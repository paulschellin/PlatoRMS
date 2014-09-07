

# g++ -std=c++11 -ggdb -Wall -I src/ src/plato_daemon.cpp -o bin/plato_daemon

# g++ -std=c++11 -Wall -I src/ src/plato_client.cpp -o bin/plato_client

# CXX=g++-4.9

source_dir=src/
output_dir=bin/


programs=plato_daemon plato_client plato_fuse

program_sources=$(addprefix $(source_dir),$(addsuffix .cpp,$(programs)))

headers=FileMap.hpp PFile.hpp RNode.hpp TagDef.hpp TagMap.hpp ExampleTagDefs.hpp PlatoDB.hpp
header_sources=$(addprefix $(source_dir),$(headers))

program_binaries=$(programs)
#program_binaries=$(addprefix $(output_dir),$(addsuffix 




MAKEFILE=makefile


std_lib_flags=-std=c++11

c_flags=-Wall -Wextra -g -ggdb

ld_flags=-Wall -Wextra -g -ggdb

include_dirs=-I. -I src/ -I easyloggingpp/src $(shell pkg-config fuse --cflags)

boost_libs=-lboost_program_options -lboost_system -lboost_iostreams -lboost_filesystem -lboost_serialization

fuse_libs=$(shell pkg-config fuse --cflags --libs)

libs=$(boost_libs) $(fuse_libs)




#CSTS_VERSION_FILE=version.csts.txt

#ifeq ($(wildcard ${CSTS_VERSION_FILE}),)
#CSTS_VERSION_STRING := $(shell echo 0.1.0 | tee ${CSTS_VERSION_FILE})
#else
#CSTS_VERSION_STRING := $(shell cat ${CSTS_VERSION_FILE})
#endif

#CSTS_VERSION_MAJOR = $(shell echo $(CSTS_VERSION_STRING) | sed "s/^\([0-9]*\).*/\1/")
#CSTS_VERSION_MINOR = $(shell echo $(CSTS_VERSION_STRING) | sed "s/[0-9]*\.\([0-9]*\).*/\1/")
#CSTS_VERSION_PATCH = $(shell echo $(CSTS_VERSION_STRING) | sed "s/[0-9]*\.[0-9]*\.\([0-9]*\).*/\1/")
#CSTS_VERSION_BUILD = $(shell echo $(CSTS_VERSION_STRING) | sed "s/.*-b\([0-9]*\)/\1/")

#GIT_COMMIT = $(shell git log --pretty=format:"%H - ")


versioning_flags=-DPLATO_DAEMON_VERSION_ID='"${PLATO_DAEMON_VERSION_STRING}"' -D'PLATO_DAEMON_GIT_COMMIT="$(GIT_COMMIT)"'



################

.PHONY: all
all:	$(program_binaries)


.SECONDEXPANSION:
$(program_binaries):	$(source_dir)$$@.cpp $(header_sources) $(MAKEFILE) 
	$(CXX) $(std_lib_flags) $(c_flags) $(include_dirs) $(libs) $(versioning_flags) $< -o $(output_dir)$@


.PHONY: run
run:	$(program_binaries)
	./$@


.PHONY: clean
clean:
	rm -f $(program_binaries)
