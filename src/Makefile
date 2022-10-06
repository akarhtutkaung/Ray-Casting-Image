CXX=clang++
CXXFLAGS=-g -std=c++11 -Wall

all: raytracer
	rm -f *.o
clean:
	rm -f *.o *.h.gch raytracer
test: rayTracing
	./rayTracing
.PHONY: all clean test
raytracer: rayTracing.o calculation.o vector3.o storeData.o
	$(CXX) $(LDFLAGS) -o $(@) $(^)
rayTracing.o: rayTracing.cpp header.h
	$(CXX) $(CXXFLAGS) -c -o $(@) $(<)
calculation.o: calculation.cpp header.h
	$(CXX) $(CXXFLAGS) -c -o $(@) $(<)
vector3.o: vector3.cpp header.h
	$(CXX) $(CXXFLAGS) -c -o $(@) $(<)
storeData.o: storeData.cpp header.h
	$(CXX) $(CXXFLAGS) -c -o $(@) $(<)