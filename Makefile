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

all:	Ray_tracer.exe

Ray_tracer.exe:	Ray_tracer.cpp
	$(CXX) $(CXXFLAGS) -o Ray_tracer.exe Ray_tracer.cpp Ray_tracer.h lumiere.h objets.h utils.h materiaux.h

testplan: testplan.cpp
	$(CXX) $(CXXFLAGS) -o testplan.exe testplan.cpp objets.h utils.h materiaux.h

help: 
	@echo "Available targets : "
	@echo "    all                      : compile all executables"
	@echo "    clean                    : remove all .exe and .gch files"
	@echo "Add DEBUG=yes to compile in debug"
	@echo "Configuration :"
	@echo "    CXX      :    $(CXX)"
	@echo "    CXXFLAGS :    $(CXXFLAGS)"

clean:
	@rm -f *.exe *~ 
	@rm -f *.gch *~

