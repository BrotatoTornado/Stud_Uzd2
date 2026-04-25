CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

SRCS = main.cpp Studentas.cpp meniu.cpp skaitymas.cpp Generavimas.cpp spausdinam.cpp tyrimas.cpp laikai.cpp Zmogus.cpp


.PHONY: all clean

all: programa_vector programa_list programa_deque

programa_vector: $(SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@

programa_list: $(SRCS)
	$(CXX) $(CXXFLAGS) -DUSE_LIST $^ -o $@

programa_deque: $(SRCS)
	$(CXX) $(CXXFLAGS) -DUSE_DEQUE $^ -o $@


.PHONY: opt

opt: programa_vector_O1 programa_vector_O2 programa_vector_O3

programa_vector_O1: $(SRCS)
	$(CXX) $(CXXFLAGS) -O1 $^ -o $@

programa_vector_O2: $(SRCS)
	$(CXX) $(CXXFLAGS) -O2 $^ -o $@

programa_vector_O3: $(SRCS)
	$(CXX) $(CXXFLAGS) -O3 $^ -o $@


clean:
	rm -f programa_vector programa_list programa_deque programa_vector_O1 programa_vector_O2 programa_vector_O3
