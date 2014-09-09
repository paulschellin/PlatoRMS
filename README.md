PlatoRMS
========

The Plato Semantic Resource Management System


# DISCLAIMER #

This repository contains no end-user code. It contains almost no code that would be interesting to a developer. This repository is effectively the scratchpad in which I've been tinkering with early design decisions for Plato.

I pushed this repository to github as a type of commitment device for myself. I found that it can be easy to let myself become complacent when there is no externally-observable evidence of progress. Hopefully the commitment device is effective.

If you are interested in looking through my scratchwork, be my guest -- but be warned that there is a large amount of dead code, and a large deficit of documentation. I hope to work on that throughout September/October 2014.


## Dependencies ##

You will need:

- a [C++11 compatible](http://cpprocks.com/c11-compiler-support-shootout-visual-studio-gcc-clang-intel/) compiler
- [boost](http://www.boost.org/) (including some compiled boost libraries!)
- [easylogging++](http://easylogging.org/) (You'll need to put it into the main directory. I may change the logging tool to either [boost::log](http://boost-log.sourceforge.net/libs/log/doc/html/index.html) or [Google's glog](https://code.google.com/p/google-glog/) in the future.

### More about boost usage ###

I use the following libraries heavily:

- [boost::interprocess](http://www.boost.org/doc/libs/1_56_0/doc/html/interprocess.html) for shared memory
- [boost::filesystem](http://www.boost.org/doc/libs/1_56_0/libs/filesystem/doc/index.htm) for basic filesystem actions
- boost::program_options

I might make use of more in the future, namely [boost::log](http://boost-log.sourceforge.net/libs/log/doc/html/index.html) or something for better parsing.


## How to build

### Download the software

```Shell
git clone https://github.com/paulschellin/PlatoRMS.git
```

### Build the three binaries

```Shell
make plato_daemon
```

```Shell
make plato_client
```

```Shell
make test_plato_daemon
```


