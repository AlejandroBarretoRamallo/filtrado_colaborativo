#include "UtilityMatrix.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

UtilityMatrix::UtilityMatrix() : numUsers(0), numItems(0), minRating(0.0), maxRating(0.0) {}

bool UtilityMatrix::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: No se puede abrir el archivo " << filename << endl;
        return false;
    }
    
    string line;
    
    // Leer rango de calificaciones
    getline(file, line);
    minRating = stod(line);
    getline(file, line);
    maxRating = stod(line);
    
    // Leer matriz de utilidad
    while (getline(file, line)) {
        vector<double> userRatings;
        stringstream ss(line);
        string value;
        
        while (ss >> value) {
            if (value == "-") {
                userRatings.push_back(-1.0); // -1 indica valor faltante
            } else {
                userRatings.push_back(stod(value));
            }
        }
        
        if (!userRatings.empty()) {
            ratings.push_back(userRatings);
        }
    }
    
    numUsers = ratings.size();
    numItems = numUsers > 0 ? ratings[0].size() : 0;
    
    file.close();
    return true;
}

int UtilityMatrix::getNumUsers() const { 
    return numUsers; 
}

int UtilityMatrix::getNumItems() const { 
    return numItems; 
}

double UtilityMatrix::getMinRating() const { 
    return minRating; 
}

double UtilityMatrix::getMaxRating() const { 
    return maxRating; 
}

double UtilityMatrix::getRating(int user, int item) const { 
    return ratings[user][item]; 
}

void UtilityMatrix::setRating(int user, int item, double value) { 
    ratings[user][item] = value; 
}

bool UtilityMatrix::isMissing(int user, int item) const { 
    return ratings[user][item] == -1.0; 
}

double UtilityMatrix::getUserMean(int user) const {
    double sum = 0.0;
    int count = 0;
    for (int i = 0; i < numItems; i++) {
        if (!isMissing(user, i)) {
            sum += ratings[user][i];
            count++;
        }
    }
    return count > 0 ? sum / count : 0.0;
}

void UtilityMatrix::print() {
  if (numUsers >= 25 || numItems >= 25) {
      cout << "Matriz demasiado grande, imprimiendo solo resultados\n";
      return;
    }
    cout << "\n=== MATRIZ DE UTILIDAD ===" << endl;
    cout << "Usuarios: " << numUsers << ", Items: " << numItems << endl;
    cout << "Rango: [" << minRating << ", " << maxRating << "]" << endl << endl;
    
    cout << setw(8) << "Usuario";
    for (int j = 0; j < numItems; j++) {
        cout << setw(8) << "Item" + to_string(j);
    }
    cout << endl;
    
    for (int i = 0; i < numUsers; i++) {
        cout << setw(8) << "U" + to_string(i);
        for (int j = 0; j < numItems; j++) {
            if (isMissing(i, j)) {
                std::pair<int, int> prediccion;
                prediccion.first = i;
                prediccion.second = j;
                predicciones_.push_back(prediccion);
                cout << setw(8) << "-";
            } else {

                cout << setw(8) << fixed << setprecision(3) << ratings[i][j];
            }
        }
        cout << endl;
    }
    cout << endl;
}

void UtilityMatrix::printPredictions() {
    if (numUsers >= 25 || numItems >= 25) {
      cout << "Matriz demasiado grande, imprimiendo solo resultados\n";
      return;
    }
    cout << "\n=== MATRIZ DE UTILIDAD ===" << endl;
    cout << "Usuarios: " << numUsers << ", Items: " << numItems << endl;
    cout << "Rango: [" << minRating << ", " << maxRating << "]" << endl << endl;
    
    cout << setw(8) << "Usuario";
    for (int j = 0; j < numItems; j++) {
        cout << setw(8) << "Item" + to_string(j);
    }
    cout << endl;
    
    for (int i = 0; i < numUsers; i++) {
        cout << setw(8) << "U" + to_string(i);
        for (int j = 0; j < numItems; j++) {    
          std::pair<int, int> par(i, j);
          for (auto par_mat : predicciones_) {
            if (par_mat == par) {
              cout << "\033[31m";
            }
          }
          cout << setw(8) << fixed << setprecision(3) << ratings[i][j];
          cout << "\033[0m";
        }
        cout << endl;
    }
    cout << endl;
}
