CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

SRCS = main.cpp Studentas.cpp meniu.cpp skaitymas.cpp Generavimas.cpp spausdinam.cpp tyrimas.cpp laikai.cpp Zmogus.cpp


.PHONY: all

all: programa_vector programa_list programa_deque programa_manoVector

programa_vector: $(SRCS)
	$(CXX) $(CXXFLAGS) -O2 $^ -o $@

programa_list: $(SRCS)
	$(CXX) $(CXXFLAGS) -O2 -DUSE_LIST $^ -o $@

programa_deque: $(SRCS)
	$(CXX) $(CXXFLAGS) -O2 -DUSE_DEQUE $^ -o $@

programa_manoVector: $(SRCS)
	$(CXX) $(CXXFLAGS) -O2 -DUSE_MYVECTOR $^ -o $@

.PHONY: opt_vector

opt_vector: programa_vector_O1 programa_vector_O2 programa_vector_O3

programa_vector_O1: $(SRCS)
	$(CXX) $(CXXFLAGS) -O1 $^ -o $@

programa_vector_O2: $(SRCS)
	$(CXX) $(CXXFLAGS) -O2 $^ -o $@

programa_vector_O3: $(SRCS)
	$(CXX) $(CXXFLAGS) -O3 $^ -o $@

.PHONY: opt_manoVector

opt_manoVector: programa_manoVector_O1 programa_manoVector_O2 programa_manoVector_O3

programa_manoVector_O1: $(SRCS)
	$(CXX) $(CXXFLAGS) -O1 -DUSE_MYVECTOR $^ -o $@

programa_manoVector_O2: $(SRCS)
	$(CXX) $(CXXFLAGS) -O2 -DUSE_MYVECTOR $^ -o $@

programa_manoVector_O3: $(SRCS)
	$(CXX) $(CXXFLAGS) -O3 -DUSE_MYVECTOR $^ -o $@

.PHONY: benchmark
benchmark: spartos_analize

spartos_analize: spartos_analize.cpp
	$(CXX) $(CXXFLAGS) -O2 $^ -o $@

.PHONY: test
test: vector_testavimas

vector_testavimas: vector_testavimas.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ -lgtest -lgtest_main -pthread

.PHONY: docs
docs:
	doxygen Doxyfile

clean:
	rm -f programa_vector.exe programa_list.exe programa_deque.exe programa_vector_O1.exe programa_vector_O2.exe programa_vector_O3.exe programa_manoVector.exe programa_manoVector_O1.exe programa_manoVector_O2.exe programa_manoVector_O3.exe vector_testavimas.exe spartos_analize.exe
