CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2

all:
	$(CXX) $(CXXFLAGS) -o recommender main.cc UtilityMatrix.cc SimilarityCalculator.cc RecommenderSystem.cc

clean:
	rm -f recommender
