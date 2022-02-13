include Make_linux.inc
#include Make_msys2.inc
#include Make_osx.inc

CXXFLAGS = -std=c++11 -fPIC
ifdef DEBUG
CXXFLAGS += -g -O0 -Wall -fbounds-check -pedantic -D_GLIBCXX_DEBUG
else
CXXFLAGS += -O3 -march=native -Wall
endif

default:	help

all:	Ray_tracer.exe

Ray_tracer.exe:	Ray_tracer.cpp
	$(CXX) $(CXXFLAGS) -o Ray_tracer.exe Ray_tracer.cpp $(LIB)


help: 
	@echo "Available targets : "
	@echo "    all                      : compile all executables"
	@echo "Add DEBUG=yes to compile in debug"
	@echo "Configuration :"
	@echo "    CXX      :    $(CXX)"
	@echo "    CXXFLAGS :    $(CXXFLAGS)"

clean:
	@rm -f *.exe *~ 
	@rm -f *.gch *~

