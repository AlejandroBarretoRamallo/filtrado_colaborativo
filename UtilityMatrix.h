#ifndef UTILITY_MATRIX_H
#define UTILITY_MATRIX_H

#include <vector>
#include <string>

using namespace std;

class UtilityMatrix {
private:
    int numUsers;
    int numItems;
    double minRating;
    double maxRating;
    vector<vector<double>> ratings;
    vector<pair<int, int>> predicciones_;
    
public:
    UtilityMatrix();
    bool loadFromFile(const string& filename);
    
    int getNumUsers() const;
    int getNumItems() const;
    double getMinRating() const;
    double getMaxRating() const;
    double getRating(int user, int item) const;
    void setRating(int user, int item, double value);
    bool isMissing(int user, int item) const;
    double getUserMean(int user) const;
    void print();
    void printPredictions();
};

#endif
