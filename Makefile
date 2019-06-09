CXX     = g++
CXXFLAGS = -std=c++17
LDLIBS  = -lstdc++fs `pkg-config opencv --cflags --libs`
WFLAGS = -Wall -Wextra -O3

fuzzy_color: fuzzy_color_enhancement.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(WFLAGS) $(LDLIBS) 

fuzzy_grayscale: fuzzy_grayscale_enhancement.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(WFLAGS) $(LDLIBS) 

.PHONY: clean

clean:
	rm -rf fuzzy_color fuzzy_grayscale
