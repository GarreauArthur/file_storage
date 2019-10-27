# move libs to /usr/local/lib in production

CXX = g++
CXXFLAGS = -flto -O3 -std=c++17
LDFLAGS = 
SRCDIR = src
BUILDDIR = build
BINDIR = bin
INCLUDEDIR = include
SRC = $(shell find $(SRCDIR) -type f -name *.cpp -printf "%f ")
OBJ = $(SRC:.cpp=.o) # like SRC but .o instead of .cpp
LIB = -lz -lpthread
INC =-I$(INCLUDEDIR)
EXEC = server
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SOURCES))

start:
	./$(BINDIR)/$(EXEC)

all : $(BINDIR)/$(EXEC)

# -lz link to the zlib library
$(BINDIR)/$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INC) -o $(BINDIR)/$(EXEC) $(wildcard lib/uWebSockets/uSockets/*.o) $^ $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

clean:
	@echo " Cleaning"
	find $(BUILDDIR) $(BINDIR) ! -name '.gitignore' -type f -exec rm -f {} +


#uSockets
us:
	cd lib/uWebSockets/uSockets && WITH_SSL=0 make
#uWebSockets
uws:
	$(CXX) $(CXXFLAGS) -o $(INCLUDEDIR)/App.h.gch lib/uWebSockets/src/App.h -Ilib/uWebSockets/src -Ilib/uWebSockets/uSockets/src


# test
hel:
	g++ -std=c++17 -flto -O3 test/hel.cpp lib/uWebSockets/uSockets/*.o -Iinclude -Winvalid-pch -lpthread -lz

.PHONY: clean

