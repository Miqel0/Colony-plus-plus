CXX = g++
CXXFLAGS = -Wall -O2

OBJS = build/main.o \
       build/logistics.o \
       build/colony.o \
       build/housing.o \
       build/farm.o \
       build/energy.o \
       build/terr.o \
       build/producer.o \
       build/building.o

main: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o main

build/main.o: src/main.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o build/main.o

build/logistics.o: src/logistics.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c src/logistics.cpp -o build/logistics.o

build/colony.o: src/colony.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c src/colony.cpp -o build/colony.o

build/housing.o: src/housing.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c src/housing.cpp -o build/housing.o

build/farm.o: src/farm.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c src/farm.cpp -o build/farm.o

build/energy.o: src/energy.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c src/energy.cpp -o build/energy.o

build/terr.o: src/terr.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c src/terr.cpp -o build/terr.o

build/producer.o: src/producer.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c src/producer.cpp -o build/producer.o

build/building.o: src/building.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c src/building.cpp -o build/building.o

clean:
	rm -rf build main
