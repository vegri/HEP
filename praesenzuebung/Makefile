# This is the Makefile to make the macros 


ROOTLIBS     := $(shell root-config --libs)
ifeq ($(mode),debug)
   CFLAGS = -g -O0 -Wall
else
   mode = release
   CFLAGS = -g -Wall
endif
CXX           = g++

CXXFLAGS      = $(shell root-config --cflags) $(CFLAGS) -I. -Iinclude
LIBS          = $(ROOTLIBS) -I.

LD            = g++
LDFLAGS       = 

#*****************************************************************************
#* Targets
#*****************************************************************************
#BINARY1 = Blatt_1
BINARY2 = PraesenzUebung_1






#OBJECT1 = $(BINARY1).o
OBJECT2 = $(BINARY2).o


####### all binaries #############
BINARIES =  $(BINARY2) $(BINARY3)
#$(BINARY1)

#*****************************************************************************
#* Dependencies
#*****************************************************************************

all: $(BINARIES)

#$(BINARY1) :   $(OBJECT1)
#	$(LD) $(LDFLAGS) $(OBJECT1) $(LIBS) -o $(BINARY1)
#	@echo "$@ done"

$(BINARY2) :   $(OBJECT2)
	$(LD) $(LDFLAGS) $(OBJECT2) $(LIBS) -o $(BINARY2)
	@echo "$@ done"


%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

.PHONY : clean

clean:
	$(RM) *.o $(BINARIES)
