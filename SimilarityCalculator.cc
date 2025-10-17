#include "SimilarityCalculator.h"
#include <cmath>
#include <vector>

using namespace std;

SimilarityCalculator::SimilarityCalculator(const UtilityMatrix& m, Metric met) 
    : matrix(m), metric(met) {}

double SimilarityCalculator::pearsonCorrelation(int user1, int user2) const {
    vector<double> x, y;
    
    // Recopilar items calificados por ambos usuarios
    for (int i = 0; i < matrix.getNumItems(); i++) {
        if (!matrix.isMissing(user1, i) && !matrix.isMissing(user2, i)) {
            x.push_back(matrix.getRating(user1, i));
            y.push_back(matrix.getRating(user2, i));
        }
    }
    
    if (x.size() < 2) return 0.0;
    
    // Calcular medias
    double meanX = 0.0, meanY = 0.0;
    for (size_t i = 0; i < x.size(); i++) {
        meanX += x[i];
        meanY += y[i];
    }
    meanX /= x.size();
    meanY /= y.size();
    
    // Calcular correlación
    double numerator = 0.0, denomX = 0.0, denomY = 0.0;
    for (size_t i = 0; i < x.size(); i++) {
        double dx = x[i] - meanX;
        double dy = y[i] - meanY;
        numerator += dx * dy;
        denomX += dx * dx;
        denomY += dy * dy;
    }
    
    double denominator = sqrt(denomX * denomY);
    return denominator > 0.0 ? numerator / denominator : 0.0;
}

double SimilarityCalculator::cosineSimilarity(int user1, int user2) const {
    double dotProduct = 0.0, norm1 = 0.0, norm2 = 0.0;
    int commonItems = 0;
    
    for (int i = 0; i < matrix.getNumItems(); i++) {
        if (!matrix.isMissing(user1, i) && !matrix.isMissing(user2, i)) {
            double r1 = matrix.getRating(user1, i);
            double r2 = matrix.getRating(user2, i);
            dotProduct += r1 * r2;
            norm1 += r1 * r1;
            norm2 += r2 * r2;
            commonItems++;
        }
    }
    
    if (commonItems == 0) return 0.0;
    
    double denominator = sqrt(norm1 * norm2);
    return denominator > 0.0 ? dotProduct / denominator : 0.0;
}

double SimilarityCalculator::euclideanSimilarity(int user1, int user2) const {
    double sumSquares = 0.0;
    int commonItems = 0;
    
    for (int i = 0; i < matrix.getNumItems(); i++) {
        if (!matrix.isMissing(user1, i) && !matrix.isMissing(user2, i)) {
            double diff = matrix.getRating(user1, i) - matrix.getRating(user2, i);
            sumSquares += diff * diff;
            commonItems++;
        }
    }
    
    if (commonItems == 0) return 0.0;
    
    double distance = sqrt(sumSquares);
    return 1.0 / (1.0 + distance); // Convertir distancia a similitud
}

double SimilarityCalculator::calculateSimilarity(int user1, int user2) const {
    switch (metric) {
        case PEARSON:
            return pearsonCorrelation(user1, user2);
        case COSINE:
            return cosineSimilarity(user1, user2);
        case EUCLIDEAN:
            return euclideanSimilarity(user1, user2);
        default:
            return 0.0;
    }
}
