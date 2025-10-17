#ifndef RECOMMENDER_SYSTEM_H
#define RECOMMENDER_SYSTEM_H

#include "UtilityMatrix.h"
#include "SimilarityCalculator.h"
#include <vector>
#include <utility>

enum PredictionType {
    SIMPLE,
    MEAN_DIFF
};

class RecommenderSystem {
private:
    UtilityMatrix matrix;
    Metric metric;
    int numNeighbors;
    PredictionType predictionType;
    vector<vector<double>> similarities;
    
    void calculateAllSimilarities();
    vector<pair<int, double>> getNeighbors(int user, int item, int k) const;
    double simplePrediction(int user, int item, const vector<pair<int, double>>& neighbors) const;
    double meanDiffPrediction(int user, int item, const vector<pair<int, double>>& neighbors) const;
    
public:
    RecommenderSystem(const string& filename, Metric m, int k, PredictionType pt);
    void run();
    void printSimilarities() const;
    void makePredictions();
    void generateRecommendations();
};

#endif
