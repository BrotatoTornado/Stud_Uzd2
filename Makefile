CXX = g++

CXXFLAGS = -std=c++17 -Wall -Wextra

SRCS = main.cpp Studentas.cpp meniu.cpp skaitymas.cpp Generavimas.cpp spausdinam.cpp tyrimas.cpp laikai.cpp testas.cpp Zmogus.cpp

TEST_SRCS = unit_tests.cpp Studentas.cpp Generavimas.cpp skaitymas.cpp spausdinam.cpp Zmogus.cpp

GTEST = -lgtest -lgtest_main -pthread

.PHONY: all clean opt tests

all: programa_vector programa_list programa_deque

programa_vector:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@

programa_list:
	$(CXX) $(CXXFLAGS) -DUSE_LIST $(SRCS) -o $@

programa_deque:
	$(CXX) $(CXXFLAGS) -DUSE_DEQUE $(SRCS) -o $@

opt: programa_vector_O1 programa_vector_O2 programa_vector_O3 tests

programa_vector_O1:
	$(CXX) $(CXXFLAGS) -O1 $(SRCS) -o $@

programa_vector_O2:
	$(CXX) $(CXXFLAGS) -O2 $(SRCS) -o $@

programa_vector_O3:
	$(CXX) $(CXXFLAGS) -O3 $(SRCS) -o $@

tests:
	$(CXX) $(CXXFLAGS) $(TEST_SRCS) $(GTEST) -o unit_tests

clean:
	rm -f programa_vector programa_list programa_deque
	rm -f programa_vector_O1 programa_vector_O2 programa_vector_O3
	rm -f unit_tests
