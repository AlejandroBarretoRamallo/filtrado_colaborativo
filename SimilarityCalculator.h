#ifndef SIMILARITY_CALCULATOR_H
#define SIMILARITY_CALCULATOR_H

#include "UtilityMatrix.h"

enum Metric {
    PEARSON,
    COSINE,
    EUCLIDEAN
};

class SimilarityCalculator {
private:
    const UtilityMatrix& matrix;
    Metric metric;
    
    double pearsonCorrelation(int user1, int user2) const;
    double cosineSimilarity(int user1, int user2) const;
    double euclideanSimilarity(int user1, int user2) const;
    
public:
    SimilarityCalculator(const UtilityMatrix& m, Metric met);
    double calculateSimilarity(int user1, int user2) const;
};

#endif
