include Make_linux.inc
#include Make_msys2.inc
#include Make_osx.inc

CXXFLAGS = -std=c++11 -fPIC
ifdef DEBUG
CXXFLAGS += -g -O0 -fbounds-check -pedantic -D_GLIBCXX_DEBUG
else
CXXFLAGS += -O0 -march=native -Wall
endif

default:	help

clean:
	@rm -f *.exe *~ bin/*.o
	@rm -f *.gch *~

HEADS = bin/lumiere.o bin/Ray_tracer.o bin/objets.o bin/utils.o

all:	Ray_tracer.exe

bin/lumiere.o:
	$(CXX) $(CXXFLAGS) -c lumiere.h -o lumiere.o

bin/Ray_tracer.o:
	$(CXX) $(CXXFLAGS) -c Ray_tracer.h -o Ray_tracer.o

bin/objets.o:
	$(CXX) $(CXXFLAGS) -c objets.h -o objets.o

bin/utils.o:
	$(CXX) $(CXXFLAGS) -c utils.h -o utils.o

Ray_tracer.exe:	$(HEADS) Ray_tracer.cpp
	$(CXX) $(CXXFLAGS) $($EADS) Ray_tracer.cpp -o Ray_tracer.exe


help: 
	@echo "Available targets : "
	@echo "    all                      : compile all executables"
	@echo "    clean                    : remove all .exe and .gch files"
	@echo "Add DEBUG=yes to compile in debug"
	@echo "Configuration :"
	@echo "    CXX      :    $(CXX)"
	@echo "    CXXFLAGS :    $(CXXFLAGS)"



