#!make -f
# This Makefile can handle any set of cpp and hpp files.
# To use it, you should put all your cpp and hpp files in the SOURCE_PATH folder.
CXX=clang++-9
CXXFLAGS = -g -Wall -pthread
SOURCE_PATH=sources
OBJECT_PATH=objects
VALGRIND_FLAGS=-v --leak-check=full --show-leak-kinds=all  --error-exitcode=99

SOURCES=$(wildcard $(SOURCE_PATH)/*.cpp)
HEADERS=$(wildcard $(SOURCE_PATH)/*.hpp)
OBJECTS=$(subst sources/,objects/,$(subst .cpp,.o,$(SOURCES)))
SERVER_OBJ = $(filter-out objects/client.o, $(OBJECTS))
CLIENT_OBJ = $(filter-out objects/multi_server.o, $(OBJECTS))

all: server client

client: $(CLIENT_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

server: $(SERVER_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) --compile $< -o $@

$(OBJECT_PATH)/%.o: $(SOURCE_PATH)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) --compile $< -o $@


valgrind: test
	valgrind --tool=memcheck $(VALGRIND_FLAGS) ./test 2>&1 | { egrep "lost| at " || true; }

.PHONY: clean

clean:
	rm -f $(OBJECTS) *.o 
	rm client server
